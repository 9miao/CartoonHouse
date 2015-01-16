

#include "DMBrowseController.h"
#include "DMSettingManager.h"
#include "DMUserManager.h"
#include "DMColllectionManager.h"
#include "NetWorkUtils/NetWorkUtils.h"
#include "DMSettingController.h"

static std::string getNetWorkUtils()
{
    std::string relust;
    const NetWorkType& type = NetWorkUtils::getInstance()->getNewWorkType();
    switch (type)
    {
        case NetWorkTypeNone:
            relust = "无网络";
            break;
        case NetWorkType3G:
            relust = "3G";
            break;
        case NetWorkTypeWifi:
            relust = "WIFI";
            break;
        default:
            break;
    }
    return relust;
}

std::string _getImagePath(DMBrowseModel* browseModel, int index)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "download/"
    + browseModel->comic_id + "/"
    + browseModel->chapter_id + "/";
    
    string str_index;
    stringstream ss;
    ss << index;
    ss >> str_index;
    
    path += str_index + ".jpg";
    
    return path;
}

bool _isDownloaded(DMBrowseModel* browseModel)
{
    bool relust = false;
    FILE* fp = fopen(_getImagePath(browseModel, 0).c_str(), "r");
    if (fp)
    {
        fseek(fp, 0L, SEEK_END);
        relust = ftell(fp) > 0;
        fclose(fp);
    }
    return relust;
}

DMBrowseController::DMBrowseController(const std::string& comicID, const std::vector<DMIntroductionItemInfo>& vec, int index)
: m_pVerticalView(NULL)
, m_pHorizontalView(NULL)
, m_sComicID(comicID)
, m_iCharIndex(index)
, m_obIntroductionItemInfos(vec)
, m_nCurrPage(-0xffff)
, m_bBrowseVertical(false)
, m_iRequestFinish(0)
, m_pLoading(NULL)
{
    this->getView()->setDisplayRange(false);
    m_pCurrBrowseModel = new DMBrowseModel();
    m_pPrevBrowseModel = new DMBrowseModel();
    m_pNextBrowseModel = new DMBrowseModel();
    
    DMSettingManager* settingmanger = new DMSettingManager();
    m_bOnLineWatch = settingmanger->getSetOnLineWatch() == On;
    delete settingmanger;
}

DMBrowseController::~DMBrowseController()
{
    delete m_pCurrBrowseModel;
    delete m_pPrevBrowseModel;
    delete m_pNextBrowseModel;
}

void DMBrowseController::viewDidLoad()
{
    this->getView()->setColor(CAColor_black);
    
    m_fTableViewCellInitialAspectRatio = this->getView()->getBounds().size.height / (this->getView()->getBounds().size.width - _px(20));
    
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeChapPages, m_sComicID, m_obIntroductionItemInfos.at(m_iCharIndex).ID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this,
                                               CommonHttpJson_selector(DMBrowseController::onCurrRequestFinished));


    
    this->requestPrevData();
    this->requestNextData();
    
    {
        CCRect loadingRect = this->getView()->getBounds();
        loadingRect.origin.y = _px(96);
        loadingRect.size.height -= loadingRect.origin.y;
        m_pLoading = CAActivityIndicatorView::createWithFrame(loadingRect);
        CAImageView* indicator = CAImageView::createWithFrame(CADipRect(0, 0, 50, 50));
        indicator->setImage(CAImage::create(loadingIcon));
        m_pLoading->setActivityIndicatorView(indicator);
        CAView* bg = CAView::createWithFrame(CADipRect(0, 0, 275, 300), CAColor_clear);
        CAImageView* bg2 = CAImageView::createWithFrame(CADipRect(0, 0, 275, 100));
        bg2->setImage(CAImage::create(loadingBackground));
        bg->addSubview(bg2);
        m_pLoading->setActivityBackView(bg);
        this->getView()->insertSubview(m_pLoading, CAWindowZoderTop);
    }
    
    UMENG_EventBeginValue("browse", m_sTitle.c_str());
}

void DMBrowseController::viewDidUnload()
{
    for (size_t i=0; i<m_pBrowseImageViews.size(); i++)
    {
        m_pBrowseImageViews.at(i)->setDelegate(NULL);
    }
    
    if (m_pVerticalView)
    {
        m_pVerticalView->removeAllSubviews();
    }
    
    if (m_pHorizontalView)
    {
        m_pHorizontalView->removeAllSubviews();
    }
    UMENG_EventEndValue("browse", m_sTitle.c_str());
}

void DMBrowseController::setBrowseVertical(bool var)
{
    m_bBrowseVertical = var;
    
    this->getView()->removeSubview(m_pVerticalView);
    m_pVerticalView = NULL;
    this->getView()->removeSubview(m_pHorizontalView);
    m_pHorizontalView = NULL;
    
    if (this->isBrowseVertical())
    {
        m_pVerticalView = new DMBrowseVerticalView(this);
        m_pVerticalView->setFrame(this->getView()->getBounds());
        m_pVerticalView->init();
        m_pVerticalView->autorelease();
        this->getView()->addSubview(m_pVerticalView);
    }
    else
    {
        m_pHorizontalView = new DMBrowseHorizontalView(this);
        m_pHorizontalView->setCenter(CCRect(this->getView()->getBounds().size.width/2,
                                            this->getView()->getBounds().size.height/2,
                                            this->getView()->getBounds().size.height,
                                            this->getView()->getBounds().size.width));
        m_pHorizontalView->setRotation(90);
        m_pHorizontalView->init();
        m_pHorizontalView->autorelease();
        this->getView()->addSubview(m_pHorizontalView);
    }
    
    //更新UI，当前只有是否收藏项
    if (this->isBrowseVertical())
    {
        m_pVerticalView->updateUI();
    }
    else
    {
        m_pHorizontalView->updateUI();
    }
    
}

void DMBrowseController::requestPrevData()
{
    CC_RETURN_IF(m_iCharIndex <= 0);

    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeChapPages, m_sComicID, m_obIntroductionItemInfos.at(m_iCharIndex - 1).ID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this,
                                               CommonHttpJson_selector(DMBrowseController::onPrevRequestFinished));
}

void DMBrowseController::requestNextData()
{
    CC_RETURN_IF(m_obIntroductionItemInfos.size() - m_iCharIndex <= 1);

    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeChapPages, m_sComicID,m_obIntroductionItemInfos.at(m_iCharIndex + 1).ID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this,
                                               CommonHttpJson_selector(DMBrowseController::onNextRequestFinished));
}

#define ___SIZE___ 4
#define IMAGEVIEW_SIZE (___SIZE___ * 2 + 1)

CommonUrlImageView* DMBrowseController::createWithUrlImageView(int* index)
{
    CommonUrlImageView* imageView = CommonUrlImageView::createWithImage(NULL);
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    imageView->setImageType(HttpGetImageNoMemoryCache);
    imageView->setDelegate(this);
    this->showImage(imageView, index);
    return imageView;
}

void DMBrowseController::showImage(CommonUrlImageView* imageView, int* index)
{
    bool isRequestImage = m_bOnLineWatch || NetWorkUtils::getInstance()->getNewWorkType() == NetWorkTypeWifi;
    
    DMBrowseModel* browseModel = NULL;
    
    if (*index < 0)
    {
        CC_RETURN_IF(m_pPrevBrowseModel->imageUrls.empty());
        *index = m_pPrevBrowseModel->imageUrls.size() + *index;
        browseModel = m_pPrevBrowseModel;
        
    }
    else if (*index >= 0 && *index < m_pCurrBrowseModel->imageUrls.size())
    {
        browseModel = m_pCurrBrowseModel;
    }
    else
    {
        CC_RETURN_IF(m_pNextBrowseModel->imageUrls.empty());
        *index = *index - m_pCurrBrowseModel->imageUrls.size();
        browseModel = m_pNextBrowseModel;
    }
    
    if (browseModel)
    {
        CC_RETURN_IF(*index < 0);
        CC_RETURN_IF(*index >= browseModel->imageUrls.size());
        imageView->setTextTag(browseModel->chapter_id);
        imageView->setTag(*index);
        
        std::map<std::string, std::string> map;
        
        if (m_pPrevBrowseModel->isDowloaded)
        {
            imageView->setImageAsyncWithFile(_getImagePath(browseModel, *index));
            map["加载方式"] = "读取已下载";
            
        }
        else if (isRequestImage)
        {
            imageView->setUrl(browseModel->imageUrls.at(*index));
            map["加载方式"] = "直接请求";
        }
        else
        {
            imageView->setUrlOnlyReadCache(browseModel->imageUrls.at(*index));
            map["加载方式"] = "直接请求";
            
        }
        map["网络类型"] = getNetWorkUtils();
        UMENG_EventMap("browse_showImage", map);
    }
}

void DMBrowseController::updateTitle()
{
    this->setTitle(m_pCurrBrowseModel->chapter_name);
    if (this->isBrowseVertical())
    {
        m_pVerticalView->updateTitle();
    }
    else
    {
        m_pHorizontalView->updateTitle();
    }
}

