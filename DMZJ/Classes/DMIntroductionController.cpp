

#include "DMIntroductionController.h"
#include "DMBrowseController.h"
#include "DMIntroductionInfoManager.h"
#include "DMSelectionDownloadController.h"
#include "DMColllectionManager.h"
#include "DMUserManager.h"
#include "DMSearchController.h"

static DMCommentModel converse(const CSJson::Value &data, DMCommentModel &model)
{
    model._id = data["id"].asString();
    model.uid = data["uid"].asString();
    model.nickname = data["nickname"].asString();
    model.avatar_url = data["avatar_url"].asString();
    model.pid = data["pid"].asString();
    model.comic_id = data["comic_id"].asString();
    model.author_id = data["author_id"].asString();
    model.author = data["author"].asString();
    model.content = data["content"].asString();
    model.createtime = data["createtime"].asString();
    model.isused = data["isused"].asString();
    model.replycount = 0;
    
    if (data.isMember("reply") && data["reply"]["data"].isArray() &&data["reply"]["data"].size()>0)
    {
        if (data["reply"].isMember("total")) {
            model.replycount = data["reply"]["total"].asInt();
        }
        vector<DMCommentModel> reply;
        for (int i=0; i< data["reply"]["data"].size(); i++)
        {
            DMCommentModel smodel;
            const CSJson::Value &v =  data["reply"]["data"][i];
            converse(v, smodel);
            reply.push_back(smodel);
        }
        model.reply = reply;
    }
    return model;
}

DMIntroductionController::DMIntroductionController(const std::string& ID, const std::string& title)
: m_sID(ID), m_pCollectionView(NULL), m_pTabBarView(NULL), m_pHeaderView(NULL), __scale(1), m_eType(DMIntroductionItem1)
, m_uItemsRowCount(0), m_pLoading(NULL),m_pCommentInputer(NULL),m_pCommentFiled(NULL), _modelCacheForSubmit(dMCommentModelDefault), isJoinDownload(false)
{
    this->setTitle(title);
    __scale = CAApplication::getApplication()->getWinSize().width / 640.0f;
    __scale = MIN(__scale, 1.8);
    float width = _dip(CAApplication::getApplication()->getWinSize().width);
    
    m_obItemInterval[0] = CADipSize(20 , 20);
    m_obItemInterval[1] = CADipSize(20 , 20);
    m_obItemInterval[2] = CADipSize(30 , 30);
    m_uiItems[0] = MIN(8, (width - 20) / 150);
    m_uiItems[1] = 1;
    m_uiItems[2] = MIN(7, (width - 30) / 150);
    
    m_bReverse[0] = 1;
    m_bReverse[1] = 1;
}

DMIntroductionController::~DMIntroductionController()
{
    CC_SAFE_RELEASE(m_pTabBarView);
    CC_SAFE_RELEASE(m_pHeaderView);
    m_obListItemInfos.clear();
    m_obAloneItemInfos.clear();
    m_obSimilarItemInfos.clear();
    m_pItemInfos.clear();
    m_pViewCells.clear();
}

DMIntroductionController* DMIntroductionController::create(const std::string &ID, const std::string& title)
{
    DMIntroductionController* controller = new DMIntroductionController(ID, title);
    if (controller && controller->init())
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return NULL;
}

void DMIntroductionController::viewDidLoad()
{
    CADipRect rect = this->getView()->getBounds();
    rect.size.height = 96;
    
    CAView* bar = CAView::createWithFrame(rect, ccc4(87, 154, 244, 255));
    this->getView()->addSubview(bar);
    
    CADipRect titleRect;
    titleRect.origin = rect.size/2;
    titleRect.size.width = rect.size.width * 0.5f;
    titleRect.size.height = rect.size.height;
    CALabel* label = CALabel::createWithCenter(titleRect);
    label->setNumberOfLine(1);
    label->setFontSize(_px(32));
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setText(m_sTitle);
    bar->addSubview(label);
    
    CADipRect leftRect = rect;
    leftRect.size.width = leftRect.size.height * 0.9f;
    leftRect.size.height *= 0.8f;
    leftRect.origin.x = leftRect.size.width * 0.7f;
    leftRect.origin.y = rect.size.height / 2;
    CAButton* button = CAButton::createWithCenter(leftRect, CAButtonTypeCustom);
    button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
    button->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
    bar->addSubview(button);
    button->addTarget(this, CAControl_selector(DMIntroductionController::onReturn), CAControlEventTouchUpInSide);
    
    if (!isJoinDownload)
    {
        CAButton* downLoadButton = CAButton::createWithFrame(CADipRect(rect.size.width - 96 - 40, 0, 120, 96), CAButtonTypeCustom);
        downLoadButton->setImageForState(CAControlStateNormal, CAImage::create(dowloadButtonMinIcon));
        downLoadButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        bar->addSubview(downLoadButton);
        downLoadButton->addTarget(this, CAControl_selector(DMIntroductionController::onDownLoad), CAControlEventTouchUpInSide);
    }
    
    m_pHeaderView = new DMIntroductionHeaderView(this);
    m_pHeaderView->init();

    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeBookDetail, m_sID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMIntroductionController::onRequestFinished));

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
        m_pLoading->setLoadingMinTime(0.6f);
        m_pLoading->setColor(CAColor_white);
        this->getView()->insertSubview(m_pLoading, CAWindowZoderTop);
        m_pLoading->setTargetOnCancel(this, callfunc_selector(DMIntroductionController::initWithCollectionView));
    }
    
    std::vector<unsigned long> ids = CADownloadManager::getInstance()->getDownloadIdsFromTextTag(m_sID);
    for (std::vector<unsigned long>::iterator itr=ids.begin(); itr!=ids.end(); itr++)
    {
        CC_CONTINUE_IF(!CADownloadManager::getInstance()->isFinished(*itr));
        const std::string& url = CADownloadManager::getInstance()->getDownloadUrl(*itr);
        std::string id = url.substr(0, url.find_last_of('.'));
        int pos = url.find_last_of('/') + 1;
        id = id.substr(pos, id.length() - pos);
        m_sHaveDownloadInfoIDs.insert(id);
    }
    
    CC_RETURN_IF(isJoinDownload);
    
    //初始化回复输入框
    m_pCommentInputer = CAView::createWithFrame(CCRect(0, getView()->getFrame().size.height - _px(88), getView()->getFrame().size.width, _px(88)));
    m_pCommentInputer->setColor(ccc4(239, 239, 239, 255));
    this->getView()->insertSubview(m_pCommentInputer, 10);
    
    m_pCommentFiled = CATextField::createWithFrame(CCRect(_px(30), _px(14), getView()->getFrame().size.width-_px(60), _px(60)));
    m_pCommentFiled->setPlaceHolder("评论");
    m_pCommentFiled->setFontSize(_px(28));
    m_pCommentFiled->setSpaceHolderColor(ccc4(153, 153, 153, 255));
    m_pCommentFiled->setHoriMargins(_px(8));
    
    CAScale9ImageView *scaleImageView = CAScale9ImageView::create();
    scaleImageView->setFrame(CCRect(0, 0, m_pCommentFiled->getFrame().size.width,m_pCommentFiled->getFrame().size.height));
    scaleImageView->setImage(CAImage::create("dm_resource/Introduction/dm_comment_bac.png"));
    m_pCommentFiled->setBackgroundView(scaleImageView);
    m_pCommentInputer->addSubview(m_pCommentFiled);
    m_pCommentFiled->setDelegate(this);
    m_pCommentFiled->setKeyboardReturnType(KEY_BOARD_RETURN_SEND);
    
    m_pCommentInputer->setVisible(false);

    UMENG_EventValue("introduction", m_sTitle.c_str());

}

void DMIntroductionController::initWithCollectionView()
{
    CC_RETURN_IF(isJoinDownload);
    CCRect collectionViewRect = this->getView()->getBounds();
    collectionViewRect.origin.y = _px(96);
    collectionViewRect.size.height -= collectionViewRect.origin.y;
    m_pCollectionView = CACollectionView::createWithFrame(collectionViewRect);
    m_pCollectionView->setCollectionViewDataSource(this);
    m_pCollectionView->setCollectionViewDelegate(this);
    this->getView()->addSubview(m_pCollectionView);
    m_pCollectionView->setHoriInterval(m_obItemInterval[0].width);
    m_pCollectionView->setVertInterval(m_obItemInterval[0].height);
    m_pCollectionView->setAllowsSelection(true);
    
    m_pCollectionView->setCollectionHeaderHeight(m_pHeaderView->getFrame().size.height);
    m_pCollectionView->setCollectionHeaderView(m_pHeaderView);
    
    
    //添加上下拉刷新
    m_pCollectionView->setScrollViewDelegate(this);
    CAPullToRefreshView* headerView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    m_pCollectionView->setHeaderRefreshView(headerView);
    m_pFooterView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    m_pFooterView->retain();
    
    _iCollectionHeight = m_pCollectionView->getFrame().size.height;
    
}

void DMIntroductionController::viewDidUnload()
{

}

