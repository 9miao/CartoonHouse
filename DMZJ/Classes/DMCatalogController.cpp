

#include "DMCatalogController.h"
#include "DMCatalogCollectionCell.h"
#include "DMIntroductionController.h"

static std::map<std::string, std::vector<DMBriefInfo> > m_mvData = std::map<std::string, std::vector<DMBriefInfo> >();

DMCatalogController::DMCatalogController()
: m_pCollectionView(NULL)
, m_uiItems(3)
, m_pLoading(NULL)
{

}

DMCatalogController::~DMCatalogController()
{
    
}

void DMCatalogController::viewDidLoad()
{
    if (m_mvData.empty())
    {
        HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeRecommend);
        CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMCatalogController::onRequestFinished));
        
        {
            CCRect loadingRect = this->getView()->getBounds();
            m_pLoading = CAActivityIndicatorView::createWithFrame(loadingRect);
            CAImageView* indicator = CAImageView::createWithFrame(CADipRect(0, 0, 50, 50));
            indicator->setImage(CAImage::create(loadingIcon));
            m_pLoading->setActivityIndicatorView(indicator);
            CAView* bg = CAView::createWithFrame(CADipRect(0, 0, 275, 300), CAColor_clear);
            CAImageView* bg2 = CAImageView::createWithFrame(CADipRect(0, 0, 275, 100));
            bg2->setImage(CAImage::create(loadingBackground));
            bg->addSubview(bg2);
            m_pLoading->setActivityBackView(bg);
            m_pLoading->setColor(CAColor_white);
            this->getView()->insertSubview(m_pLoading, CAWindowZoderTop);
            m_pLoading->setTargetOnCancel(this, callfunc_selector(DMCatalogController::initWithCollectionView));
        }
    }
    else
    {
        this->initWithCollectionView();
    }
}

void DMCatalogController::viewDidUnload()
{
    
}

void DMCatalogController::initWithCollectionView()
{
    CADipRect rect = this->getView()->getBounds();
    m_uiItems = (unsigned int)(rect.size.width / 300);
    m_uiItems = MIN(m_uiItems, 5);
    m_uiItems = MAX(m_uiItems, 3);
    
    m_pCollectionView = CACollectionView::createWithFrame(rect);
    m_pCollectionView->setCollectionViewDataSource(this);
    m_pCollectionView->setCollectionViewDelegate(this);
    m_pCollectionView->setScrollViewDelegate(this);
    CAPullToRefreshView* view = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    m_pCollectionView->setHeaderRefreshView(view);
    m_pCollectionView->setBackGroundColor(CAColor_clear);
    this->getView()->addSubview(m_pCollectionView);
    m_pCollectionView->setAllowsSelection(true);
    m_pCollectionView->setHoriInterval(_px(30));
    m_pCollectionView->setVertInterval(_px(10));
    
    float width = this->getView()->getBounds().size.width;
    m_pCollectionView->setCollectionHeaderHeight(width * 15/32.0f);
    
    CAButton* bg = CAButton::createWithFrame(CADipRect(0, 0, rect.size.width, _dip(m_pCollectionView->getCollectionHeaderHeight())), CAButtonTypeCustom);
    m_pHeaderImageView = CommonUrlImageView::createWithImage(NULL);
    bg->setBackGroundViewForState(CAControlStateNormal, m_pHeaderImageView);
    bg->addTarget(this, CAControl_selector(DMCatalogController::onButton), CAControlEventTouchUpInSide);
    m_pCollectionView->setCollectionHeaderView(bg);
    
    CADipRect headerRect = CADipRectZero;
    headerRect.size = rect.size;
    headerRect.size.height = 76;
    headerRect.origin.y = _dip(m_pCollectionView->getCollectionHeaderHeight()) - 76;
    CAImageView* imageView = CAImageView::createWithFrame(headerRect);
    imageView->setImage(CAImage::create(title_bg_first));
    bg->insertSubview(imageView,1);
    
    headerRect.origin.x = 25;
    headerRect.size.height -= 20;
    headerRect.size.width -= headerRect.origin.x;
    headerRect.origin.y = _dip(m_pCollectionView->getCollectionHeaderHeight()) - headerRect.size.height;
    m_pHeaderLabel = CALabel::createWithFrame(headerRect);
    m_pHeaderLabel->setFontSize(_px(28));
    m_pHeaderLabel->setColor(CAColor_white);
    m_pHeaderLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    bg->insertSubview(m_pHeaderLabel,1);
    
    if (!m_mvData[m_sTitle].empty())
    {
        const DMBriefInfo& info = m_mvData[m_sTitle].front();
        m_pHeaderLabel->setText("《" + info.title + "》" + info.subtitle);
        m_pHeaderImageView->setUrl(info.cover);
    }
}