void DMBrowseController::setCurrPage(int var)
{
    CC_RETURN_IF(m_nCurrPage == var);
    int off_index = var - m_nCurrPage;
    int old_currPage = m_nCurrPage;
    m_nCurrPage = var;
    
    int size = m_pCurrBrowseModel->imageUrls.size();
    CC_RETURN_IF(size == 0);
    if (off_index <= -IMAGEVIEW_SIZE || off_index >= IMAGEVIEW_SIZE)
    {
        m_pBrowseImageViews.clear();
        for (int i=0; i<IMAGEVIEW_SIZE; i++)
        {
            int n = i + m_nCurrPage - ___SIZE___;
            m_pBrowseImageViews.pushBack(this->createWithUrlImageView(&n));
            
            CALabel* label = NULL;
            if (m_pPageLabels.size() <= i)
            {
                label = CALabel::createWithFrame(CADipRect(0, 0, 100, 100));
                label->setTextAlignment(CATextAlignmentCenter);
                label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                label->setFontSize(_px(30));
                label->setColor(CAColor_white);
                m_pPageLabels.pushBack(label);
                
                CAActivityIndicatorView * activityView = CAActivityIndicatorView::createWithFrame(label->getBounds());
                CAImageView* indicator = CAImageView::createWithCenter(CADipRect(50, 49, 72, 72));
                indicator->setImage(CAImage::create(loadingIcon));
                activityView->setActivityIndicatorView(indicator);
                label->insertSubview(activityView, -1);
            }
            else
            {
                label = m_pPageLabels.at(i);
            }

            char str[8];
            sprintf(str, "%d", n + 1);
            label->setText(str);
            
        }
    }
    else if (off_index > 0)
    {
        for (int i=0; i<off_index; i++)
        {
            m_pBrowseImageViews.popFront();
            int n = i + (___SIZE___ + 1) + old_currPage;
            m_pBrowseImageViews.pushBack(this->createWithUrlImageView(&n));
            
            CALabel* label = m_pPageLabels.front();
            label->retain()->autorelease();
            m_pPageLabels.popFront();
            char str[8];
            sprintf(str, "%d", n + 1);
            label->setText(str);
            m_pPageLabels.pushBack(label);
        }
    }
    else
    {
        for (int i=0; i>off_index; i--)
        {
            m_pBrowseImageViews.popBack();
            int n = i - (___SIZE___ + 1) + old_currPage;
            m_pBrowseImageViews.pushFront(this->createWithUrlImageView(&n));
            
            CALabel* label = m_pPageLabels.back();
            label->retain()->autorelease();
            m_pPageLabels.popBack();
            char str[8];
            sprintf(str, "%d", n + 1);
            label->setText(str);
            m_pPageLabels.pushFront(label);
        }
    }
    
    for (int i=0; i<IMAGEVIEW_SIZE; i++)
    {
        int n = i + m_nCurrPage - ___SIZE___;
        if (m_pBrowseImageViews.at(i)->getTag() == kCAObjectTagInvalid)
        {
            this->showImage(m_pBrowseImageViews.at(i), &n);
            char str[8];
            sprintf(str, "%d", n + 1);
            m_pPageLabels.at(i)->setText(str);
        }
    }

}

int DMBrowseController::detectingWhetherTurnThePage()
{
    do
    {
        if (m_nCurrPage < 0)
        {
            CC_BREAK_IF(m_iCharIndex == 0);
            m_nCurrPage = m_pPrevBrowseModel->imageUrls.size() + m_nCurrPage;
            CC_SAFE_DELETE(m_pNextBrowseModel);
            m_pNextBrowseModel = m_pCurrBrowseModel;
            m_pCurrBrowseModel = m_pPrevBrowseModel;
            m_pPrevBrowseModel = new DMBrowseModel();
            --m_iCharIndex;
            m_iRequestFinish = -1;
            this->requestPrevData();
            this->updateTitle();
            
            return -1;
        }
        else if (m_nCurrPage >= m_pCurrBrowseModel->imageUrls.size())
        {
            CC_BREAK_IF(m_iCharIndex == m_obIntroductionItemInfos.size() - 1);
            m_nCurrPage = m_nCurrPage - m_pCurrBrowseModel->imageUrls.size();;
            CC_SAFE_DELETE(m_pPrevBrowseModel);
            m_pPrevBrowseModel = m_pCurrBrowseModel;
            m_pCurrBrowseModel = m_pNextBrowseModel;
            m_pNextBrowseModel = new DMBrowseModel();
            ++m_iCharIndex;
            m_iRequestFinish = -1;
            this->requestNextData();
            this->updateTitle();
            
            return 1;
        }
        else break;
    }
    while (0);
    
    
    return 0;
}

bool DMBrowseController::isBrowseVertical()
{
    return m_bBrowseVertical;
}

void DMBrowseController::viewDidAppear()
{
    RootWindow::getInstance()->getDrawerController()->setTouchMoved(false);
    RootWindow::getInstance()->getRootNavigationController()->setTouchMoved(false);
}

void DMBrowseController::viewDidDisappear()
{
    RootWindow::getInstance()->getDrawerController()->setTouchMoved(true);
    RootWindow::getInstance()->getRootNavigationController()->setTouchMoved(true);
}

void DMBrowseController::onCurrRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["page"];
        
        do
        {
            CC_BREAK_IF(value.size() == 0);
            CC_BREAK_IF(value.size() == 1 && value[0].asString().empty());
            
            m_pCurrBrowseModel->imageUrls.resize(value.size());
            for (int i=0; i<value.size(); i++)
            {
                m_pCurrBrowseModel->imageUrls[i] = value[i].asString();
            }
            
            m_pCurrBrowseModel->chapter_id = json["id"].asString();
            m_pCurrBrowseModel->comic_id = json["comic_id"].asString();
            m_pCurrBrowseModel->chapter_name = json["chapter_name"].asString();
            m_pCurrBrowseModel->isDowloaded = _isDownloaded(m_pCurrBrowseModel);
            
            std::vector<float> aspectRatios;
            aspectRatios.resize(m_pCurrBrowseModel->imageUrls.size());
            for (int i=0; i<aspectRatios.size(); i++)
            {
                aspectRatios[i] = this->getTableViewCellInitialAspectRatio();
            }
            m_fAspectRatioss[m_pCurrBrowseModel->chapter_id] = aspectRatios;
        }
        while (0);

        this->requestFinishOnce();
    }
}

void DMBrowseController::onPrevRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    do
    {
        if (status == HttpResponseSucceed)
        {
            const CSJson::Value& value = json["page"];
            do
            {
                CC_BREAK_IF(value.size() == 0);
                CC_BREAK_IF(value.size() == 1 && value[0].asString().empty());
                
                m_pPrevBrowseModel->imageUrls.resize(value.size());
                for (int i=0; i<value.size(); i++)
                {
                    m_pPrevBrowseModel->imageUrls[i] = value[i].asString();
                }
                m_pPrevBrowseModel->chapter_id = json["id"].asString();
                m_pPrevBrowseModel->comic_id = json["comic_id"].asString();
                m_pPrevBrowseModel->chapter_name = json["chapter_name"].asString();
                m_pPrevBrowseModel->isDowloaded = _isDownloaded(m_pPrevBrowseModel);
                
                std::vector<float> aspectRatios;
                aspectRatios.resize(m_pPrevBrowseModel->imageUrls.size());
                for (int i=0; i<aspectRatios.size(); i++)
                {
                    aspectRatios[i] = this->getTableViewCellInitialAspectRatio();
                }
                m_fAspectRatioss[m_pPrevBrowseModel->chapter_id] = aspectRatios;

            }
            while (0);
            
        }

    }
    while (0);
    
    this->requestFinishOnce();
}

void DMBrowseController::onNextRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    do
    {
        if (status == HttpResponseSucceed)
        {
            const CSJson::Value& value = json["page"];
            do
            {
                CC_BREAK_IF(value.size() == 0);
                CC_BREAK_IF(value.size() == 1 && value[0].asString().empty());
                
                m_pNextBrowseModel->imageUrls.resize(value.size());
                for (int i=0; i<value.size(); i++)
                {
                    m_pNextBrowseModel->imageUrls[i] = value[i].asString();
                }
                m_pNextBrowseModel->chapter_id = json["id"].asString();
                m_pNextBrowseModel->comic_id = json["comic_id"].asString();
                m_pNextBrowseModel->chapter_name = json["chapter_name"].asString();
                m_pNextBrowseModel->isDowloaded = _isDownloaded(m_pNextBrowseModel);
                
                std::vector<float> aspectRatios;
                aspectRatios.resize(m_pNextBrowseModel->imageUrls.size());
                for (int i=0; i<aspectRatios.size(); i++)
                {
                    aspectRatios[i] = this->getTableViewCellInitialAspectRatio();
                }
                m_fAspectRatioss[m_pNextBrowseModel->chapter_id] = aspectRatios;
                
            }
            while (0);
        }
    }
    while (0);
    
    this->requestFinishOnce();
}

