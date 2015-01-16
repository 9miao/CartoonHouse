

#include "DMClassifCatalogViewController.h"
#include "DMCatalogCollectionCell.h"
#include "DMIntroductionController.h"

DMClassifCatalogViewController::DMClassifCatalogViewController(const std::string& ID)
: m_pCollectionView(NULL)
, m_sID(ID)
, m_nDataPage(0)
, m_uiItems(0)
, m_pLoading(NULL)
, m_bFooterRefreshing(true)
{
    
}

DMClassifCatalogViewController::~DMClassifCatalogViewController()
{
    
}

void DMClassifCatalogViewController::viewDidLoad()
{
    CADipRect rect = this->getView()->getBounds();
    rect.size.height = 96;
    m_uiItems = (unsigned int)(rect.size.width / 300);
    m_uiItems = MIN(m_uiItems, 5);
    m_uiItems = MAX(m_uiItems, 3);
    
    CAView* bar = CAView::createWithFrame(rect, ccc4(87, 154, 244, 255));
    this->getView()->insertSubview(bar, 1);
    
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
    
    CCRect btnRect;
    btnRect.size.width = rect.size.height * 0.9f;
    btnRect.size.height = rect.size.height * 0.8f;
    btnRect.origin.x = btnRect.size.width * 0.7f;
    btnRect.origin.y = rect.size.height * 0.5f;
    CAButton* button = CAButton::createWithCenter(btnRect, CAButtonTypeCustom);
    button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
    button->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
    bar->addSubview(button);
    button->addTarget(this, CAControl_selector(DMIntroductionController::onReturn), CAControlEventTouchUpInSide);
    
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeClassifCatalog, "update", m_sID, "", m_vDatas.size());
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMClassifCatalogViewController::onRequestFinished));
    
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
        m_pLoading->setLoadingMinTime(0.4f);
        m_pLoading->setColor(CAColor_white);
        this->getView()->insertSubview(m_pLoading, CAWindowZoderTop);
        m_pLoading->setTargetOnCancel(this, callfunc_selector(DMClassifCatalogViewController::initWithCollectionView));
    }
}

void DMClassifCatalogViewController::viewDidUnload()
{
    
}

void DMClassifCatalogViewController::initWithCollectionView()
{
    CADipRect collectionRect = this->getView()->getBounds();
    collectionRect.origin.y = 96;
    collectionRect.size.height -= 96;
    m_pCollectionView = CACollectionView::createWithFrame(collectionRect);
    m_pCollectionView->setCollectionViewDataSource(this);
    m_pCollectionView->setCollectionViewDelegate(this);
    m_pCollectionView->setScrollViewDelegate(this);
    CAPullToRefreshView* viewHeader = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    m_pCollectionView->setHeaderRefreshView(viewHeader);
    CAPullToRefreshView* viewFooter = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    m_pCollectionView->setFooterRefreshView(viewFooter);
    this->getView()->addSubview(m_pCollectionView);
    m_pCollectionView->setAllowsSelection(true);
    m_pCollectionView->setHoriInterval(_px(30));
    m_pCollectionView->setVertInterval(_px(10));
}

void DMClassifCatalogViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        if (!m_bFooterRefreshing)
        {
            m_vDatas.clear();
        }
        
        const CSJson::Value& info = json["data"];
        
        for (int i=0; i<info.size(); i++)
        {
            const CSJson::Value& value = info[i];
            m_vDatas.push_back(DMBriefInfo(value["id"].asString(),
                                      value["cover"].asString(),
                                      value["title"].asString(),
                                      value["last_update_chapter_name"].asString()));
        }

        
    }
    else
    {
        
    }
    
    do
    {
        CC_BREAK_IF(m_pLoading == NULL);
        if (m_pLoading->isAnimating())
        {
            m_pLoading->stopAnimating();
        }
        else
        {
            m_pCollectionView->reloadData();
        }
    }
    while (0);
}

void DMClassifCatalogViewController::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
    m_bFooterRefreshing = false;
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeClassifCatalog, "update", m_sID, "", 0);
    CommonHttpManager::getInstance()->send_post(s_http_url.url,
                                               s_http_url.key_value,
                                               this,
                                               CommonHttpJson_selector(DMClassifCatalogViewController::onRequestFinished));
}

void DMClassifCatalogViewController::scrollViewFooterBeginRefreshing(CrossApp::CAScrollView *view)
{
    m_bFooterRefreshing = true;
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeClassifCatalog, "update", m_sID, "", m_vDatas.size());
    CommonHttpManager::getInstance()->send_post(s_http_url.url,
                                                s_http_url.key_value,
                                               this,
                                               CommonHttpJson_selector(DMClassifCatalogViewController::onRequestFinished));
}

CACollectionViewCell* DMClassifCatalogViewController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    DMCatalogCollectionCell* cell = cell = (DMCatalogCollectionCell*)collectionView->dequeueReusableCellWithIdentifier("ID");
    if (cell == NULL)
    {
        cell = new DMCatalogCollectionCell();
        cell->init("ID", cellSize);
        cell->autorelease();
    }
    
    if (row * m_uiItems + item < m_vDatas.size())
    {
        const DMBriefInfo& info = m_vDatas.at(row * m_uiItems + item);
        
        cell->setBookNameText(info.title);
        cell->setInfoText(info.subtitle);
        cell->setBookImageFromUrl(info.cover);
    }
    else
    {
        cell->setVisible(false);
    }
    
    return cell;
}

unsigned int DMClassifCatalogViewController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    int size = (int) m_vDatas.size();
    return size % m_uiItems > 0 ? (size / m_uiItems + 1) : (size / m_uiItems);
}

unsigned int DMClassifCatalogViewController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return m_uiItems;
}

unsigned int DMClassifCatalogViewController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    float width = (this->getView()->getBounds().size.width - ((int)m_uiItems + 1) * _px(30)) / m_uiItems;
    
    return width * 25 / 16.0f;
}

void DMClassifCatalogViewController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    if (m_vDatas.size() > row * m_uiItems + item)
    {
        const DMBriefInfo& info = m_vDatas.at(row * m_uiItems + item);
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(DMIntroductionController::create(info.ID, info.title), true);
    }
}

void DMClassifCatalogViewController::onButton(CAControl* control, CCPoint point)
{

}