void DMIntroductionController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CC_RETURN_IF(json["result"].asInt() != 1);
        const CSJson::Value& headerData = json["data"]["info"];
        DMIntroductionInfo headerInfo;
        headerInfo.ID = headerData["id"].asString();
        headerInfo.title = headerData["title"].asString();
        headerInfo.subtitle = headerData["subtitle"].asString();
        headerInfo.types = headerData["types"].asString();
        headerInfo.zone = headerData["zone"].asString();
        headerInfo.status = headerData["status"].asString();
        headerInfo.last_update_chapter_name = headerData["last_update_chapter_name"].asString();
        headerInfo.last_updatetime = headerData["last_updatetime"].asString();
        headerInfo.cover = headerData["cover"].asString();
        headerInfo.authors = headerData["authors"].asString();
        headerInfo.description = headerData["description"].asString();
        headerInfo.first_letter = headerData["first_letter"].asString();
        headerInfo.direction = headerData["direction"].asString();
        m_pHeaderView->setIntroductionInfo(headerInfo);
        
        
        //保存浏览记录
        DMIntroductionInfoManager* dIIMgr = DMIntroductionInfoManager::sharedDMIntroductionInfoManager();
        DMBrowseInfo browInfo;
        browInfo.ID = headerInfo.ID;
        browInfo.title = headerInfo.title;
        browInfo.subtitle = headerInfo.subtitle;
        browInfo.types = headerInfo.types;
        browInfo.zone = headerInfo.zone;
        browInfo.status = headerInfo.status;
        browInfo.last_update_chapter_name = headerInfo.last_update_chapter_name;
        browInfo.last_updatetime = headerInfo.last_updatetime;
        browInfo.cover = headerInfo.cover;
        browInfo.authors = headerInfo.authors;
        browInfo.description = headerInfo.description;
        browInfo.first_letter = headerInfo.first_letter;
        browInfo.direction = headerInfo.direction;
        browInfo.time = getCurrentTime();
        dIIMgr->save(browInfo);
        
        m_obListItemInfos.clear();
        const CSJson::Value& listData = json["data"]["list"];
        for (int i=0; i<listData.size(); i++)
        {
            DMIntroductionItemInfo info;
            info.ID = listData[i]["id"].asString();
            info.comic_id = listData[i]["comic_id"].asString();
            info.chapter_name = listData[i]["chapter_name"].asString();
            info.chapter_order = listData[i]["chapter_order"].asString();
            info.filesize = listData[i]["filesize"].asString();
            m_obListItemInfos.push_back(info);
        }
        
        m_obAloneItemInfos.clear();
        const CSJson::Value& aloneData = json["data"]["alone"];
        if (aloneData.isArray())
        {
            for (int i=0; i<aloneData.size(); i++)
            {
                DMIntroductionItemInfo info;
                info.ID = aloneData[i]["id"].asString();
                info.comic_id = aloneData[i]["comic_id"].asString();
                info.chapter_name = aloneData[i]["chapter_name"].asString();
                info.chapter_order = aloneData[i]["chapter_order"].asString();
                info.filesize = aloneData[i]["filesize"].asString();
                m_obAloneItemInfos.push_back(info);
            }
        }
        
        {
            /***********************************************************/
            m_pItemInfos.clear();
            m_uNoItemRows.clear();
            
            //单行本item的row计算
            m_uItemsRowCount = m_obListItemInfos.size() / m_uiItems[0] + 1;
            if (m_obListItemInfos.size() % m_uiItems[0] != 0)
            {
                m_uItemsRowCount ++;
            }
            
            //多行本item的row计算
            m_uItemsRowCount += m_obAloneItemInfos.empty() ? 0 : m_obAloneItemInfos.size() / m_uiItems[0] + 1;
            if (m_obAloneItemInfos.size() % m_uiItems[0] != 0)
            {
                m_uItemsRowCount ++;
            }
            
            //分类标签row计算
            unsigned int noItemRow1 = 0;
            unsigned int noItemRow2 = m_obListItemInfos.size() / m_uiItems[0] + 1;
            if (m_obListItemInfos.size() % m_uiItems[0] != 0)
            {
                noItemRow2 ++;
            }
            
            m_uNoItemRows.insert(noItemRow1);
            if (!m_obAloneItemInfos.empty())
            {
                m_uNoItemRows.insert(noItemRow2);
            }
            
            //map
            for (int i=0; i<m_uItemsRowCount; i++)
            {
                CC_CONTINUE_IF(m_uNoItemRows.count(i) > 0);
                for (int j=0; j<m_uiItems[0]; j++)
                {
                    if (i <= noItemRow2)
                    {
                        int index = (i - 1) * m_uiItems[0] + j;
                        if (m_obListItemInfos.size() > index)
                        {
                            m_pItemInfos[CAIndexPath3E(0, i, j)] = &m_obListItemInfos.at(index);
                        }
                        else
                        {
                            m_pItemInfos[CAIndexPath3E(0, i, j)] = NULL;
                        }
                    }
                    else
                    {
                        int index = (i - 1 - noItemRow2) * m_uiItems[0] + j;
                        if (m_obAloneItemInfos.size() > index)
                        {
                            m_pItemInfos[CAIndexPath3E(0, i, j)] = &m_obAloneItemInfos.at(index);
                        }
                        else
                        {
                            m_pItemInfos[CAIndexPath3E(0, i, j)] = NULL;
                        }
                    }
                }
            }
            /***********************************************************/
        }
        
        m_obSimilarItemInfos.clear();
        const CSJson::Value& similarData = json["data"]["similar"];
        for (int i=0; i<similarData.size(); i++)
        {
            DMBriefInfo info = DMBriefInfo(similarData[i]["id"].asString(),
                                           similarData[i]["cover"].asString(),
                                           similarData[i]["title"].asString(),
                                           similarData[i]["last_update_chapter_name"].asString());
            m_obSimilarItemInfos.push_back(info);
        }
        
        
    }
    else
    {

    }
    
    if (m_pLoading->isAnimating())
    {
        m_pLoading->stopAnimating();
    }
    else
    {
        CC_RETURN_IF(isJoinDownload);
        m_pCollectionView->reloadData();
    }
}

void DMIntroductionController::joinDownload()
{
    isJoinDownload = true;
    if (m_pLoading == NULL || m_pLoading->isAnimating())
    {
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(DMIntroductionController::joinDownload));
        this->getView()->runAction(CCSequence::create(CCDelayTime::create(1/30.0f), callFunc, NULL));
    }
    else
    {
        DMSelectionDownloadController* controller = new DMSelectionDownloadController(m_pHeaderView->getIntroductionInfo(), m_obListItemInfos, m_obAloneItemInfos);
        controller->init();
        controller->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->replaceViewController(controller, false);
    }
}

void DMIntroductionController::onReturn(CAControl* control, CCPoint point)
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}

void DMIntroductionController::onDownLoad(CAControl* control, CCPoint point)
{
    DMSelectionDownloadController* controller = new DMSelectionDownloadController(m_pHeaderView->getIntroductionInfo(), m_obListItemInfos, m_obAloneItemInfos);
    controller->init();
    controller->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
}

void DMIntroductionController::onReadBook(CAControl* control, CCPoint point)
{
    
    DMIntroductionInfoManager* dIIMgr = DMIntroductionInfoManager::sharedDMIntroductionInfoManager();
    DMBrowseInfo readInfo = dIIMgr->selectById(m_sID);
    if (readInfo.readIndex > -1)
    {
        const std::vector<DMIntroductionItemInfo>& vec = readInfo.isList ? m_obListItemInfos : m_obAloneItemInfos;
        DMBrowseController* browseController = new DMBrowseController(m_sID, vec, readInfo.readIndex);
        browseController->setTitle(readInfo.readChapTitle);
        browseController->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(browseController, false);
        browseController->release();
        updateWanderRecord(readInfo.readIndex, readInfo.isList, readInfo.readChapTitle, m_sID);
    }
    else
    {
        DMBrowseController* browseController = new DMBrowseController(m_sID, readInfo.isList ? m_obListItemInfos : m_obAloneItemInfos, 0);
        browseController->setTitle(m_obListItemInfos.at(0).chapter_name);
        browseController->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(browseController, false);
        browseController->release();
        updateWanderRecord(0 , 1, m_obListItemInfos.at(0).chapter_name, m_sID);
    }
    
}

void DMIntroductionController::onAddCollect(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    updateCollectData();
}

void DMIntroductionController::updateCollectData()
{
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    DMUser usr = uMgr->onLineUser();
    DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
    bool coll = cMgr->isCollect(m_sID);
    //登陆状态并且没有收藏,进行收藏
    if (usr.uid.length()>0 && !coll)
    {
        addSubScirbeWithRemote();
        
    }
    //登陆状态并且已收藏,取消收藏
    else if (usr.uid.length()>0 && coll)
    {
        delSubScribeWithRemote();
        
    }
    //非登陆状态，先登陆再收藏
    else
    {
        
        _loginForReason = LoginForReason_Collection;
        
        startLoginController();
    }
}

void DMIntroductionController::onSort(CAControl* control, CCPoint point)
{
    CASegmentedControl* seg = (CASegmentedControl*)control;
    m_bReverse[seg->getTag()] = seg->getSelectedIndex();
    
    std::reverse(m_obListItemInfos.begin(), m_obListItemInfos.end());
    std::reverse(m_obAloneItemInfos.begin(), m_obAloneItemInfos.end());
    
    //map
    unsigned int noItemRow2 = m_obListItemInfos.size() / m_uiItems[0] + 1;
    if (m_obAloneItemInfos.size() % m_uiItems[0] != 0)
    {
        noItemRow2 ++;
    }
    for (int i=0; i<m_uItemsRowCount; i++)
    {
        CC_CONTINUE_IF(m_uNoItemRows.count(i) > 0);
        for (int j=0; j<m_uiItems[0]; j++)
        {
            if (i <= noItemRow2)
            {
                int index = (i - 1) * m_uiItems[0] + j;
                if (m_obListItemInfos.size() > index)
                {
                    m_pItemInfos[CAIndexPath3E(0, i, j)] = &m_obListItemInfos.at(index);
                }
                else
                {
                    m_pItemInfos[CAIndexPath3E(0, i, j)] = NULL;
                }
            }
            else
            {
                int index = (i - 1 - noItemRow2) * m_uiItems[0] + j;
                if (m_obAloneItemInfos.size() > index)
                {
                    m_pItemInfos[CAIndexPath3E(0, i, j)] = &m_obAloneItemInfos.at(index);
                }
                else
                {
                    m_pItemInfos[CAIndexPath3E(0, i, j)] = NULL;
                }
            }
        }
    }

    
    m_pCollectionView->reloadData();
}