void DMBrowseController::requestFinishOnce()
{
    ++m_iRequestFinish;
    CC_RETURN_IF(m_iRequestFinish < 2 && m_obIntroductionItemInfos.size() > 1);
    CC_RETURN_IF(m_iRequestFinish < 3 && m_iCharIndex > 0 && m_iCharIndex < m_obIntroductionItemInfos.size() - 1);

    m_pLoading->stopAnimating();
    
    this->setCurrPage(0);
    DMSettingManager mgr;
    if (mgr.getSetDisplayModel() == On)
    {
        this->setBrowseVertical(false);
    }
    else
    {
        this->setBrowseVertical(true);
    }
    
    CC_RETURN_IF(m_pCurrBrowseModel->imageUrls.empty());
    
    if (this->isBrowseVertical())
    {
        m_pVerticalView->updatePageView();
    }
    else
    {
        m_pHorizontalView->updateTableView();
    }

}

void DMBrowseController::imageViewRequestFinished(CommonUrlImageView* imageView)
{
    const std::string& key = imageView->getTextTag();
    int row = imageView->getTag();
    CC_RETURN_IF(row < 0);
    CC_RETURN_IF(m_fAspectRatioss[key].size() <= row);
    CC_RETURN_IF(m_fAspectRatioss[key].at(row) == imageView->getImage()->getAspectRatio());
    m_fAspectRatioss[key].at(row) = imageView->getImage()->getAspectRatio();
    if (!this->isBrowseVertical())
    {
        if (imageView->getTextTag().compare(m_pPrevBrowseModel->chapter_id) == 0)
        {
            row = row - m_pPrevBrowseModel->imageUrls.size();
        }
        if (imageView->getTextTag().compare(m_pNextBrowseModel->chapter_id) == 0)
        {
            row = row + m_pCurrBrowseModel->imageUrls.size();
        }
        m_pHorizontalView->updateOffset(row);
    }
}

const std::string& DMBrowseController::getKeyAtSection(unsigned int section)
{
    switch (section)
    {
        case 0:
            return m_pPrevBrowseModel->chapter_id;
            break;
        case 1:
            return m_pCurrBrowseModel->chapter_id;
            break;
        default:
            return m_pNextBrowseModel->chapter_id;
            break;
    }
}

bool DMBrowseController::isTheEndOfThePage()
{
    if (m_obIntroductionItemInfos.size() - m_iCharIndex > 1)
    {
        return false;
    }
    
    if (m_pCurrBrowseModel->imageUrls.size() - m_nCurrPage > 1)
    {
        return false;
    }
    
    return true;
}

bool DMBrowseController::isTheTopOfThePage()
{
    if (m_iCharIndex > 0)
    {
        return false;
    }
    
    if (m_nCurrPage > 0)
    {
        return false;
    }
    
    return true;
}


float DMBrowseController::getTableViewCellHeight(unsigned int section, unsigned int index)
{
    if (m_fAspectRatioss.count(this->getKeyAtSection(section)) == 0)
    {
        return this->getView()->getBounds().size.width - _px(20);
    }
    const std::vector<float>& aspectRatios = m_fAspectRatioss[this->getKeyAtSection(section)];
    if (aspectRatios.size() <= index)
    {
        return this->getView()->getBounds().size.width - _px(20);
    }
    return this->getView()->getBounds().size.height / aspectRatios.at(index);
}

float DMBrowseController::getTableViewSectionHeight(unsigned int section)
{
    float height = 0;
    const std::vector<float>& aspectRatios = m_fAspectRatioss[this->getKeyAtSection(section)];
    for (std::vector<float>::const_iterator itr=aspectRatios.begin(); itr!=aspectRatios.end(); itr++)
    {
        height += this->getView()->getBounds().size.height / *itr + _px(20);
    }
    return height;
}

void DMBrowseController::onButton(CAControl* control, CCPoint point)
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}

void DMBrowseController::setReversing(CAControl* control, CCPoint point)
{
    DMSettingManager mgr;
    mgr.setSetDisplayModel(m_bBrowseVertical ? On:Off);
    this->setBrowseVertical(!m_bBrowseVertical);
    if (!m_bBrowseVertical)
    {
        m_pHorizontalView->updateTableView();
    }
}

void DMBrowseController::onActionPerformed(LoginAction action)
{
    if (action == LoginActionLoginSuccess)
    {
        addSubScirbeWithRemote();
    }
}

void DMBrowseController::startLoginController()
{
    DMLoginController *controller = new DMLoginController();
    controller->addUserLoginDelegate(this);
    controller->init();
    controller->autorelease();
    
    CANavigationController *nController = new CANavigationController();
    nController->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    nController->initWithRootViewController(controller);
    nController->autorelease();
    
    this->presentModalViewController(nController, true);
}

void DMBrowseController::onCollectionClicked()
{
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
    
    //只有登陆，并且书已经被收藏，才是删除收藏，其它均为要收藏
    if (uMgr->onLineUser().uid.length() > 0 && cMgr->isCollect(m_sComicID))
    {
        delSubScribeWithRemote();
    }
    else
    {
        addSubScirbeWithRemote();
    }
    
}

void DMBrowseController::addSubScirbeWithRemote()
{
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    //未登陆
    if (uMgr->onLineUser().uid.length() == 0)
    {
        startLoginController();
        return;
    }
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeSubScribAdd, uMgr->onLineUser().uid, m_sComicID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMBrowseController::onAddScribeWithRemoteFinish));
}

void DMBrowseController::onAddScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
        cMgr->deCollect(m_sComicID);
        cMgr->collect(m_sComicID);
        updateCollectionStatus();
    }
}

void DMBrowseController::delSubScribeWithRemote()
{
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeSubScribDel, uMgr->onLineUser().uid, m_sComicID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url,
                                                s_http_url.key_value,
                                                this,
                                                CommonHttpJson_selector(DMBrowseController::onDelScribeWithRemoteFinish));
}

void DMBrowseController::onDelScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
        cMgr->deCollect(m_sComicID);
        cMgr->deCollect(m_sComicID);
        updateCollectionStatus();
    }
}

void DMBrowseController::updateCollectionStatus()
{
    if (m_pVerticalView != NULL)
    {
        m_pVerticalView->updateUI();
    }
    else if (m_pHorizontalView != NULL)
    {
        m_pHorizontalView->updateUI();
    }
    
}

#pragma DMBrowseVerticalView


DMBrowseVerticalView::DMBrowseVerticalView(DMBrowseController* controller)
: m_pController(controller), m_pNavigationBar(NULL)
, m_pSetupBar(NULL), m_pPageView(NULL)
, m_pSlider(NULL), m_bBarShow(false)
, m_pbrightnessButton(NULL), m_pBrowseInfoLabel(NULL)
, m_pDirectionButton(NULL), m_pAboutButton(NULL) 
, m_pCollectButton(NULL), m_pPageInfoLabel(NULL)
, m_pBrightnessSlider(NULL), m_pBrightnessBg(NULL)
, m_iLastPage(1)
{
    m_bLeftModel = false;
    UMENG_EventBeginValue("browse_type", "竖屏");
}

DMBrowseVerticalView::~DMBrowseVerticalView()
{
    UMENG_EventEndValue("browse_type", "竖屏");
}