void DMCatalogController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        m_mvData.clear();

        for (int i=0; i<SUBSCRIBE_SECTION_COUNT; i++)
        {
            std::vector<DMBriefInfo> vec;
            const CSJson::Value& info = json[catalogTitleTag[i]];
            const CSJson::Value& headlines = info["headlines"];
            vec.push_back(DMBriefInfo(headlines["id"].asString(),
                                      headlines["cover"].asString(),
                                      headlines["title"].asString(),
                                      headlines["subtitle"].asString()));
            const CSJson::Value& episode = info["episode"];
            for (int j=0; j<episode.size(); j++)
            {
                const CSJson::Value& value = episode[j];
                vec.push_back(DMBriefInfo(value["id"].asString(),
                                          value["cover"].asString(),
                                          value["title"].asString(),
                                          value["last_update_chapter_name"].asString()));
            }
            
            m_mvData[catalogTitleTag[i]]= vec;
        }
        if (m_pCollectionView)
        {
            m_pCollectionView->reloadData();
            const DMBriefInfo& info = m_mvData[m_sTitle].front();
            m_pHeaderLabel->setText("《" + info.title + "》" + info.subtitle);
            m_pHeaderImageView->setUrl(info.cover);
        }
        
        CATabBarController* tabBarController = this->getTabBarController();
        
        for (int i=0; i<6; i++)
        {
            DMCatalogController* catalog = dynamic_cast<DMCatalogController*>(tabBarController->getViewControllerAtIndex(i));
            if (catalog && catalog->m_pLoading)
            {
                catalog->m_pLoading->stopAnimating();
            }
        }
    }
    else
    {

    }
    
}

void DMCatalogController::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeRecommend);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMCatalogController::onRequestFinished));
}

CACollectionViewCell* DMCatalogController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    if (row * m_uiItems + item + 1 >= m_mvData[m_sTitle].size())
    {
        return NULL;
    }
    
    DMCatalogCollectionCell* cell = cell = (DMCatalogCollectionCell*)collectionView->dequeueReusableCellWithIdentifier("ID");
    if (cell == NULL)
    {
        cell = new DMCatalogCollectionCell();
        cell->init("ID", cellSize);
        cell->autorelease();
    }

    if (m_mvData.count(m_sTitle) > 0)
    {
        const DMBriefInfo& info = m_mvData[m_sTitle].at(row * m_uiItems + item + 1);
        
        cell->setBookNameText(info.title);
        cell->setInfoText(info.subtitle);
        cell->setBookImageFromUrl(info.cover);
    }
    
    return cell;
}

unsigned int DMCatalogController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    if (m_mvData.count(m_sTitle) > 0)
    {
        int size = (int) m_mvData[m_sTitle].size() - 1;
        return (size / m_uiItems);
    }
    return 0;
}

unsigned int DMCatalogController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return m_uiItems;
}

unsigned int DMCatalogController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    float width = (this->getView()->getBounds().size.width - ((int)m_uiItems + 1) * _px(30)) / m_uiItems;
    
    return width * 25 / 16.0f;
}

void DMCatalogController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    if (m_mvData[m_sTitle].size() > row * m_uiItems + item + 1)
    {
        const DMBriefInfo& info = m_mvData[m_sTitle].at(row * m_uiItems + item + 1);
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(DMIntroductionController::create(info.ID, info.title), true);
    }
}

void DMCatalogController::onButton(CAControl* control, CCPoint point)
{
    if (!m_mvData[m_sTitle].empty())
    {
        const DMBriefInfo& info = m_mvData[m_sTitle].front();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(DMIntroductionController::create(info.ID, info.title), true);
    }
}