void DMIntroductionController::onAuthorClick(CAControl* control, CCPoint point)
{
    CAButton* btn = dynamic_cast<CAButton*>(control);
    CALabel* label = dynamic_cast<CALabel*>(btn->getBackGroundViewForState(CAControlStateNormal));
    CC_RETURN_IF(label->getText().compare("") == 0);
    const std::string& author = label->getText();
    
    DMSearchController* searchController = DMSearchController::create();
    searchController->setExtraKeyWord(author);
    this->presentModalViewController(searchController, true);
    
    
}

void DMIntroductionController::tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index)
{
    m_eType = (DMIntroductionItemType)index;
    
    m_pCollectionView->setHoriInterval(m_obItemInterval[index].width);
    m_pCollectionView->setVertInterval(m_obItemInterval[index].height);
    
    if (m_eType==DMIntroductionItem2 && m_eType)
    {
        HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeBookDetail, m_sID);
        CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMIntroductionController::onRequestFinished));
        
        if (m_DMCommentModels.size()==0)
        {
            requestCommentInfo();
        }
    }
    
    if (m_eType !=DMIntroductionItem2)
    {
        if (m_pCommentInputer)
        {
            m_pCommentInputer->setVisible(false);
        }
        
        m_pCollectionView->setFooterRefreshView(NULL);
        
        CCRect frame = m_pCollectionView->getFrame();
        frame.size.height = _iCollectionHeight;
        m_pCollectionView->setFrame(frame);
    }
    else
    {
        m_pCommentInputer->setVisible(true);
        m_pCollectionView->setFooterRefreshView(m_pFooterView);
        
        CCRect frame = m_pCollectionView->getFrame();
        frame.size.height = _iCollectionHeight-_px(88);
        m_pCollectionView->setFrame(frame);
        
        
    }
    
    m_pCollectionView->reloadData();
}


void DMIntroductionController::onActionPerformed(LoginAction action)
{
    
    switch (_loginForReason)
    {
        case LoginForReason_Collection:
        {
            if (action == LoginActionLoginSuccess)
            {
                addSubScirbeWithRemote();
            }
        }
            break;
        case LoginForReason_SubmitComment:
        {
            if (action == LoginActionLoginSuccess)
            {
                submitComment(_modelCacheForSubmit);
            }
        }
            break;
    }
    
    
}

void DMIntroductionController::addSubScirbeWithRemote()
{
    std::map<string, string> key_value;
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeSubScribAdd, uMgr->onLineUser().uid, m_sID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMIntroductionController::onAddScribeWithRemoteFinish));
}

void DMIntroductionController::delSubScribeWithRemote()
{
    std::map<string, string> key_value;
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeSubScribDel, uMgr->onLineUser().uid, m_sID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMIntroductionController::onDelScribeWithRemoteFinish));
}

void DMIntroductionController::onAddScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
        cMgr->deCollect(m_sID);
        cMgr->collect(m_sID);
        updateCollectionStatus();
    }
}

void DMIntroductionController::onDelScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
        cMgr->deCollect(m_sID);
        updateCollectionStatus();
    }
}


unsigned int DMIntroductionController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    if (m_eType == DMIntroductionItem1)
    {
        return _px(70);
    }
    else if (m_eType == DMIntroductionItem3)
    {
        float width = _dip(CAApplication::getApplication()->getWinSize().width);
        return _px(((width - 30) / m_uiItems[2] - 30) * 1.33f) + _px(45);
    }
    else
    {
        return getCommentCellHeight(m_DMCommentModels.at(row));
    }
}


CACollectionViewCell* DMIntroductionController::getCollectionCell(const CCSize& cellSize, unsigned int row)
{
    int index = row == 0 ? 0 : 1;
    
    CACollectionViewCell* cell = NULL;
    
    if (index < m_pViewCells.size() == false)
    {
        char str[8];
        sprintf(str, "Cell%d", index);
        cell = CACollectionViewCell::create(str);
        cell->setBackgroundView(NULL);
        m_pViewCells.pushBack(cell);
        
        float winSizeWidth = _dip(cellSize.width);
        
        CADipRect rect = CADipRect(150, 45, 260, 70);
        CALabel* label = CALabel::createWithCenter(rect);
        label->setFontSize(_px(24));
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setColor(ccc4(153, 153, 153, 255));
        cell->addSubview(label);
        
        char text[24];
        if (row == 0)
        {
            sprintf(text, "连载(%lu话)", m_obListItemInfos.size());
        }
        else
        {
            sprintf(text, "单行本(%lu话)", m_obAloneItemInfos.size());
        }
        
        label->setText(text);
        
        if (row == 0)
        {
            CADipRect rect1 = CADipRect(winSizeWidth - 250, 45, 200, 70);
            CALabel* label2 = CALabel::createWithCenter(rect1);
            label2->setFontSize(_px(24));
            label2->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label2->setColor(ccc4(153, 153, 153, 255));
            label2->setText("排序方式:");
            cell->addSubview(label2);
            
            CADipRect rect2 = CADipRect(winSizeWidth - 130, 45, 200, 50);
            CASegmentedControl* control = CASegmentedControl::createWithCenter(rect2, 2);
            control->setBackgroundImageAtIndex(NULL, 0, CAControlStateNormal);
            control->setBackgroundImageAtIndex(NULL, 1, CAControlStateNormal);
            control->setBackgroundImageAtIndex(CAImage::create(sortButton), 0, CAControlStateHighlighted);
            control->setBackgroundImageAtIndex(CAImage::create(sortButton), 1, CAControlStateHighlighted);
            control->setBackgroundImageAtIndex(CAImage::create(sortButton), 0, CAControlStateSelected);
            control->setBackgroundImageAtIndex(CAImage::create(sortButton), 1, CAControlStateSelected);
            control->setTitleAtIndex("正序", 0, CAControlStateAll);
            control->setTitleAtIndex("倒序", 1, CAControlStateAll);
            control->setTitleColorAtIndex(CAColor_gray, 0, CAControlStateAll);
            control->setTitleColorAtIndex(CAColor_gray, 1, CAControlStateAll);
            control->setTitleColorAtIndex(CAColor_white, 0, CAControlStateHighlighted);
            control->setTitleColorAtIndex(CAColor_white, 1, CAControlStateHighlighted);
            control->setTitleColorAtIndex(CAColor_white, 0, CAControlStateSelected);
            control->setTitleColorAtIndex(CAColor_white, 1, CAControlStateSelected);
            control->setSelectedAtIndex(1);
            control->setTag(index);
            cell->addSubview(control);
            control->addTarget(this, CAControl_selector(DMIntroductionController::onSort));
        }
    }
    else
    {
        cell = m_pViewCells.at(index);
    }
    
    return cell;
}

CACollectionViewCell* DMIntroductionController::getItem1(const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = NULL;
    
    if (m_uNoItemRows.find(row) == m_uNoItemRows.end())
    {
        cell = m_pCollectionView->dequeueReusableCellWithIdentifier("item");
    }
    else
    {
        int index = row == 0 ? 0 : 1;
        char str[8];
        sprintf(str, "Cell%d", index);
        cell = m_pCollectionView->dequeueReusableCellWithIdentifier(str);
    }
    
    if (cell == NULL)
    {
        if (m_uNoItemRows.find(row) == m_uNoItemRows.end())
        {
            cell = new DMIntroductionItemCell();
            ((DMIntroductionItemCell*)cell)->initWithReuseIdentifier("item", cellSize);
            cell->autorelease();
            cell->setAllowsSelected(false);
        }
        else
        {

            cell = this->getCollectionCell(cellSize, row);
        }
    }
    
    CAIndexPath3E indexPath = CAIndexPath3E(section, row, item);
    if (m_uNoItemRows.find(row) == m_uNoItemRows.end())
    {
        DMIntroductionItemCell* itemCell = dynamic_cast<DMIntroductionItemCell*>(cell);
        DMIntroductionItemInfo* info = m_pItemInfos.at(indexPath);
        if (info)
        {
            itemCell->getLabel()->setText(info->chapter_name);
            itemCell->setHaveDownload(m_sHaveDownloadInfoIDs.count(info->ID) > 0);
        }
        else
        {
            itemCell->setVisible(false);
        }
    }
    return cell;
}

static string getTime(std::string time){
    time_t t = atol(time.c_str());
    struct tm *p;
    p=gmtime(&t);
    char s[100];
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
    return s;
}