bool DMBrowseVerticalView::init()
{
    //......NavigationBar
    {
        CCRect rect = m_pController->getView()->getBounds();
        rect.size.height = _px(88);
        rect.origin.y = -rect.size.height;
        
        m_pNavigationBar = CAView::createWithFrame(rect, ccc4(0, 0, 0, 204));
        this->insertSubview(m_pNavigationBar, 1);
        m_pNavigationBar->setHaveNextResponder(false);
        
        CCRect titleRect;
        titleRect.origin = rect.size/2;
        titleRect.size.width = rect.size.width * 0.5f;
        titleRect.size.height = rect.size.height;
        CALabel* label = CALabel::createWithCenter(titleRect);
        label->setNumberOfLine(1);
        label->setFontSize(_px(32));
        label->setColor(CAColor_white);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setText(m_pController->getTitle());
        label->setTag(0xffff);
        m_pNavigationBar->addSubview(label);
        
        CCRect leftRect = rect;
        leftRect.size.width = leftRect.size.height * 0.9f;
        leftRect.size.height *= 0.8f;
        leftRect.origin.x = leftRect.size.width * 0.7f;
        leftRect.origin.y = rect.size.height / 2;
        CAButton* button = CAButton::createWithCenter(leftRect, CAButtonTypeCustom);
        button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
        button->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pNavigationBar->addSubview(button);
        button->addTarget(m_pController, CAControl_selector(DMBrowseController::onButton), CAControlEventTouchUpInSide);
        
        CAButton* button2 = CAButton::createWithFrame(CADipRect(_dip(rect.size.width) - 200, 12, 200, 72), CAButtonTypeCustom);
        button2->setTitleForState(CAControlStateAll, "操作提醒");
        button2->setTitleColorForState(CAControlStateNormal, CAColor_white);
        button2->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pNavigationBar->addSubview(button2);
        button2->addTarget(this, CAControl_selector(DMBrowseVerticalView::onOperationRemind), CAControlEventTouchUpSide);
    }
    
    //......SetUpBar
    {
        CCRect rect = m_pController->getView()->getBounds();
        rect.origin.y = rect.size.height;
        rect.size.height = _px(200);
        
        
        m_pSetupBar = CAView::createWithFrame(rect, ccc4(0, 0, 0, 204));
        this->insertSubview(m_pSetupBar, 1);
        m_pSetupBar->setHaveNextResponder(false);
        
        CCRect sliderRect;
        sliderRect.origin.x = rect.size.width * 0.32f;
        sliderRect.origin.y = _px(41);
        sliderRect.size.width = rect.size.width * 0.6f;
        sliderRect.size.height = _px(72);
        m_pSlider = CASlider::createWithCenter(sliderRect);
        m_pSlider->setThumbTintImage(CAImage::create(pagecontrolSliderIcon));
        m_pSetupBar->addSubview(m_pSlider);
        m_pSlider->addTarget(this, CAControl_selector(DMBrowseVerticalView::onSliderValueChanged));
        m_pSlider->addTargetForTouchUpSide(this, CAControl_selector(DMBrowseVerticalView::onSliderTouchUpSide));
        
        CCRect labelRect;
        labelRect.origin.x = rect.size.width * (0.64f + 0.18f);
        labelRect.origin.y = sliderRect.origin.y;
        labelRect.size.width = rect.size.width * 0.4f;
        labelRect.size.height = _px(82);
        m_pPageInfoLabel = CALabel::createWithCenter(labelRect);
        m_pPageInfoLabel->setColor(CAColor_white);
        m_pPageInfoLabel->setFontSize(_px(24));
        m_pPageInfoLabel->setTextAlignment(CATextAlignmentCenter);
        m_pPageInfoLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pSetupBar->addSubview(m_pPageInfoLabel);
        
        CAView* lineView = CAView::createWithFrame(CCRect(0, _px(82), rect.size.width, _px(2)), CAColor_gray);
        m_pSetupBar->addSubview(lineView);
        
        CCSize btnSize = CCSize((rect.size.width - _px(20)) / 4, _px(116));
        
        CCRect brightnessRect;
        brightnessRect.size = btnSize;
        brightnessRect.origin.x = 0;
        brightnessRect.origin.y = _px(84);
        m_pbrightnessButton = CAButton::createWithFrame(brightnessRect, CAButtonTypeCustom);
        m_pbrightnessButton->setImageForState(CAControlStateNormal, CAImage::create(brightnessButtonIcon));
        m_pbrightnessButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pbrightnessButton->setTitleForState(CAControlStateAll, brightnessButtonTitle);
        m_pbrightnessButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pbrightnessButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pSetupBar->addSubview(m_pbrightnessButton);
        m_pbrightnessButton->addTarget(this, CAControl_selector(DMBrowseVerticalView::onBrightnessView), CAControlEventTouchUpInSide);
        
        CCRect collectRect;
        collectRect.size = btnSize;
        collectRect.origin.x = btnSize.width;
        collectRect.origin.y = _px(84);
        m_pCollectButton = CAButton::createWithFrame(collectRect, CAButtonTypeCustom);
        
//        m_pCollectButton->setImageForState(CAControlStateNormal, CAImage::create(collectButtonNormalIcon));
//        m_pCollectButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
//        m_pCollectButton->setImageForState(CAControlStateSelected, CAImage::create(collectButtonSelectedIcon));
//        m_pCollectButton->setTitleForState(CAControlStateNormal, collectButtonNormalTitle);
//        m_pCollectButton->setTitleForState(CAControlStateSelected, collectButtonSelectedTitle);
//        m_pCollectButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
//        m_pCollectButton->setTitleColorForState(CAControlStateSelected, CAColor_white);
//        m_pCollectButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
//        m_pCollectButton->setAllowsSelected(true);
        
        m_pCollectButton->setImageForState(CAControlStateAll, CAImage::create(collectButtonNormalIcon));
        m_pCollectButton->setTitleForState(CAControlStateAll, collectButtonNormalTitle);
        m_pCollectButton->setTitleColorForState(CAControlStateAll, CAColor_white);
        m_pCollectButton->addTarget(m_pController, CAControl_selector(DMBrowseController::onCollectionClicked), CAControlTouchUpInSide);
        
        m_pSetupBar->addSubview(m_pCollectButton);
        
        CCRect directionRect;
        directionRect.size = btnSize;
        directionRect.origin.x = btnSize.width * 2;
        directionRect.origin.y = _px(84);
        m_pDirectionButton = CAButton::createWithFrame(directionRect, CAButtonTypeCustom);
        m_pDirectionButton->setImageForState(CAControlStateNormal, CAImage::create(directionButtonIcon));
        m_pDirectionButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pDirectionButton->setTitleForState(CAControlStateAll, directionButtonTitle[0]);
        m_pDirectionButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pDirectionButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pSetupBar->addSubview(m_pDirectionButton);
        m_pDirectionButton->addTarget(m_pController, CAControl_selector(DMBrowseController::setReversing), CAControlEventTouchUpInSide);
        
        CCRect aboutRect;
        aboutRect.size = btnSize;
        aboutRect.origin.x = btnSize.width * 3;
        aboutRect.origin.y = _px(84);
        m_pAboutButton = CAButton::createWithFrame(aboutRect, CAButtonTypeCustom);
        m_pAboutButton->setImageForState(CAControlStateAll, CAImage::create(aboutButtonIcon));
        m_pAboutButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pAboutButton->setTitleForState(CAControlStateNormal, aboutButtonTitle[1]);
        m_pAboutButton->setTitleForState(CAControlStateSelected, aboutButtonTitle[0]);
        m_pAboutButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pAboutButton->setTitleColorForState(CAControlStateSelected, CAColor_white);
        m_pAboutButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pAboutButton->setAllowsSelected(true);
        m_pAboutButton->setControlStateSelected();
        m_pSetupBar->addSubview(m_pAboutButton);
        m_pAboutButton->addTarget(this, CAControl_selector(DMBrowseVerticalView::onExchangedAboutModel), CAControlEventTouchUpInSide);
    }
    
    
    //......pageView
    {
        m_pPageView = CAPageView::createWithFrame(this->getBounds(), CAPageView::CAPageViewDirectionHorizontal);
        m_pPageView->setBackGroundColor(CAColor_black);
        m_pPageView->setScrollViewDelegate(this);
        m_pPageView->setPageViewDelegate(this);
        this->addSubview(m_pPageView);
        m_pPageView->setBackGroundColor(CAColor_clear);
        
        
        CCRect labelRect;
        labelRect.size = CADipSize(400, 36);
        labelRect.origin = ccpSub(m_pController->getView()->getBounds().size, labelRect.size);
        m_pBrowseInfoLabel = CALabel::createWithFrame(labelRect);
        m_pBrowseInfoLabel->setFontSize(_px(24));
        m_pBrowseInfoLabel->setTextAlignment(CATextAlignmentCenter);
        m_pBrowseInfoLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pBrowseInfoLabel->setColor(CAColor_white);
        this->addSubview(m_pBrowseInfoLabel);
        
        m_pBrowseInfoLabel->insertSubview(CAView::createWithFrame(m_pBrowseInfoLabel->getBounds(), ccc4(0, 0, 0, 204)), -1);
    }
    
    if (!m_pController->m_pBrowseImageViews.empty())
    {
        this->updatePageView();
    }
    
    return true;
}

void DMBrowseVerticalView::updatePageView()
{
    do
    {
        if (m_pPageView->getSubViewAtIndex(0) == NULL)
        {
            CAVector<CAView*> views;
            for (int i=0; i<3; i++)
            {
                DMBrowseVerticalImageView* imageView = new DMBrowseVerticalImageView(this->getBounds().size);
                imageView->init();
                views.pushBack(imageView);
                imageView->release();
                
            }
            m_pPageView->setViews(views);
            m_pPageView->setCurrPage(m_iLastPage, false);
        }
        
        int off_index = m_pPageView->getCurrPage() - m_iLastPage;
        int index = (m_pController->getCurrPage() + off_index);
        m_pController->setCurrPage(index);
        m_pController->detectingWhetherTurnThePage();
        m_pSlider->setValue(m_pController->getCurrPage()/(float)(m_pController->getCurrBrowseModel()->imageUrls.size()-1));
        
        int currPage = 1;
        
        if (m_pController->isTheEndOfThePage())
        {
            currPage = 2;
        }
        else if (m_pController->isTheTopOfThePage())
        {
            currPage = 0;
        }
        
        for (int i=0; i<3; i++)
        {
            DMBrowseVerticalImageView* imageView = (DMBrowseVerticalImageView*)m_pPageView->getSubViewAtIndex(i);
            imageView->setImageView(m_pController->m_pBrowseImageViews.at(i + ___SIZE___ - currPage));
            imageView->setLabel(m_pController->m_pPageLabels.at(i + ___SIZE___ - currPage));
        }
        
        m_pPageView->setCurrPage(currPage, false);
        m_iLastPage = currPage;
    }
    while (0);
    
    
    
    char str[16];
    sprintf(str, "%d/%d", m_pController->getCurrPage() + 1, (int)m_pController->getCurrBrowseModel()->imageUrls.size());
    
    if (m_pPageInfoLabel)
    {
        m_pPageInfoLabel->setText(str);
    }
    
    if (m_pBrowseInfoLabel)
    {
        m_pBrowseInfoLabel->setText(m_pController->getTitle() + " " + str + " " + getNetWorkUtils());
    }
}