CACollectionViewCell* DMIntroductionController::getItem2(const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    DMCommentCell* cell = (DMCommentCell*)m_pCollectionView->dequeueReusableCellWithIdentifier("item2");
    
    if (cell==NULL)
    {
        cell= new DMCommentCell();
        cell->autorelease();
        cell->initWithReuseIdentifier("item2", CCSize(this->getView()->getFrame().size.width, 0));
    }
    
    DMCommentModel model = m_DMCommentModels.at(row);
    if (cell->getSubViewLayout())
    {
        cell->getSubViewLayout()->removeAllSubviews();
    }
    
    cell->getHeadView()->setUrl(model.avatar_url);
    string time = model.createtime.length()==0 ? " " :getTime(model.createtime);
    cell->getDateView()->setText(time);
    cell->getTopName()->setText(model.nickname.length()==0 ? " " : model.nickname);
    cell->getMainComment()->setText(model.content.length()==0 ? " ": model.content);
    
    //设置主评论块儿的Frame
    CCRect contR = getMainCommentRect(model, cell->getMainComment()->getFontSize());
    cell->getMainComment()->setFrame(contR);
    
    //设置回复块的Frame
    CCRect repR = getFeedBackRect(contR);
    
    CSJson::Value value;
    value["id"]=model._id;
    value["uid"]=model.uid;
    value["nickname"]=model.nickname;
    
    cell->getBtnFeedBack()->setTextTag(value.toStyledString());
    cell->getBtnFeedBack()->addTarget(this, CAControl_selector(DMIntroductionController::onFeedBackAction), CAControlEventTouchUpInSide);
    cell->getBtnFeedBack()->setFrame(repR);
    
    //设置子评论块的Frame
    CCRect subsR = getSubFeedBacksRect(repR, contR, m_DMCommentModels.at(row));
    cell->getSubViewLayout()->setFrame(subsR);
    
    if (m_DMCommentModels.at(row).reply.size()>0)
    {
        CCRect preR = CCRect(0, 0, 0, 0);
        for (int i = 0 ; i< m_DMCommentModels.at(row).reply.size(); i++)
        {
            CAView *view = generateSubCommentView(m_DMCommentModels.at(row).reply.at(i), m_DMCommentModels.at(row).nickname, this->getView()->getFrame().size.width-_px(175));
            if (preR.size.width == 0)
            {
                CCRect r = view->getFrame();
                r.origin.x = _px(15);
                r.origin.y = _px(15);
                view->setFrame(r);
                cell->getSubViewLayout()->addSubview(view);
                preR = view->getFrame();
            }
            else
            {
                CCRect cR = CCRect(preR.origin.x, preR.origin.y+ preR.size.height+_px(10), view->getFrame().size.width, view->getFrame().size.height);
                preR = cR;
                view->setFrame(cR);
                cell->getSubViewLayout()->addSubview(view);
            }
        }
    }
    //添加获取更多回复按钮
    if (m_DMCommentModels.at(row).reply.size() < m_DMCommentModels.at(row).replycount) {
        CCRect mRR = CCRect(_px(20), subsR.size.height-_px(70), subsR.size.width - _px(40) , _px(55));
        CAButton *moreReply = CAButton::createWithFrame(mRR, CAButtonTypeCustom);
        moreReply->setTitleForState(CAControlStateAll, "查看更多评论");
        moreReply->setTitleColorForState(CAControlStateAll, ccc4(102, 102, 102, 255));
        CAScale9ImageView *bacImg = CAScale9ImageView::createWithImage(CAImage::create(getMoreReplyBtn));
        CAScale9ImageView *bacImg2 = CAScale9ImageView::createWithImage(CAImage::create(getMoreReplyBtn));
        bacImg2->setColor(ccc4(217, 217, 217, 255));
        moreReply->setBackGroundViewForState(CAControlStateNormal, bacImg);
        moreReply->setBackGroundViewForState(CAControlStateHighlighted, bacImg2);
        
        
        moreReply->setTextTag(m_DMCommentModels.at(row)._id);
        moreReply->addTarget(this, CAControl_selector(DMIntroductionController::getMoreCommentReplys), CAControlEventTouchUpSide);
        
        cell->getSubViewLayout()->addSubview(moreReply);
    }
    
    return cell;
}


void DMIntroductionController::getMoreCommentReplys(CAControl *control,CCPoint cp)
{
    CAButton *button = (CAButton *)control;
    
    std::string url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeGetCommentReply).url;
    
    std::map<string, string> key_value;
    
    url= url+"/"+m_sID+"/"+button->getTextTag();
    
    CommonHttpManager::getInstance()->send_post(url, key_value, this, CommonHttpJson_selector(DMIntroductionController::onRequestCommonReplyFinish));
    
    
}

void DMIntroductionController::onRequestCommonReplyFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    vector<DMCommentModel> moreReps;
    
    if (status == HttpResponseSucceed)
    {
        if (json["data"].isArray() && json["data"].size()>0)
        {
            const CSJson::Value &v = json["data"];
            for (int i = 0 ; i<v.size(); i++)
            {
                DMCommentModel model;
                const CSJson::Value &mv = v[i];
                converse(mv, model);
                moreReps.push_back(model);
            }
        }
    }
    
    std::string pid = moreReps.at(0).pid;
    for (int i = 0 ; i<m_DMCommentModels.size(); i++) {
        if (m_DMCommentModels.at(i)._id.compare(pid)==0) {
            m_DMCommentModels.at(i).reply = moreReps;
        }
    }
    
    m_pCollectionView->reloadData();
}


//得到一个评论块的高度
int DMIntroductionController::getCommentCellHeight(DMCommentModel &model)
{
    //固定高度的是头像块和回复按钮块，高度为_px(180)
    //右侧的宽度为 _px(135) 左侧空余105右边距30
    
    //主评论块儿的Frame
    CCRect contR = getMainCommentRect(model, _px(28));

    //回复块的Frame
    CCRect repR = getFeedBackRect(contR);
    
    //子评论块的Frame
    CCRect subsR = getSubFeedBacksRect(repR, contR, model);
    
    return subsR.origin.y + subsR.size.height + _px(15);
    
}

//每一个回复块的高度
int DMIntroductionController::getReplyItemHeight(DMCommentModel &model)
{
    
    int staH = _px(30);
    int staW = this->getView()->getFrame().size.width - _px(135);
    int tH = CAImage::getStringHeight("", _px(30), model.content.c_str(), staW);
    
    return staH + tH;
}


CCRect DMIntroductionController::getMainCommentRect(DMCommentModel &model,int textsize)
{
    
    int mCRW = getView()->getFrame().size.width - _px(150);
    int mCH = CAImage::getStringHeight("", textsize, model.content.c_str(), mCRW);
    
    //设置主评论块儿的Frame
    CCRect contR = CCRect(_px(105), _px(107), mCRW, mCH);
    
    return contR;
}

CCRect DMIntroductionController::getFeedBackRect(CCRect &contR)
{
    CCRect repR = CCRect(this->getView()->getFrame().size.width - _px(140), contR.origin.y+contR.size.height + _px(25), _px(110), _px(40));
    
    return repR;
}

CCRect DMIntroductionController::getSubFeedBacksRect(CCRect &repR,CCRect &contR,DMCommentModel &model)
{
    int comH = getSubFeedBacksHeight(model);
    CCRect subR = CCRect(_px(105), repR.origin.y +repR.size.height + _px(25) , contR.size.width, comH);
    return subR;
}

int DMIntroductionController::getSubFeedBacksHeight(DMCommentModel &model)
{
    int comH = 0;
    if (model.reply.size()>0)
    {
        for (int i =0; i<model.reply.size(); i++)
        {
            comH +=getReplyItemHeight(model.reply.at(i));
            comH +=10;
        }
    }
    
    if (comH>=0 && (model.reply.size()<model.replycount))
    {
        comH+=_px(90);
    }
    else if (comH>0)
    {
        comH+=_px(20);
    }
    else{
        comH=0;
    }
    
    return comH;
}


CAButton *DMIntroductionController::generateSubCommentView(DMCommentModel model ,string pstr,int width)
{
    
    std::string toContent = model.content;
    int nameStart = toContent.find("@");
    int nameEnd = toContent.find(":");
    
    std::string subName ;
    std::string subCont ;
    if (nameEnd > nameStart && nameEnd >0 )
    {
        subName = toContent.substr(nameStart+1,nameEnd-nameStart-1);
        subCont = toContent.substr(nameEnd+1,toContent.length()-nameEnd+1);
    }
    
//    CCLog("content=%s,\nnamestart=%d,\nnameend=%d,\nsubName=%s,\nsubCont=%s",toContent.c_str(),nameStart,nameEnd,subName.c_str(),subCont.c_str());

    
    CAButton *viewLayout = CAButton::create(CAButtonTypeCustom);
    CAView *viewLayoutBack = CAView::create();
    viewLayoutBack->setColor(CAColor_clear);
    
    int preLen = 0;
    int surLen = 0;
    int midLen = 0;
    
    if (model.nickname.length()>0)
    {
        preLen = CAImage::getStringWidth("", _px(28), model.nickname.c_str());
        CALabel *Lable = CALabel::createWithFrame(CCRect(0, 0, preLen, _px(30)));
        Lable->setFontSize(_px(28));
        Lable->setColor(ccc4(71, 135, 241, 255));
        Lable->setText(model.nickname);
        Lable->sizeToFit();
//        Lable ->setText(subName);
        viewLayoutBack->addSubview(Lable);
    }
    
    if (pstr.length()>0)
    {
        surLen = CAImage::getStringWidth("", _px(28), pstr.c_str());
        midLen = CAImage::getStringWidth("", _px(28), "回复");
        CALabel *mid = CALabel::createWithFrame(CCRect(preLen+_px(3), 0, midLen, _px(30)));
        mid->setFontSize(_px(28));
        mid->setColor(CAColor_black);
        mid->setText("回复");
        mid->sizeToFit();
        viewLayoutBack->addSubview(mid);
        
        CALabel *sufLa = CALabel::createWithFrame(CCRect(preLen+midLen+_px(6), 0, surLen, _px(30)));
        sufLa->setFontSize(_px(28));
        sufLa->setColor(ccc4(71, 135, 241, 255));
//        sufLa->setText(pstr);//已经被注释
        sufLa->setText(subName.length()>0 ? subName : pstr);
        sufLa->sizeToFit();
        viewLayoutBack->addSubview(sufLa);
    }
    
    int contentHeight = CAImage::getStringHeight("", _px(28), model.content.c_str(), width);
    CALabel *cont = CALabel::createWithFrame(CCRect(0, _px(30), width, contentHeight));
    cont->setFontSize(_px(28));
    cont->setColor(CAColor_black);
//    cont->setText(model.content);//已经被注释
    cont->setText(subCont.length()>0 ? subCont : model.content);
    viewLayoutBack->addSubview(cont);
    
    viewLayoutBack->setFrame(CCRect(0, 0, width, _px(30)+contentHeight));
    viewLayout->addSubview(viewLayoutBack);
    CAView *normal = CAView::createWithFrame(viewLayoutBack->getFrame());
    CAView *hilight = CAView::createWithFrame(viewLayoutBack->getFrame());
    normal->setColor(ccc4(239, 239, 239, 255));
    hilight->setColor(ccc4(150, 150, 150, 255));
    viewLayout->setBackGroundViewForState(CAControlStateNormal, normal);
    viewLayout->setBackGroundViewForState(CAControlStateHighlighted, hilight);
    
    viewLayout->removeAllSubviews();
    viewLayout->addSubview(viewLayoutBack);
    CSJson::Value value;
    value["id"]=model._id;
    value["uid"]=model.uid;
    value["nickname"]=model.nickname;
    viewLayout->setTextTag(value.toStyledString());
    viewLayout->addTarget(this, CAControl_selector(DMIntroductionController::onFeedBackAction), CAControlEventTouchUpInSide);
    viewLayout->setFrame(CCRect(0, 0, width, _px(30)+contentHeight));
    
    return viewLayout;
}

void DMIntroductionController::onFeedBackAction(CAControl *control,CCPoint cp)
{
    CAButton *btn = (CAButton *)control;
    CSJson::Value value = btn->getTextTag();
    CSJson::Reader read;
    CSJson::Value json;
    read.parse(btn->getTextTag(),json);
    std::string _id = json["id"].asString();
    std::string uid = json["uid"].asString();
    std::string nickname = json["nickname"].asString();
    
    m_pCommentFiled->becomeFirstResponder();
    replyId = _id;
    
    m_pCommentFiled->setPlaceHolder("回复:"+nickname);
    
}



const DMCommentModel & DMIntroductionController::getLocalCommentById(std::string id)
{
    for (int i = 0 ; i< m_DMCommentModels.size() ; i++)
    {
        if (!m_DMCommentModels.at(i)._id.compare(id))
        {
            return  m_DMCommentModels.at(i);
        }
        if (m_DMCommentModels.at(i).reply.size()>0)
        {
            for (int j = 0 ; j< m_DMCommentModels.at(i).reply.size(); j++)
            {
                if (!m_DMCommentModels.at(i).reply.at(j)._id.compare(id))
                {
                    return m_DMCommentModels.at(i).reply.at(j);
                }
            }
        }
    }
    return dMCommentModelDefault;
}


CACollectionViewCell* DMIntroductionController::getItem3(const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    DMIntroductionSimilarItemCell* cell = (DMIntroductionSimilarItemCell*)m_pCollectionView->dequeueReusableCellWithIdentifier("image");
    if (cell == NULL)
    {
        cell = new DMIntroductionSimilarItemCell();
        cell->autorelease();
        cell->initWithReuseIdentifier("image", cellSize);
    }
    
    
    if (m_obSimilarItemInfos.size() > row * m_uiItems[2] + item)
    {
        const DMBriefInfo& info = m_obSimilarItemInfos.at(row * m_uiItems[2] + item);
        
        cell->getLabel()->setText(info.title);
        cell->getImageView()->setUrl(info.cover);
    }
    else
    {
        cell->setVisible(false);
    }
    
    return cell;
}

CACollectionViewCell* DMIntroductionController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = NULL;

    switch (m_eType)
    {
        case DMIntroductionItem1:
            cell = this->getItem1(cellSize, section, row, item);
            break;
            
        case DMIntroductionItem2:
            cell = this->getItem2(cellSize, section, row);
            break;
            
        case DMIntroductionItem3:
            cell = this->getItem3(cellSize, section, row, item);
            break;
            
        default:
            break;
    }

    
    return cell;
}

unsigned int DMIntroductionController::numberOfSections(CACollectionView *collectionView)
{

    return 1;
}

unsigned int DMIntroductionController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    unsigned int count = 0;
    
    if (m_eType == DMIntroductionItem1)
    {
        count = m_uItemsRowCount;
    }
    else if (m_eType == DMIntroductionItem3)
    {
        if (m_obSimilarItemInfos.size() % m_uiItems[m_eType] == 0)
        {
            count = m_obSimilarItemInfos.size() / m_uiItems[m_eType];
        }
        else
        {
            count = m_obSimilarItemInfos.size() / m_uiItems[m_eType] + 1;
        }
    }
    else
    {
        count = m_DMCommentModels.size();
    }
    
    return count;
}

unsigned int DMIntroductionController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    if (m_eType == DMIntroductionItem1 && m_uNoItemRows.count(row) > 0)
    {
        return 1;
    }
    
    return m_uiItems[m_eType];
}

unsigned int DMIntroductionController::collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
{
    return _px(80);
}

CAView* DMIntroductionController::collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
{
    if (m_pTabBarView == NULL)
    {
        std::vector<CATabBarItem*> items;
        
        for (int i=0; i<3; i++)
        {
            items.push_back(CATabBarItem::create(tabBatTitle[i], NULL));
        }
        
        m_pTabBarView = CATabBar::create(items, viewSize);
        m_pTabBarView->retain();
        m_pTabBarView->showSelectedIndicator();
        m_pTabBarView->setTitleColorForNormal(CAColor_black);
        m_pTabBarView->setTitleColorForSelected(ccc4(87, 154, 244, 255));
        m_pTabBarView->setBackGroundView(CAView::createWithColor(ccc4(244, 244, 244, 255)));
        m_pTabBarView->setSelectedBackGroundView(CAView::createWithColor(CAColor_clear));
//        m_pTabBarView->setSelectedIndicatorView(CAView::createWithColor(ccc4(87, 154, 244, 255)));
        CAImageView *imageView = CAImageView::createWithImage(CAImage::create(common_tabbarIndicator));
        m_pTabBarView->setSelectedIndicatorView(imageView);
        m_pTabBarView->setSelectedAtIndex(0);
        m_pTabBarView->setDelegate(this);
        
        for (int i=0; i<2; i++)
        {
            CCRect rect = CCRect(m_pTabBarView->getSegmentedControlFrame().size.width/3 * (i+1),
                                 m_pTabBarView->getSegmentedControlFrame().size.height/2,
                                 2,
                                 m_pTabBarView->getSegmentedControlFrame().size.height/2);
            rect.origin = ccpAdd(m_pTabBarView->getSegmentedControlFrame().origin, rect.origin);
            CAImageView* imageView = CAImageView::createWithImage(CAImage::create(divisionIcon));
            imageView->setCenter(rect);
            m_pTabBarView->insertSubview(imageView, 2);
        }
    }
    return m_pTabBarView;
}

void DMIntroductionController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    if (m_eType == DMIntroductionItem1)
    {
        CC_RETURN_IF(m_pItemInfos.find(CAIndexPath3E(section, row, item)) == m_pItemInfos.end());
        DMIntroductionItemInfo* info = m_pItemInfos.at(CAIndexPath3E(section, row, item));
        
        bool isList = true;
        int row_ = 0;
        for (std::set<unsigned int>::iterator it = m_uNoItemRows.begin();
             it != m_uNoItemRows.end(); it++)
        {
            if (*it != 0)
            {
                row_ = *it;
                isList = row < row_;
            }
        }
        
        int index = 0;
        
        std::vector<DMIntroductionItemInfo> vec;
        if (isList)
        {
            vec = m_obListItemInfos;
            index = (row - 1) * m_uiItems[0] + item;
            if (m_bReverse[0])
            {
                std::reverse(vec.begin(), vec.end());
                index = vec.size() - index - 1;
            }
            
        }
        else
        {
            vec = m_obAloneItemInfos;
            index = (row - row_ - 1) * m_uiItems[0] + item;
            if (m_bReverse[1])
            {
                std::reverse(vec.begin(), vec.end());
                index = vec.size() - index - 1;
            }
        }
        
        
        DMBrowseController* browseController = new DMBrowseController(m_sID, vec, index);
        browseController->setTitle(info->chapter_name);
        browseController->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(browseController, false);
        browseController->release();
        updateWanderRecord(index, isList, info->chapter_name, m_sID);
        m_pHeaderView->updateStartReadButton();
        
    }
    else if (m_eType == DMIntroductionItem3)
    {
        if (m_obSimilarItemInfos.size() > row * m_uiItems[2] + item)
        {
            const DMBriefInfo& info = m_obSimilarItemInfos.at(row * m_uiItems[2] + item);
            RootWindow::getInstance()->getRootNavigationController()->pushViewController(DMIntroductionController::create(info.ID, info.title), true);
        }
    }
}