void DMBrowseVerticalView::showBar()
{
    CC_RETURN_IF(m_bBarShow == true);
    m_bBarShow = true;
    m_pNavigationBar->stopAllActions();
    m_pSetupBar->stopAllActions();
    
    CCPoint navigationBarPoint = CCPointZero;
    m_pNavigationBar->runAction
    (
        CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, navigationBarPoint))
    );
    
    
    CCPoint setUpBarPoint = CCPoint(0, this->getBounds().size.height - m_pSetupBar->getFrame().size.height);
    m_pSetupBar->runAction
    (
        CCSequence::create(
                        CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                           callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                        CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, setUpBarPoint)),
                        CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                           callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                        NULL
                        )
     
    );
}

void DMBrowseVerticalView::hideBar()
{
    CC_RETURN_IF(m_bBarShow == false);
    m_bBarShow = false;
    m_pNavigationBar->stopAllActions();
    m_pSetupBar->stopAllActions();
    
    if (m_pBrightnessBg)
    {
        m_pBrightnessBg->setVisible(false);
    }
    
    
    CCPoint navigationBarPoint = CCPoint(0, -m_pNavigationBar->getFrame().size.height);
    m_pNavigationBar->runAction
    (
        CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, navigationBarPoint))
    );
    
    CCPoint setUpBarPoint = CCPoint(0, this->getBounds().size.height);
    m_pSetupBar->runAction
    (
        CCSequence::create(
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                              callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                           CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, setUpBarPoint)),
                           CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                              callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                           NULL
                           )
    
    );
    
}

void DMBrowseVerticalView::scrollViewWillBeginDragging(CAScrollView* view)
{
    this->hideBar();
}

void DMBrowseVerticalView::pageViewDidEndTurning(CAPageView* pageView)
{
    this->updatePageView();
}

void DMBrowseVerticalView::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point)
{
    CCSize size = this->getBounds().size;
    int number = 0;
    if (point.x < size.width/3.0f || (point.x < size.width/3.0f && point.y < size.height/7.0f))
    {
        number = m_bLeftModel ? number = 1 : number = -1;
    }
    else if (point.x > size.width* 2/3.0f || (point.x > size.width/3.0f && point.y > size.height* 6/7.0f))
    {
        number = m_bLeftModel ? number = -1 : number = 1;
    }

    if (number == 0)
    {
        if (m_bBarShow)
        {
            this->hideBar();
        }
        else
        {
            this->showBar();
        }
    }
    else
    {
        this->hideBar();
        m_pPageView->setCurrPage(m_pPageView->getCurrPage() + number, true, true);
    }
}

void DMBrowseVerticalView::onSliderValueChanged(CAControl* control, CCPoint point)
{
    if (control->isEqual(m_pSlider))
    {
        int size = (int)m_pController->getCurrBrowseModel()->imageUrls.size();
        size = MAX(size, 1);
        int index = m_pSlider->getValue() * (size - 1);
        
        char str[16];
        sprintf(str, "%d/%d", index + 1, size);
        
        if (m_pPageInfoLabel)
        {
            m_pPageInfoLabel->setText(str);
        }
    }
    
    if (control->isEqual(m_pBrightnessSlider))
    {
        CADevice::setScreenBrightness(m_pBrightnessSlider->getValue());
    }
}

void DMBrowseVerticalView::onSliderTouchUpSide(CAControl* control, CCPoint point)
{
    CC_RETURN_IF(m_pController->m_pCurrBrowseModel->imageUrls.empty());
    int index = m_pSlider->getValue() * (m_pController->getCurrBrowseModel()->imageUrls.size() - 1);
    m_pController->setCurrPage(index);
    m_pController->detectingWhetherTurnThePage();
    this->updatePageView();
}

void DMBrowseVerticalView::onOperationRemind(CAControl* control, CCPoint point)
{
    this->hideBar();
    CAButton* button = CAButton::createWithFrame(this->getBounds(), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create(operationRemindV_Icon));
    button->setBackGroundViewForState(CAControlStateNormal, imageView);
    this->insertSubview(button, 10);
    button->addTarget(button, CAControl_selector(CAButton::removeFromSuperview), CAControlEventTouchUpInSide);
    
    const char* title[3] = {"上\n一\n页","菜\n单","下\n一\n页"};
    
    if (m_bLeftModel)
    {
        title[0] = "下\n一\n页";
        title[2] = "上\n一\n页";
    }
    
    for (int i=0; i<3; i++)
    {
        CCRect rect = CCRect(this->getBounds().size.width * (1/6.0f + i/3.0f), this->getBounds().size.height/2, this->getBounds().size.width/6, this->getBounds().size.height/2);
        CALabel* label = CALabel::createWithCenter(rect);
        label->setFontSize(this->getBounds().size.width/10);
        label->setColor(CAColor_white);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setText(title[i]);
        imageView->addSubview(label);
    }
}

void DMBrowseVerticalView::onExchangedAboutModel(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CAButton* button = (CAButton*)control;
    m_bLeftModel = button->isSelected();
    this->onOperationRemind(control, point);
}

void DMBrowseVerticalView::onBrightnessView(CAControl* control, CCPoint point)
{
    if (m_pBrightnessBg == NULL)
    {
        m_pBrightnessBg = CAImageView::createWithFrame(CADipRect(40, _dip(this->getBounds().size.height) - 730, 75, 510));
        m_pBrightnessBg->setImage(CAImage::create(brightnessSliderBg));
        this->insertSubview(m_pBrightnessBg, 1);
        m_pBrightnessBg->setHaveNextResponder(false);
        m_pBrightnessBg->setVisible(false);
        
        CCRect rect;
        rect.origin = m_pBrightnessBg->getBounds().size/2;
        rect.size.width = _px(360);
        rect.size.height = _px(60);
        m_pBrightnessSlider = CASlider::createWithCenter(rect);
        m_pBrightnessSlider->setThumbTintImage(CAImage::create(pagecontrolSliderIcon));
        m_pBrightnessBg->addSubview(m_pBrightnessSlider);
        m_pBrightnessSlider->setRotation(270);
        m_pBrightnessSlider->setValue(CADevice::getScreenBrightness());
        m_pBrightnessSlider->addTarget(this, CAControl_selector(DMBrowseVerticalView::onSliderValueChanged));
    }
    
    m_pBrightnessBg->setVisible(!m_pBrightnessBg->isVisible());
}

void DMBrowseVerticalView::updateUI()
{
    DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
    bool collect = cMgr->isCollect(m_pController->m_sComicID);
    if (collect)
    {
        m_pCollectButton->setImageForState(CAControlStateAll, CAImage::create(collectButtonSelectedIcon));
        m_pCollectButton->setTitleForState(CAControlStateAll, collectButtonSelectedTitle);
        m_pCollectButton->setTitleColorForState(CAControlStateAll, CAColor_gray);
    }
    else
    {
        m_pCollectButton->setImageForState(CAControlStateAll, CAImage::create(collectButtonNormalIcon));
        m_pCollectButton->setTitleForState(CAControlStateAll, collectButtonNormalTitle);
        m_pCollectButton->setTitleColorForState(CAControlStateAll, CAColor_white);
    }
}

void DMBrowseVerticalView::updateTitle()
{
    CALabel* label = (CALabel*)m_pNavigationBar->getSubviewByTag(0xffff);
    label->setText(m_pController->getTitle());
}

#pragma DMBrowseVerticalImageView
static int ssss = 0;
DMBrowseVerticalImageView::DMBrowseVerticalImageView(const CADipSize& winSize)
: m_obWinSize(winSize)
, m_pScrollView(NULL)
, m_pImageView(NULL)
, m_pLabel(NULL)
{
    CCLog("DMBrowseVerticalImageView::%d",++ssss);
}

DMBrowseVerticalImageView::~DMBrowseVerticalImageView()
{
    CCLog("~DMBrowseVerticalImageView::%d",--ssss);
}

bool DMBrowseVerticalImageView::init()
{
    this->setContentSize(m_obWinSize);

    m_pScrollView = CAScrollView::createWithFrame(this->getBounds());
    m_pScrollView->setShowsHorizontalScrollIndicator(false);
    m_pScrollView->setShowsVerticalScrollIndicator(false);
    m_pScrollView->setBounceVertical(false);
    m_pScrollView->setMaximumZoomScale(4.0f);
    m_pScrollView->setMinimumZoomScale(1.0f);
    m_pScrollView->setBackGroundColor(CAColor_clear);
    m_pScrollView->setHaveNextResponder(true);
    this->insertSubview(m_pScrollView, 1);

    return true;
}