void DMIntroductionController::updateCollectionStatus()
{
    
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    DMUser user = uMgr->onLineUser();
    
    DMColllectionManager* cMgr= DMColllectionManager::sharedDMColllectionManager();
    bool coll = cMgr->isCollect(m_sID);
    if (user.uid.length()>0 && coll)
    {
        CAImageView* addCollectionNormal = CAImageView::createWithImage(CAImage::create(addCollectionButtonIcon));
        this->m_pHeaderView->getAddCollectionButton()->setBackGroundViewForState(CAControlStateAll, addCollectionNormal);
        CAImageView* addCollectionHighlighted = CAImageView::createWithImage(CAImage::create(addCollectionButtonIcon));
        addCollectionHighlighted->setColor(CAColor_gray);
        this->m_pHeaderView->getAddCollectionButton()->setBackGroundViewForState(CAControlStateHighlighted, addCollectionHighlighted);
//        this->m_pHeaderView->getAddCollectionButton()->setTitleForState(CAControlStateAll,"已收藏");
        this->m_pHeaderView->getAddCollectionLable()->setText("已收藏");
    }
    else
    {
        CAImageView* addCollectionNormal = CAImageView::createWithImage(CAImage::create(addCollectionButtonIcon));
        this->m_pHeaderView->getAddCollectionButton()->setBackGroundViewForState(CAControlStateAll, addCollectionNormal);
        CAImageView* addCollectionHighlighted = CAImageView::createWithImage(CAImage::create(addCollectionButtonIcon));
        addCollectionHighlighted->setColor(CAColor_gray);
        this->m_pHeaderView->getAddCollectionButton()->setBackGroundViewForState(CAControlStateHighlighted, addCollectionHighlighted);
//        this->m_pHeaderView->getAddCollectionButton()->setTitleForState(CAControlStateAll,"添加收藏");
        this->m_pHeaderView->getAddCollectionLable()->setText("添加收藏");
    }
    
}

void DMIntroductionController::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeBookDetail, m_sID);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMIntroductionController::onRequestFinished));
}

void DMIntroductionController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    requestCommentInfo();
}

void DMIntroductionController::requestCommentInfo()
{
    std::map<string, string> key_value;
    std::string url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeGetComment).url;
    std::string tomicID = m_sID;
    char page[128];
    int pn = m_DMCommentModels.size();
    sprintf(page, "%d",pn);
    url= url+"/"+m_sID+"/"+page;
    
    CommonHttpManager::getInstance()->send_post(url, key_value, this, CommonHttpJson_selector(DMIntroductionController::onRequestCommonFinish));
}

void DMIntroductionController::onRequestCommonFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        if (json["data"].isArray() && json["data"].size()>0)
        {
            const CSJson::Value &v = json["data"];
            for (int i = 0 ; i<v.size(); i++)
            {
                DMCommentModel model;
                const CSJson::Value &mv = v[i];
                converse(mv, model);
                m_DMCommentModels.push_back(model);
            }
        }
    }
    m_pCollectionView->reloadData();
}

static void getLowercase(std::string& string)
{
    for (int i=0; i<string.length(); i++)
    {
        if (string[i] >= 65 && string[i]<=90)
        {
            string[i] += 32;
        }
    }
}

void DMIntroductionController::submitComment(DMCommentModel & model)
{
    
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    DMUser usr = uMgr->onLineUser();
    
    if (usr.uid.length()==0)
    {
        _loginForReason = LoginForReason_SubmitComment;
        _modelCacheForSubmit = model;
        _cachedTextForSubmit = m_pCommentFiled->getText();
        startLoginController();
        return;
    }
    
    std::map<string, string> key_value;
    
    //评论
    if (model._id.length()==0 || !model._id.compare("0"))
    {
        key_value["type"]="0";
        key_value["pid"]="0";
        
        key_value["id"]=model._id;
        key_value["uid"]=usr.uid;
        key_value["nickname"]=usr.nickname;
        key_value["avatar_url"]=usr.photo;
        key_value["comic_id"]=m_sID;
        key_value["content"]=m_pCommentFiled->getText().length()==0 ? _cachedTextForSubmit: m_pCommentFiled->getText();
        _cachedTextForSubmit = "";
        
    }
    //回复
    else
    {
        key_value["type"]="1";
        
        key_value["pid"]=(model.pid.length()==0 || !(model.pid.compare("0"))) ? model._id : model.pid;
        
        key_value["id"]=model._id;
        key_value["uid"]=usr.uid;
        key_value["nickname"]=usr.nickname;
        key_value["avatar_url"]=usr.photo;
        key_value["comic_id"]=model.comic_id;
        key_value["author_id"]=model.pid;
        key_value["author"]=model.nickname;
        key_value["content"]="回复@"+model.nickname+":"+(m_pCommentFiled->getText().length()==0 ? _cachedTextForSubmit: m_pCommentFiled->getText());
        _cachedTextForSubmit = "";
        
    }
    
    std::string firstStr = "b1r2y3s4j5"+usr.uid;
    std::string firstMd = MD5(firstStr).md5();
    getLowercase(firstMd);
    
    std::string secondStr = usr.nickname+firstMd;
    std::string token = MD5(secondStr).md5();
    getLowercase(token);
    
    key_value["token"] = token;
    
    std::string url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeAddComment).url;
    
    
    CommonHttpManager::getInstance()->send_post(url, key_value, this, CommonHttpJson_selector(DMIntroductionController::submitCommentFinished));
}

void DMIntroductionController::startLoginController()
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

void DMIntroductionController::submitCommentFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    
    if (status == HttpResponseSucceed)
    {
        if(json["result"].asInt()==1){
            
            if (replyId.length() ==0 || !replyId.compare("0")) {
                DMCommentModel model ;
                converse(json["data"], model);
                m_DMCommentModels.insert(m_DMCommentModels.begin(), model);
                
            }
            else
            {
                DMCommentModel omodel ;
                converse(json["data"], omodel);
                DMCommentModel model = getLocalCommentById(replyId);
                for (int i = 0 ; i<m_DMCommentModels.size(); i++)
                {
                    if (!m_DMCommentModels.at(i)._id.compare(model.pid))
                    {
                        m_DMCommentModels.at(i).reply.push_back(omodel);
                        break;
                    }
                }
            }
            m_pCollectionView->reloadData();
        }
    }
    
}

bool DMIntroductionController::getKeyBoardHeight(int height)
{
    CCRect inputR =  m_pCommentInputer->getFrame();
    inputR.origin.y = getView()->getFrame().size.height - (_px(88)+height);
    m_pCommentInputer->setFrame(inputR);
    return false;
}

bool DMIntroductionController::onTextFieldDetachWithIME(CATextField * sender)
{
    
    CCRect inputR =  m_pCommentInputer->getFrame();
    inputR.origin.y = getView()->getFrame().size.height -_px(88);
    m_pCommentInputer->setFrame(inputR);
    m_pCommentFiled->setPlaceHolder("评论");
    m_pCommentFiled->setText("");
    
    return false;
}

bool DMIntroductionController::onTextFieldAttachWithIME(CATextField * sender)
{
    replyId = "";
    return false;
}

bool DMIntroductionController::keyBoardCallBack(CATextField *sender)
{
    //发送操作
    CCLog("DMIntroductionController  %s",sender->getText().c_str());
    
    DMCommentModel model = getLocalCommentById(replyId);
    submitComment(model);
    return false;
}

std::string DMIntroductionController::getID()
{
    return m_sID;
}


DMIntroductionHeaderView::DMIntroductionHeaderView(DMIntroductionController* controller)
: m_pBookImageView(NULL), m_pBookAuthorLabel(NULL)
, m_pInfernalLabel(NULL), m_pStatusLabel(NULL), m_pTheLatestLabel(NULL)
, m_pDetailsLabel(NULL), m_pAddCollectionButton(NULL)
, m_pStartReadButton(NULL) ,m_pOpenContractionButton(NULL)
, m_pAddCollectionLable(NULL),m_pStartReadLable(NULL)
, m_pController(controller), m_bDetailsLabelShowAll(false)
{

}

DMIntroductionHeaderView::~DMIntroductionHeaderView()
{

}