void DMBrowseVerticalImageView::setImageView(CommonUrlImageView* var)
{
    var->removeFromSuperview();
    m_pScrollView->removeSubview(m_pImageView);
    m_pImageView = var;
    m_pImageView->setCenter(CADipRect(m_obWinSize.width/2, m_obWinSize.height/2, m_obWinSize.width, m_obWinSize.height));
    m_pScrollView->insertSubview(m_pImageView, 1);
}

void DMBrowseVerticalImageView::setLabel(CALabel* label)
{
    this->removeSubview(m_pLabel);
    label->removeFromSuperview();
    m_pLabel = label;
    m_pLabel->setCenterOrigin(ccpAdd(m_obWinSize/2, CCPoint(0, _px(2))));
    m_pLabel->setScale(1.5f);
    this->addSubview(m_pLabel);
}

void DMBrowseVerticalImageView::reset()
{
    m_pScrollView->setZoomScale(1.0f);
    m_pScrollView->setViewSize(m_obWinSize);
    m_pScrollView->setContentOffset(CCPointZero, false);
}

void DMBrowseVerticalImageView::imageViewRequestFinished(CommonUrlImageView* imageView)
{
    CC_RETURN_IF(imageView->getSuperview() == NULL);
    CADipRect rect = imageView->getBounds();
    rect.origin = m_obWinSize/2;
    m_pScrollView->setCenter(rect);
    m_pScrollView->setViewSize(rect.size);
    imageView->setFrame(imageView->getBounds());
}



#pragma DMBrowseHorizontalView

DMBrowseHorizontalView::DMBrowseHorizontalView(DMBrowseController* controller)
: m_pController(controller), m_pNavigationBar(NULL)
, m_pSetupBar(NULL), m_pTableView(NULL)
, m_pSlider(NULL), m_bBarShow(false)
, m_pbrightnessButton(NULL), m_pBrowseInfoLabel(NULL)
, m_pDirectionButton(NULL), m_pCollectButton(NULL)
, m_pPageInfoLabel(NULL), m_bMoved(false)
, m_pBrightnessBg(NULL), m_pBrightnessSlider(NULL)
{
    UMENG_EventBeginValue("browse_type", "横屏");
}

DMBrowseHorizontalView::~DMBrowseHorizontalView()
{
    UMENG_EventEndValue("browse_type", "横屏");
}

bool DMBrowseHorizontalView::init()
{
    //......NavigationBar
    {
        CCRect rect = this->getBounds();
        rect.size.height = _px(88);
        rect.origin.y = -rect.size.height;
        
        m_pNavigationBar = CAView::createWithFrame(rect, ccc4(0, 0, 0, 204));
        this->insertSubview(m_pNavigationBar, 1);
        m_pNavigationBar->setHaveNextResponder(false);
        
        CCRect titleRect;
        titleRect.origin = rect.size/2;
        titleRect.size.width = rect.size.width * 0.5f;
        titleRect.size.height = rect.size.height;
        CALabel* label = CALabel::createWithCenter(titleRect);
        label->setNumberOfLine(1);
        label->setFontSize(_px(32));
        label->setColor(CAColor_white);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setText(m_pController->getTitle());
        label->setTag(0xffff);
        m_pNavigationBar->addSubview(label);
        
        CCRect leftRect = rect;
        leftRect.size.width = leftRect.size.height * 0.9f;
        leftRect.size.height *= 0.8f;
        leftRect.origin.x = leftRect.size.width * 0.7f;
        leftRect.origin.y = rect.size.height / 2;
        CAButton* button = CAButton::createWithCenter(leftRect, CAButtonTypeCustom);
        button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
        button->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pNavigationBar->addSubview(button);
        button->addTarget(m_pController, CAControl_selector(DMBrowseController::onButton), CAControlEventTouchUpSide);
        
        CAButton* button2 = CAButton::createWithFrame(CADipRect(_dip(rect.size.width) - 200, 12, 200, 72), CAButtonTypeCustom);
        button2->setTitleForState(CAControlStateAll, "操作提醒");
        button2->setTitleColorForState(CAControlStateNormal, CAColor_white);
        button2->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pNavigationBar->addSubview(button2);
        button2->addTarget(this, CAControl_selector(DMBrowseHorizontalView::onOperationRemind), CAControlEventTouchUpSide);
    }
    
    //......SetUpBar
    {
        CCRect rect = this->getBounds();
        rect.origin.y = rect.size.height;
        rect.size.height = _px(100);
        
        
        m_pSetupBar = CAView::createWithFrame(rect, ccc4(0, 0, 0, 204));
        this->insertSubview(m_pSetupBar, 1);
        m_pSetupBar->setHaveNextResponder(false);
        
        CCRect sliderRect;
        sliderRect.origin.x = rect.size.width * 0.046f;
        sliderRect.origin.y = _px(20);
        sliderRect.size.width = rect.size.width * 0.355f;
        sliderRect.size.height = _px(72);
        m_pSlider = CASlider::createWithFrame(sliderRect);
        m_pSlider->setThumbTintImage(CAImage::create(pagecontrolSliderIcon));
        m_pSetupBar->addSubview(m_pSlider);
        m_pSlider->addTarget(this, CAControl_selector(DMBrowseHorizontalView::onSliderValueChanged));
        m_pSlider->addTargetForTouchUpSide(this, CAControl_selector(DMBrowseHorizontalView::onSliderTouchUpSide));
        
        
        CCRect labelRect;
        labelRect.origin.x = rect.size.width * 0.447f;
        labelRect.origin.y = 0;
        labelRect.size.width = rect.size.width * 0.13f;
        labelRect.size.height = _px(100);
        m_pPageInfoLabel = CALabel::createWithFrame(labelRect);
        m_pPageInfoLabel->setColor(CAColor_white);
        m_pPageInfoLabel->setFontSize(_px(24));
        m_pPageInfoLabel->setTextAlignment(CATextAlignmentCenter);
        m_pPageInfoLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pSetupBar->addSubview(m_pPageInfoLabel);



        CCSize btnSize = CCSize(rect.size.width * 0.458f / 3.0f, _px(100));
        
        CCRect brightnessRect;
        brightnessRect.size = btnSize;
        brightnessRect.origin.x = rect.size.width - btnSize.width * 3;
        brightnessRect.origin.y = 0;
        m_pbrightnessButton = CAButton::createWithFrame(brightnessRect, CAButtonTypeCustom);
        m_pbrightnessButton->setImageForState(CAControlStateNormal, CAImage::create(brightnessButtonIcon));
        m_pbrightnessButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pbrightnessButton->setTitleForState(CAControlStateAll, brightnessButtonTitle);
        m_pbrightnessButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pbrightnessButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pSetupBar->addSubview(m_pbrightnessButton);
        m_pbrightnessButton->addTarget(this, CAControl_selector(DMBrowseHorizontalView::onBrightnessView), CAControlEventTouchUpInSide);
        
        CCRect collectRect;
        collectRect.size = btnSize;
        collectRect.origin.x = rect.size.width - btnSize.width * 2;
        collectRect.origin.y = 0;
        m_pCollectButton = CAButton::createWithFrame(collectRect, CAButtonTypeCustom);
//        m_pCollectButton->setImageForState(CAControlStateNormal, CAImage::create(collectButtonNormalIcon));
//        m_pCollectButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
//        m_pCollectButton->setImageForState(CAControlStateSelected, CAImage::create(collectButtonSelectedIcon));
//        m_pCollectButton->setTitleForState(CAControlStateNormal, collectButtonNormalTitle);
//        m_pCollectButton->setTitleForState(CAControlStateSelected, collectButtonSelectedTitle);
//        m_pCollectButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
//        m_pCollectButton->setTitleColorForState(CAControlStateSelected, CAColor_white);
//        m_pCollectButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
//        m_pCollectButton->setAllowsSelected(true);
        
        m_pCollectButton->setImageForState(CAControlStateAll, CAImage::create(collectButtonNormalIcon));
        m_pCollectButton->setTitleForState(CAControlStateAll, collectButtonNormalTitle);
        m_pCollectButton->setTitleColorForState(CAControlStateAll, CAColor_white);
        
        m_pCollectButton->addTarget(m_pController, CAControl_selector(DMBrowseController::onCollectionClicked), CAControlTouchUpInSide);
        
        m_pSetupBar->addSubview(m_pCollectButton);
        
        CCRect directionRect;
        directionRect.size = btnSize;
        directionRect.origin.x = rect.size.width - btnSize.width;
        directionRect.origin.y = 0;
        m_pDirectionButton = CAButton::createWithFrame(directionRect, CAButtonTypeCustom);
        m_pDirectionButton->setImageForState(CAControlStateNormal, CAImage::create(directionButtonIcon));
        m_pDirectionButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pDirectionButton->setTitleForState(CAControlStateAll, directionButtonTitle[1]);
        m_pDirectionButton->setTitleColorForState(CAControlStateNormal, CAColor_white);
        m_pDirectionButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        m_pSetupBar->addSubview(m_pDirectionButton);
        m_pDirectionButton->addTarget(m_pController, CAControl_selector(DMBrowseController::setReversing), CAControlEventTouchUpInSide);

    }
    
    {

        CCRect tableRect = this->getBounds();

        m_pTableView = CATableView::createWithFrame(tableRect);
        m_pTableView->setTableViewDataSource(this);
        m_pTableView->setTableViewDelegate(this);
        m_pTableView->setScrollViewDelegate(this);
        m_pTableView->setSeparatorViewHeight(_px(20));
        m_pTableView->setSeparatorColor(CAColor_clear);
        m_pTableView->setAllowsSelection(true);
        m_pTableView->setBackGroundColor(CAColor_clear);
        m_pTableView->setShowsVerticalScrollIndicator(false);
        this->addSubview(m_pTableView);
        m_pTableView->setHaveNextResponder(true);

        CCRect labelRect;
        labelRect.size = CADipSize(400, 36);
        labelRect.origin = ccpSub(this->getBounds().size, labelRect.size);
        m_pBrowseInfoLabel = CALabel::createWithFrame(labelRect);
        m_pBrowseInfoLabel->setFontSize(_px(24));
        m_pBrowseInfoLabel->setTextAlignment(CATextAlignmentCenter);
        m_pBrowseInfoLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pBrowseInfoLabel->setColor(CAColor_white);
        this->addSubview(m_pBrowseInfoLabel);
        
        m_pBrowseInfoLabel->insertSubview(CAView::createWithFrame(m_pBrowseInfoLabel->getBounds(), ccc4(0, 0, 0, 204)), -1);
    }
    return true;
}