bool DMIntroductionHeaderView::init()
{
    float __scale = CAApplication::getApplication()->getWinSize().width / 640.0f;
    __scale = MIN(__scale, 1.8);
    CAColor4B fontColor = ccc4(100, 100, 100, 255);
    
    CCRect bgRect;
    bgRect.origin = ccpMult(CCPoint(30, 30), __scale);
    bgRect.size = ccpMult(CCPoint(160, 210), __scale);
    CAScale9ImageView* bookBg = CAScale9ImageView::createWithImage(CAImage::create(book_bg));
    bookBg->setFrame(bgRect);
    this->addSubview(bookBg);
    
    
    CCRect imageRect;
    imageRect.origin = ccpAdd(bgRect.origin, CCPoint(4, 4));
    imageRect.size = ccpSub(bgRect.size, CCSize(8, 8));
    m_pBookImageView = CommonUrlImageView::createWithFrame(imageRect);
    this->addSubview(m_pBookImageView);
    m_pBookImageView->setImage(CAImage::create("dm_resource/1.png"));
    
    CCRect authorTitleLabelRect;
    authorTitleLabelRect.origin.x = bgRect.origin.x * 2 + bgRect.size.width;
    authorTitleLabelRect.origin.y = bgRect.origin.y;
    authorTitleLabelRect.size = CCPoint(75, 35) * __scale;
    CALabel* authorTitleLabel = CALabel::createWithFrame(authorTitleLabelRect);
    authorTitleLabel->setNumberOfLine(1);
    authorTitleLabel->setFontSize(24 * __scale);
    authorTitleLabel->setColor(fontColor);
    authorTitleLabel->setText("作者：");
    this->addSubview(authorTitleLabel);
    
    CCRect authorLabelRect;
    authorLabelRect.origin.x = bgRect.origin.x * 2 + bgRect.size.width + 75 * __scale;
    authorLabelRect.origin.y = bgRect.origin.y;
    authorLabelRect.size = CCPoint(400, 35) * __scale;
    m_pBookAuthorLabel = CALabel::createWithFrame(authorLabelRect);
    m_pBookAuthorLabel->setNumberOfLine(1);
    m_pBookAuthorLabel->setFontSize(24 * __scale);
    m_pBookAuthorLabel->setColor(ccc4(70, 134, 243, 255));
    m_pBookAuthorLabel->setUnderLine(true);
    m_pBookAuthorLabel->setText("");
    
    CAButton* button = CAButton::createWithFrame(authorLabelRect, CAButtonTypeCustom);
    button->setBackGroundViewForState(CAControlStateNormal, m_pBookAuthorLabel);
    this->addSubview(button);
    button->addTarget(m_pController, CAControl_selector(DMIntroductionController::onAuthorClick), CAControlEventTouchUpInSide);
    
    CCRect infernalLabelRect;
    infernalLabelRect.origin.x = bgRect.origin.x * 2 + bgRect.size.width;
    infernalLabelRect.origin.y = authorLabelRect.origin.y + authorLabelRect.size.height;
    infernalLabelRect.size = CCPoint(400, 35) * __scale;
    m_pInfernalLabel = CALabel::createWithFrame(infernalLabelRect);
    m_pInfernalLabel->setNumberOfLine(1);
    m_pInfernalLabel->setFontSize(24 * __scale);
    m_pInfernalLabel->setColor(fontColor);
    m_pInfernalLabel->setText("无信息...");
    this->addSubview(m_pInfernalLabel);

    
    CCRect statusLabelRect;
    statusLabelRect.origin.x = bgRect.origin.x * 2 + bgRect.size.width;
    statusLabelRect.origin.y = infernalLabelRect.origin.y + infernalLabelRect.size.height;
    statusLabelRect.size = CCPoint(400, 35) * __scale;
    m_pStatusLabel = CALabel::createWithFrame(statusLabelRect);
    m_pStatusLabel->setNumberOfLine(1);
    m_pStatusLabel->setFontSize(24 * __scale);
    m_pStatusLabel->setColor(fontColor);
    m_pStatusLabel->setText("无信息...");
    this->addSubview(m_pStatusLabel);
    
    
    CCRect theLatestLabelRect;
    theLatestLabelRect.origin.x = bgRect.origin.x * 2 + bgRect.size.width;
    theLatestLabelRect.origin.y = statusLabelRect.origin.y + statusLabelRect.size.height;
    theLatestLabelRect.size = CCPoint(400, 35) * __scale;
    m_pTheLatestLabel = CALabel::createWithFrame(theLatestLabelRect);
    m_pTheLatestLabel->setNumberOfLine(1);
    m_pTheLatestLabel->setFontSize(24 * __scale);
    m_pTheLatestLabel->setColor(fontColor);
    m_pTheLatestLabel->setText("无信息...");
    this->addSubview(m_pTheLatestLabel);
    
    
    CCRect addCollectionButtonRect;
    addCollectionButtonRect.origin.x = bgRect.origin.x * 2 + bgRect.size.width;
    addCollectionButtonRect.origin.y = theLatestLabelRect.origin.y + theLatestLabelRect.size.height + 10 * __scale;
    addCollectionButtonRect.size = ccpMult(CCSize(180, 60), __scale);
    m_pAddCollectionButton = CAButton::createWithFrame(addCollectionButtonRect, CAButtonTypeCustom);
    CAImageView* addCollectionNormal = CAImageView::createWithImage(CAImage::create(addCollectionButtonIcon));
    m_pAddCollectionButton->setBackGroundViewForState(CAControlStateAll, addCollectionNormal);
    CAImageView* addCollectionHighlighted = CAImageView::createWithImage(CAImage::create(addCollectionButtonIcon));
    addCollectionHighlighted->setColor(CAColor_gray);
    m_pAddCollectionButton->setBackGroundViewForState(CAControlStateHighlighted, addCollectionHighlighted);
    this->addSubview(m_pAddCollectionButton);
    m_pAddCollectionButton->addTarget(m_pController, CAControl_selector(DMIntroductionController::onAddCollect), CAControlEventTouchUpInSide);
    m_pAddCollectionButton->setTitleColorForState(CAControlStateAll, CAColor_white);
    m_pAddCollectionLable = CALabel::createWithFrame(m_pAddCollectionButton->getBounds());
    m_pAddCollectionLable->setColor(CAColor_white);
    m_pAddCollectionButton->addSubview(m_pAddCollectionLable);
    m_pAddCollectionLable->setFontSize(30 * __scale);
    
    m_pAddCollectionLable->setTextAlignment(CATextAlignmentCenter);
    m_pAddCollectionLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    
    this->m_pController->updateCollectionStatus();
    
    
    CCRect startReadButtonRect = addCollectionButtonRect;
    startReadButtonRect.origin.x = addCollectionButtonRect.origin.x + addCollectionButtonRect.size.width + 26 * __scale;
    m_pStartReadButton = CAButton::createWithFrame(startReadButtonRect, CAButtonTypeCustom);
    CAImageView* startReadNormal = CAImageView::createWithImage(CAImage::create(startReadButtonIcon));
    m_pStartReadButton->setBackGroundViewForState(CAControlStateNormal, startReadNormal);
    CAImageView* startReadHighlighted = CAImageView::createWithImage(CAImage::create(startReadButtonIcon));
    startReadHighlighted->setColor(CAColor_gray);
    m_pStartReadButton->setBackGroundViewForState(CAControlStateHighlighted, startReadHighlighted);
    this->addSubview(m_pStartReadButton);
    m_pStartReadButton->addTarget(m_pController, CAControl_selector(DMIntroductionController::onReadBook), CAControlEventTouchUpInSide);
    
    m_pStartReadLable = CALabel::createWithFrame(m_pAddCollectionButton->getBounds());
    m_pStartReadLable->setColor(CAColor_white);
    m_pStartReadLable->setFontSize(30 * __scale);
    m_pStartReadLable->setTextAlignment(CATextAlignmentCenter);
    m_pStartReadLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_pStartReadButton->addSubview(m_pStartReadLable);
    
    this->updateStartReadButton();
    
    CCRect labelRect;
    labelRect.origin.x = bgRect.origin.x;
    labelRect.origin.y = bgRect.origin.y + bgRect.size.height + 30 * __scale;
    labelRect.size = CADipSize(200, _px(44));
    CALabel* label = CALabel::createWithFrame(labelRect);
    label->setNumberOfLine(1);
    label->setFontSize(_px(26));
    label->setColor(fontColor);
    label->setText("详细介绍");
    this->addSubview(label);
    
    CCRect detailsLabelRect;
    detailsLabelRect.origin.x = labelRect.origin.x;
    detailsLabelRect.origin.y = labelRect.origin.y + labelRect.size.height;
    detailsLabelRect.size.width = CAApplication::getApplication()->getWinSize().width - detailsLabelRect.origin.x * 2;
    detailsLabelRect.size.height = _px(64);
    m_pDetailsLabel = CALabel::createWithFrame(detailsLabelRect);
    m_pDetailsLabel->setFontSize(_px(24));
    m_pDetailsLabel->setColor(fontColor);
    this->addSubview(m_pDetailsLabel);
    
    CCRect openContractionButtonRect;
    openContractionButtonRect.size.width = 150;
    openContractionButtonRect.size.height = 40;
    openContractionButtonRect.origin.x = CAApplication::getApplication()->getWinSize().width - detailsLabelRect.origin.x - 150;
    openContractionButtonRect.origin.y = detailsLabelRect.origin.y + detailsLabelRect.size.height;
    m_pOpenContractionButton = CAButton::createWithFrame(openContractionButtonRect, CAButtonTypeCustom);
    m_pOpenContractionButton->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create(openLabelButtonIcon)));
    m_pOpenContractionButton->setBackGroundViewForState(CAControlStateSelected, CAImageView::createWithImage(CAImage::create(contractionLabelButtonIcon)));
    this->addSubview(m_pOpenContractionButton);
    m_pOpenContractionButton->addTarget(this, CAControl_selector(DMIntroductionHeaderView::changedDetailsLabelState), CAControlEventTouchUpInSide);
    m_pOpenContractionButton->setAllowsSelected(true);
    
    this->setBounds(CCRect(0, 0,
                           CAApplication::getApplication()->getWinSize().width,
                           detailsLabelRect.origin.y + detailsLabelRect.size.height + 60 * __scale));
    return true;
}

void DMIntroductionHeaderView::setIntroductionInfo(const DMIntroductionInfo &var)
{
    m_obIntroductionInfo = var;
    
    m_pBookAuthorLabel->setText(var.authors);
    m_pInfernalLabel->setText("地狱：" + var.zone);
    m_pStatusLabel->setText("状态：" + var.status);
    m_pTheLatestLabel->setText("最新收录：" + var.last_update_chapter_name);
    m_pBookImageView->setUrl(m_obIntroductionInfo.cover);
    
    this->updateDetailsLabelState();
    
}

void DMIntroductionController::updateWanderRecord(int charIndex, bool isList, const std::string& readChapTitme, const std::string& comic_id)
{
    //添加浏览记录
    DMIntroductionInfoManager* dIIMgr = DMIntroductionInfoManager::sharedDMIntroductionInfoManager();
    CCLog("s---- %d", charIndex);
    dIIMgr->updateProgress(charIndex, isList, readChapTitme, comic_id);
}


void DMIntroductionHeaderView::updateStartReadButton()
{
    DMIntroductionInfoManager* dIIMgr = DMIntroductionInfoManager::sharedDMIntroductionInfoManager();
    DMBrowseInfo bInfo = dIIMgr->selectById(m_pController->getID());
    CCLog("g---- %d", bInfo.readIndex);
    if (bInfo.readIndex > -1)
    {
        m_pStartReadLable->setText("继续阅读");
    }
    else
    {
        m_pStartReadLable->setText("开始阅读");
    }
}

void DMIntroductionHeaderView::updateDetailsLabelState()
{
    CCRect detailsLabelRect = m_pDetailsLabel->getFrame();

    std::string des = m_obIntroductionInfo.description;
    if (m_bDetailsLabelShowAll)
    {
        detailsLabelRect.size.height = CAImage::getStringHeight("", _px(24), "　　" + des, detailsLabelRect.size.width);
    }
    else
    {
        
        detailsLabelRect.size.height = MIN(_px(70), CAImage::getStringHeight("", _px(24), "　　" + des, detailsLabelRect.size.width));
    }
    m_pDetailsLabel->setFrame(detailsLabelRect);
    m_pDetailsLabel->setText("　　" + des);

    CCRect openContractionButtonRect = m_pOpenContractionButton->getFrame();
    openContractionButtonRect.origin.y = detailsLabelRect.origin.y + detailsLabelRect.size.height;
    m_pOpenContractionButton->setFrame(openContractionButtonRect);
    
    this->setBounds(CCRect(0, 0,
                           CAApplication::getApplication()->getWinSize().width,
                           detailsLabelRect.origin.y + detailsLabelRect.size.height + _px(60)));
    if (m_pController->m_pCollectionView)
    {
        m_pController->m_pCollectionView->setCollectionHeaderHeight(this->getBounds().size.height);
        m_pController->m_pCollectionView->reloadData();
    }
}

void DMIntroductionHeaderView::changedDetailsLabelState(CAControl* control, CCPoint point)
{
    CAButton* btn = dynamic_cast<CAButton*>(control);
    m_bDetailsLabelShowAll = !btn->isSelected();
    this->updateDetailsLabelState();
}

const DMIntroductionInfo& DMIntroductionHeaderView::getIntroductionInfo()
{
    return m_obIntroductionInfo;
}

#pragma DMIntroductionItemCell

bool DMIntroductionItemCell::initWithReuseIdentifier(const std::string& reuseIdentifier, const CCSize& cellSize)
{
    if (!CACollectionViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }
    CAScale9ImageView* bg = CAScale9ImageView::createWithImage(CAImage::create(cellNormalIcon));
    
    this->setBackgroundView(bg);
    
    CCRect rect;
    rect.origin = cellSize/2;
    rect.size.width = cellSize.width * 0.8f;
    rect.size.height = cellSize.height - 6;
    m_pLabel = CALabel::createWithCenter(rect);
    m_pLabel->setFontSize(_px(26));
    m_pLabel->setNumberOfLine(1);
    m_pLabel->setTextAlignment(CATextAlignmentCenter);
    m_pLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->addSubview(m_pLabel);
    
    m_bHaveDownload = false;
    return true;
}

void DMIntroductionItemCell::setHaveDownload(bool var)
{
    m_bHaveDownload = var;
    if (m_bAllowsSelected == false && m_eControlState == CAControlStateSelected)
    {
        CAControl::setControlState(CAControlStateNormal);
    }
    else
    {
        CAControl::setControlState(m_eControlState);
    }
    
    CC_RETURN_IF(m_bControlStateEffect == false);
    switch (m_eControlState)
    {
        case CAControlStateNormal:
            this->normalCollectionViewCell();
            break;
        case CAControlStateHighlighted:
            this->highlightedCollectionViewCell();
            break;
        case CAControlStateSelected:
            this->selectedCollectionViewCell();
            break;
        case CAControlStateDisabled:
            this->disabledCollectionViewCell();
            break;
        default:
            break;
    }
}

bool DMIntroductionItemCell::isHaveDownload()
{
    return m_bHaveDownload;
}

void DMIntroductionItemCell::setDisabled(bool var)
{
    m_bDisabled = var;
    this->setControlState(m_bDisabled ? CAControlStateDisabled : CAControlStateNormal);
}

bool DMIntroductionItemCell::isDisabled()
{
    return m_bDisabled;
}

void DMIntroductionItemCell::normalCollectionViewCell()
{
    CAScale9ImageView* bg = dynamic_cast<CAScale9ImageView*>(this->getBackgroundView());
    CC_RETURN_IF(bg == NULL);
    if (m_bHaveDownload)
    {
        bg->setImage(CAImage::create(cellHaveDowloadIcon));
        m_pLabel->setColor(CAColor_white);
    }
    else
    {
        bg->setImage(CAImage::create(cellNormalIcon));
        m_pLabel->setColor(CAColor_black);
    }
    
}

void DMIntroductionItemCell::highlightedCollectionViewCell()
{
    CAScale9ImageView* bg = dynamic_cast<CAScale9ImageView*>(this->getBackgroundView());
    CC_RETURN_IF(bg == NULL);
    bg->setImage(CAImage::create(cellSelectedIcon));
    m_pLabel->setColor(CAColor_white);
}

void DMIntroductionItemCell::selectedCollectionViewCell()
{
    CAScale9ImageView* bg = dynamic_cast<CAScale9ImageView*>(this->getBackgroundView());
    CC_RETURN_IF(bg == NULL);
    bg->setImage(CAImage::create(cellSelectedIcon));
    m_pLabel->setColor(CAColor_white);
}

void DMIntroductionItemCell::disabledCollectionViewCell()
{
    CAScale9ImageView* bg = dynamic_cast<CAScale9ImageView*>(this->getBackgroundView());
    CC_RETURN_IF(bg == NULL);
    bg->setImage(CAImage::create(cellNormalIcon));
    m_pLabel->setColor(CAColor_gray);
}

#pragma DMIntroductionSimilarItemCell

bool DMIntroductionSimilarItemCell::initWithReuseIdentifier(const std::string& reuseIdentifier, const CCSize& cellSize)
{
    if (!CACollectionViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }
    this->setBackgroundView(NULL);

    CCRect bgRect;
    bgRect.origin = CCPointZero;
    bgRect.size = cellSize;
    bgRect.size.height = bgRect.size.width * 4/3.0f;
    CAScale9ImageView* bg = CAScale9ImageView::createWithImage(CAImage::create(book_bg));
    bg->setFrame(bgRect);
    this->addSubview(bg);
    
    CCRect imageViewRect;
    imageViewRect.origin = CCPoint(2, 2);
    imageViewRect.size = ccpSub(bgRect.size, CCSize(4, 4));
    m_pImageView = CommonUrlImageView::createWithFrame(imageViewRect);
    this->addSubview(m_pImageView);
    m_pImageView->setImage(CAImage::create("dm_resource/1.png"));
    
    CCRect labelRect;
    labelRect.size = CCSize(cellSize.width, cellSize.width / 3.0f);
    labelRect.origin = CCPoint(0, cellSize.height - labelRect.size.height);
    m_pLabel = CALabel::createWithFrame(labelRect);
    m_pLabel->setColor(ccc4(153, 153, 153, 255));
    m_pLabel->setNumberOfLine(1);
    m_pLabel->setFontSize(_px(24));
    m_pLabel->setTextAlignment(CATextAlignmentCenter);
    m_pLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentBottom);
    this->addSubview(m_pLabel);
    
    return true;
}


void DMIntroductionSimilarItemCell::normalCollectionViewCell()
{

}

void DMIntroductionSimilarItemCell::highlightedCollectionViewCell()
{

}

void DMIntroductionSimilarItemCell::selectedCollectionViewCell()
{

}

#pragma DMCommentCell

bool DMCommentCell::initWithReuseIdentifier(const std::string& reuseIdentifier, const CCSize& cellSize)
{
    
    if (!CACollectionViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }
    
    //头像
    CommonUrlImageView *head = CommonUrlImageView::createWithFrame(CCRect(_px(20), _px(30), _px(70), _px(70)));
    setHeadView(head);
    this->addSubview(head);
    //评论主题
    CALabel *title = CALabel::createWithFrame(CCRect(_px(110), _px(30), cellSize.width -_px(130), _px(35)));
    title->setFontSize(_px(30));
    title->setColor(ccc4(0, 115, 241, 255));
    setTopName(title);
    this->addSubview(title);
    //日期
    CALabel *date = CALabel::createWithFrame(CCRect(_px(110), _px(65), _px(150), _px(35)));
    date->setFontSize(_px(24));
    date->setColor(ccc4(153, 153, 153, 255));
    setDateView(date);
    this->addSubview(date);
    //主评论区
    CALabel *mainCont = CALabel::createWithFrame(CCRect(0, 0, 0, 0));
    mainCont->setFontSize(_px(28));
    mainCont->setColor(ccc4(0, 0, 0, 255));
    setMainComment(mainCont);
    this->addSubview(mainCont);
    //回复
    CAButton * feedBackV = CAButton::create(CAButtonTypeCustom);
    feedBackV->setFrame(CCRect(cellSize.height-_px(65), cellSize.width-_px(140), _px(110), _px(40)));
    CAImageView *feedIcon = CAImageView::createWithFrame(CCRect(0, 0, _px(30), _px(30)));
    feedIcon->setImage(CAImage::create(feedbackicon));
    feedBackV->addSubview(feedIcon);
    CALabel *feedInst = CALabel::createWithFrame(CCRect(_px(35), 0, _px(60), _px(40)));
    feedInst->setFontSize(_px(25));
    feedInst->setText("回复");
    feedInst->setColor(ccc4(153, 153, 153, 255));
    feedBackV->addSubview(feedInst);
    setBtnFeedBack(feedBackV);
    this->addSubview(feedBackV);
    
    //子评论区
    CAView *subviewLay = CAView::create();
    subviewLay->setColor(ccc4(239, 239, 239, 255));
    setSubViewLayout(subviewLay);
    this->addSubview(subviewLay);
    return true;
}

void DMCommentCell::normalCollectionViewCell()
{
}

void DMCommentCell::highlightedCollectionViewCell()
{
}

void DMCommentCell::selectedCollectionViewCell()
{
}


void DMCommentCell::disabledCollectionViewCell()
{
}