void DMBrowseHorizontalView::showBar()
{
    CC_RETURN_IF(m_bBarShow == true);
    m_bBarShow = true;
    m_pNavigationBar->stopAllActions();
    m_pSetupBar->stopAllActions();
    
    CCPoint navigationBarPoint = CCPointZero;
    m_pNavigationBar->runAction
    (
     CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, navigationBarPoint))
     );
    
    
    CCPoint setUpBarPoint = CCPoint(0, this->getBounds().size.height - m_pSetupBar->getFrame().size.height);
    m_pSetupBar->runAction
    (
     CCSequence::create(
                        CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                           callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                        CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, setUpBarPoint)),
                        CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                           callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                        NULL
                        )
     
     );
}

void DMBrowseHorizontalView::hideBar()
{
    CC_RETURN_IF(m_bBarShow == false);
    m_bBarShow = false;
    m_pNavigationBar->stopAllActions();
    m_pSetupBar->stopAllActions();
    
    if (m_pBrightnessBg)
    {
        m_pBrightnessBg->setVisible(false);
    }
    
    CCPoint navigationBarPoint = CCPoint(0, -m_pNavigationBar->getFrame().size.height);
    m_pNavigationBar->runAction
    (
     CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, navigationBarPoint))
     );
    
    
    CCPoint setUpBarPoint = CCPoint(0, this->getBounds().size.height);
    m_pSetupBar->runAction
    (
     CCSequence::create(
                        CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                           callfunc_selector(CATouchDispatcher::setDispatchEventsFalse)),
                        CCEaseSineOut::create(CCFrameOrginTo::create(0.2f, setUpBarPoint)),
                        CCCallFunc::create(CAApplication::getApplication()->getTouchDispatcher(),
                                           callfunc_selector(CATouchDispatcher::setDispatchEventsTrue)),
                        NULL
                        )
     
     );
    
}

void DMBrowseHorizontalView::updateTableView()
{
    m_pTableView->reloadData();
    m_pTableView->setContentOffset(CCPoint(0, this->getNewTableViewContentOffset()), false);
    
    this->updateReadInfo();
}

void DMBrowseHorizontalView::updateOffset(int row)
{
    CC_RETURN_IF(m_pController->isViewRunning() == false);
    if (row < m_pController->getCurrPage())
    {
        float offsetY = m_pTableView->getContentOffset().y;
        offsetY -= this->getOldTableViewContentOffset();
        offsetY += this->getNewTableViewContentOffset();
        m_pTableView->reloadData();
        m_pTableView->setContentOffset(CCPoint(0, offsetY), false);
    }
    else
    {
        m_pTableView->reloadData();
    }
    
    this->updateReadInfo();
}

void DMBrowseHorizontalView::updateReadInfo()
{
    int currPage = m_pController->getCurrPage();
    int size = (int)m_pController->getCurrBrowseModel()->imageUrls.size();
    size = MAX(size, 1);
    CC_RETURN_IF(currPage < 0);
    CC_RETURN_IF(currPage >= size);
    
    char str[16];
    sprintf(str, "%d/%d", currPage + 1, size);
    
    if (m_pPageInfoLabel)
    {
        m_pPageInfoLabel->setText(str);
    }
    
    if (m_pBrowseInfoLabel)
    {
        m_pBrowseInfoLabel->setText(m_pController->getTitle() + " " + str + " " + getNetWorkUtils());
    }
    
    m_pSlider->setValue(currPage/(float)(size - 1));
}

float DMBrowseHorizontalView::getNewTableViewContentOffset()
{
    return this->getNewTableViewContentOffsetAtIndex(m_pController->getCurrPage());
}

float DMBrowseHorizontalView::getNewTableViewContentOffsetAtIndex(int index)
{
    int y = 0;
    if (index < 0)
    {
        for (int i=0; i<m_pController->getPrevBrowseModel()->imageUrls.size() + index; i++)
        {
            y += m_pController->getTableViewCellHeight(0, i) + _px(20);
        }
    }
    else if (index >= 0 && index < m_pController->getCurrBrowseModel()->imageUrls.size())
    {
        y += m_pController->getTableViewSectionHeight(0);
        for (int i=0; i<index; i++)
        {
            y += m_pController->getTableViewCellHeight(1, i) + _px(20);
        }
    }
    else
    {
        y += m_pController->getTableViewSectionHeight(0);
        y += m_pController->getTableViewSectionHeight(1);
        for (int i=0; i<index - m_pController->getCurrBrowseModel()->imageUrls.size(); i++)
        {
            y += m_pController->getTableViewCellHeight(2, i) + _px(20);
        }
    }
    return y;
}

float DMBrowseHorizontalView::getOldTableViewContentOffset()
{
    return this->getOldTableViewContentOffsetAtIndex(m_pController->getCurrPage());
}

float DMBrowseHorizontalView::getOldTableViewContentOffsetAtIndex(int index)
{
    int y = 0;
    if (index < 0)
    {
        for (int i=0; i<m_pTableView->getNumberOfRowsInSection(0) + index; i++)
        {
            y += m_pTableView->getRowHeightInSectionInRow(0, i) + _px(20);
        }
    }
    else if (index >= 0 && index < m_pTableView->getNumberOfRowsInSection(1))
    {
        y += m_pTableView->getSectionHeightInSection(0);
        for (int i=0; i<index; i++)
        {
            y += m_pTableView->getRowHeightInSectionInRow(1, i) + _px(20);
        }
    }
    else
    {
        y += m_pTableView->getSectionHeightInSection(0);
        y += m_pTableView->getSectionHeightInSection(1);
        for (int i=0; i<index - m_pTableView->getNumberOfRowsInSection(1); i++)
        {
            y += m_pTableView->getRowHeightInSectionInRow(2, i) + _px(20);
        }
    }
    return y;
}

int DMBrowseHorizontalView::getIndexAtTableViewCurrContentOffset()
{
    float offset = m_pTableView->getContentOffset().y + this->getBounds().size.height / 2;
    int index = 0;
    float indexOffset = 0;
    float offsetDefault1 = m_pTableView->getSectionHeightInSection(0);
    float offsetDefault2 = offsetDefault1 + m_pTableView->getSectionHeightInSection(1);
    if (offset < offsetDefault1)
    {
        while (index < m_pTableView->getNumberOfRowsInSection(0))
        {
            indexOffset += m_pTableView->getRowHeightInSectionInRow(0, index) + _px(20);
            CC_BREAK_IF((offset - indexOffset) < FLT_EPSILON);
            index++;
        }
        index -= m_pTableView->getNumberOfRowsInSection(0);
    }
    else if (offset >= offsetDefault1 && offset < offsetDefault2)
    {
        indexOffset += offsetDefault1;
        while (index < m_pTableView->getNumberOfRowsInSection(1))
        {
            indexOffset += m_pTableView->getRowHeightInSectionInRow(1, index) + _px(20);
            CC_BREAK_IF((offset - indexOffset) < FLT_EPSILON);
            index++;
        }
    }
    else
    {
        indexOffset += offsetDefault2;
        while (index < m_pTableView->getNumberOfRowsInSection(2))
        {
            indexOffset += m_pTableView->getRowHeightInSectionInRow(2, index) + _px(20);
            CC_BREAK_IF((offset - indexOffset) < FLT_EPSILON);
            index++;
        }
        index += m_pTableView->getNumberOfRowsInSection(1);
    }
    return index;
}

CATableViewCell* DMBrowseHorizontalView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("cell");
    if (cell == NULL)
    {
        cell = CATableViewCell::create("cell");
        cell->setBounds(CCRect(0, 0, cellSize.width, cellSize.height));
        cell->setControlStateEffect(false);
        cell->setBackgroundView(NULL);
    }
    
    int n = row;

    if (section == 0)
    {
        n -= m_pTableView->getNumberOfRowsInSection(0);
    }
    if (section == 2)
    {
        n += m_pTableView->getNumberOfRowsInSection(1);
    }

    n = n - m_pController->getCurrPage() + 4;

    if (n >= 0 && n <= 8)
    {
        if (!m_pController->m_pBrowseImageViews.empty())
        {
            CommonUrlImageView* imageView = (CommonUrlImageView*)m_pController->m_pBrowseImageViews.at(n);
            imageView->removeFromSuperview();
            cell->setBackgroundView(imageView);
        }
        if (!m_pController->m_pPageLabels.empty())
        {
            CALabel* label = m_pController->m_pPageLabels.at(n);
            label->removeFromSuperview();
            label->setScale(1.5f);
            label->setCenterOrigin(cellSize/2);
            cell->getBackgroundView()->insertSubview(label, -1);
        }
    }
    
    
    return cell;
}

unsigned int DMBrowseHorizontalView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    unsigned int count = 0;
    switch (section)
    {
        case 0:
            count = m_pController->getPrevBrowseModel()->imageUrls.size();
            break;
        case 1:
            count = m_pController->getCurrBrowseModel()->imageUrls.size();
            break;
        case 2:
            count = m_pController->getNextBrowseModel()->imageUrls.size();
            break;
        default:
            break;
    }
    return count;
}

unsigned int DMBrowseHorizontalView::numberOfSections(CATableView *table)
{
    return 3;
}

unsigned int DMBrowseHorizontalView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return m_pController->getTableViewCellHeight(section, row);
}

void DMBrowseHorizontalView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void DMBrowseHorizontalView::scrollViewWillBeginDragging(CAScrollView* view)
{
    this->hideBar();
}

void DMBrowseHorizontalView::scrollViewDidMoved(CrossApp::CAScrollView* view)
{
    int index = this->getIndexAtTableViewCurrContentOffset();
    CC_RETURN_IF(m_pController->getCurrPage() == index);
    m_pController->setCurrPage(index);
    
    int relust = m_pController->detectingWhetherTurnThePage();
    float offset = 0;
    if (relust > 0)
    {
        offset = m_pTableView->getContentOffset().y - m_pTableView->getSectionHeightInSection(0);
    }
    else if (relust < 0)
    {
        offset = m_pTableView->getContentOffset().y + m_pController->getTableViewSectionHeight(0);
    }
    
    if (relust != 0)
    {
        m_pTableView->reloadData();
        m_pTableView->setContentOffset(CCPoint(0, offset), false);
    }
    
    this->updateReadInfo();
}

void DMBrowseHorizontalView::onSliderValueChanged(CAControl* control, CCPoint point)
{
    if (control->isEqual(m_pSlider))
    {
        int size = (int)m_pController->getCurrBrowseModel()->imageUrls.size();
        size = MAX(size, 1);
        int index = m_pSlider->getValue() * (size - 1);

        char str[16];
        sprintf(str, "%d/%d", index + 1, size);
        
        if (m_pPageInfoLabel)
        {
            m_pPageInfoLabel->setText(str);
        }
    }
    
    if (control->isEqual(m_pBrightnessSlider))
    {
        CADevice::setScreenBrightness(m_pBrightnessSlider->getValue());
    }
}

void DMBrowseHorizontalView::onSliderTouchUpSide(CAControl* control, CCPoint point)
{
    int index = m_pSlider->getValue() * (m_pController->getCurrBrowseModel()->imageUrls.size() - 1);
    m_pController->setCurrPage(index);
    
    char str[16];
    sprintf(str, "%d/%d", index + 1, (int)m_pController->getCurrBrowseModel()->imageUrls.size());
    if (m_pBrowseInfoLabel)
    {
        m_pBrowseInfoLabel->setText(m_pController->getTitle() + " " + str + " " + "WIFI");
    }
    
    float offsetY = this->getNewTableViewContentOffset();
    m_pTableView->setContentOffset(CCPoint(0, offsetY), false);
    m_pTableView->reloadData();
}

void DMBrowseHorizontalView::onOperationRemind(CAControl* control, CCPoint point)
{
    this->hideBar();
    CAButton* button = CAButton::createWithFrame(this->getBounds(), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create(operationRemindH_Icon));
    button->setBackGroundViewForState(CAControlStateNormal, imageView);
    this->insertSubview(button, 10);
    button->addTarget(button, CAControl_selector(CAButton::removeFromSuperview), CAControlEventTouchUpInSide);
    
    const char* title[4] = {"上一页","下一页","菜单","下一页"};
    CCSize size = this->getBounds().size;
    CCRect rect[4] =
    {
        CCRect(size.width/2, size.height * 9/64.0f, size.width/3, size.height * 9/32.0f),
        CCRect(size.width/6.0f, this->getBounds().size.height * 41/64.0f, size.width/3, size.height * 9/32.0f),
        CCRect(size.width/2.0f, this->getBounds().size.height * 41/64.0f, size.width/3, size.height * 9/32.0f),
        CCRect(size.width* 5/6.0f, this->getBounds().size.height * 41/64.0f, size.width/3, size.height * 9/32.0f)
    };
    
    for (int i=0; i<4; i++)
    {
        CALabel* label = CALabel::createWithCenter(rect[i]);
        label->setFontSize(this->getBounds().size.height/10);
        label->setColor(CAColor_white);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setText(title[i]);
        imageView->addSubview(label);
    }
}

void DMBrowseHorizontalView::onBrightnessView(CAControl* control, CCPoint point)
{
    if (m_pBrightnessBg == NULL)
    {
        m_pBrightnessBg = CAImageView::createWithFrame(CADipRect(_dip(this->getBounds().size.width) - 300, _dip(this->getBounds().size.height) - 400, 75, 510));
        m_pBrightnessBg->setRotation(90);
        m_pBrightnessBg->setImage(CAImage::create(brightnessSliderBg));
        this->insertSubview(m_pBrightnessBg, 1);
        m_pBrightnessBg->setHaveNextResponder(false);
        m_pBrightnessBg->setVisible(false);
        
        CCRect rect;
        rect.origin = m_pBrightnessBg->getBounds().size/2;
        rect.size.width = _px(360);
        rect.size.height = _px(60);
        m_pBrightnessSlider = CASlider::createWithCenter(rect);
        m_pBrightnessSlider->setThumbTintImage(CAImage::create(pagecontrolSliderIcon));
        m_pBrightnessBg->addSubview(m_pBrightnessSlider);
        m_pBrightnessSlider->setRotation(270);
        m_pBrightnessSlider->setValue(CADevice::getScreenBrightness());
        m_pBrightnessSlider->addTarget(this, CAControl_selector(DMBrowseHorizontalView::onSliderValueChanged));
    }
    
    m_pBrightnessBg->setVisible(!m_pBrightnessBg->isVisible());
}

bool DMBrowseHorizontalView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    m_bMoved = false;
    return true;
}

void DMBrowseHorizontalView::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
    m_bMoved = true;
}

void DMBrowseHorizontalView::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CC_RETURN_IF(m_bMoved);
    CCSize size = this->getBounds().size;
    CCPoint point = this->convertTouchToNodeSpace(pTouch);
    int number = 0;
    if (point.y < size.height * 9/32.0f)
    {
        number = -1;
    }
    else if (point.x < size.width/3.0f || point.x >= size.width* 2/3.0f)
    {
        number = 1;
    }
    
    if (number == 0)
    {
        if (m_bBarShow)
        {
            this->hideBar();
        }
        else
        {
            this->showBar();
        }
    }
    else
    {
        this->hideBar();
        CCPoint p = m_pTableView->getContentOffset();
        p.y += size.height * 0.8f * number;
        p.y = MAX(p.y, 0);
        p.y = MIN(p.y, m_pTableView->getViewSize().height - m_pTableView->getBounds().size.height);
        m_pTableView->setContentOffset(p, true);
    }
}

void DMBrowseHorizontalView::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    
}

void DMBrowseHorizontalView::updateUI()
{
    DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
    bool collect = cMgr->isCollect(m_pController->m_sComicID);
    if (collect)
    {
        m_pCollectButton->setImageForState(CAControlStateAll, CAImage::create(collectButtonSelectedIcon));
        m_pCollectButton->setTitleForState(CAControlStateAll, collectButtonSelectedTitle);
        m_pCollectButton->setTitleColorForState(CAControlStateAll, CAColor_gray);
    }
    else
    {
        m_pCollectButton->setImageForState(CAControlStateAll, CAImage::create(collectButtonNormalIcon));
        m_pCollectButton->setTitleForState(CAControlStateAll, collectButtonNormalTitle);
        m_pCollectButton->setTitleColorForState(CAControlStateAll, CAColor_white);
    }
}

void DMBrowseHorizontalView::updateTitle()
{
    CALabel* label = (CALabel*)m_pNavigationBar->getSubviewByTag(0xffff);
    label->setText(m_pController->getTitle());
}














