

#include "DMSelectionDownloadController.h"
#include "DMIntroductionController.h"
#include "DMSearchController.h"
#include "DMDownloadController.h"

DMSelectionDownloadController::DMSelectionDownloadController(const DMIntroductionInfo& info,
                                                             const std::vector<DMIntroductionItemInfo>& listItemInfos,
                                                             const std::vector<DMIntroductionItemInfo>& aloneItemInfos)
: m_obListItemInfos(listItemInfos)
, m_obAloneItemInfos(aloneItemInfos)
, m_obInfo(info)
, m_pDownloadInfo(NULL)
{
    this->setTitle(info.title);
    
    this->setDownloadInfo(DMDownloadBookInfo::create());
    for (std::vector<DMIntroductionItemInfo>::iterator itr=m_obListItemInfos.begin();
         itr!=m_obListItemInfos.end(); itr++)
    {
        std::string url = DMDownloadBookInfo::getDownloadUrl(m_obInfo, *itr);
        m_pDownloadInfo->m_obListItemInfoMaps[url] = *itr;
    }
    for (std::vector<DMIntroductionItemInfo>::iterator itr=m_obAloneItemInfos.begin();
         itr!=m_obAloneItemInfos.end(); itr++)
    {
        std::string url = DMDownloadBookInfo::getDownloadUrl(m_obInfo, *itr);
        m_pDownloadInfo->m_obAloneItemInfoMaps[url] = *itr;
    }
    
    this->getDownloadInfo()->m_obFilePathTextTag = DMDownloadBookInfo::getFilePathTextTag(m_obInfo);
    
    m_pSectionView[0] = 0;
    m_pSectionView[1] = 0;
    
    float width = _dip(CAApplication::getApplication()->getWinSize().width);
    m_uiItems = MIN(8, (width - 20) / 150);
}

DMSelectionDownloadController::~DMSelectionDownloadController()
{
    CC_SAFE_RELEASE(m_pSectionView[0]);
    CC_SAFE_RELEASE(m_pSectionView[1]);
    CC_SAFE_RELEASE(m_pDownloadInfo);
    m_obListItemInfos.clear();
    m_obAloneItemInfos.clear();
}

void DMSelectionDownloadController::viewDidLoad()
{
    CADipRect rect = this->getView()->getBounds();
    
    {
        CADipRect barRect = rect;
        barRect.size.height = 96;
        CAView* bar = CAView::createWithFrame(barRect, ccc4(87, 154, 244, 255));
        this->getView()->addSubview(bar);
        
        CADipRect titleRect;
        titleRect.origin = barRect.size/2;
        titleRect.size.width = barRect.size.width * 0.5f;
        titleRect.size.height = barRect.size.height;
        CALabel* label = CALabel::createWithCenter(titleRect);
        label->setNumberOfLine(1);
        label->setFontSize(_px(32));
        label->setColor(CAColor_white);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setText(m_sTitle);
        bar->addSubview(label);
        
        CADipRect leftRect = barRect;
        leftRect.size.width = leftRect.size.height * 0.9f;
        leftRect.size.height *= 0.8f;
        leftRect.origin.x = leftRect.size.width * 0.7f;
        leftRect.origin.y = barRect.size.height / 2;
        CAButton* button = CAButton::createWithCenter(leftRect, CAButtonTypeCustom);
        button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
        button->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        bar->addSubview(button);
        button->addTarget(this, CAControl_selector(DMSelectionDownloadController::onReturn), CAControlEventTouchUpInSide);
        
        CADipRect rightRect = barRect;
        rightRect.size.width = rightRect.size.height * 0.9f;
        rightRect.size.height *= 0.8f;
        rightRect.origin.x = barRect.size.width - rightRect.size.width * 0.7f;
        rightRect.origin.y = barRect.size.height / 2;
        CAButton* button2 = CAButton::createWithCenter(rightRect, CAButtonTypeCustom);
        button2->setImageForState(CAControlStateNormal, CAImage::create(searchIcon));
        button2->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
        bar->addSubview(button2);
        button2->addTarget(this, CAControl_selector(DMSelectionDownloadController::onSearch), CAControlEventTouchUpInSide);
    }
    
    
    {
        CADipRect rect1;
        rect1.origin.x = rect.size.width / 4.0f;
        rect1.origin.y = 55 + 96;
        rect1.size.width = 180;
        rect1.size.height = 60;
        CAButton* allSelection = CAButton::createWithCenter(rect1, CAButtonTypeCustom);
        CAImageView* allSelectionBg1 = CAImageView::createWithImage(CAImage::create(allSelectionButtonIcon));
        CAImageView* allSelectionBg2 = CAImageView::createWithImage(CAImage::create(allSelectionButtonIcon));
        allSelectionBg2->setColor(CAColor_gray);
        allSelection->setBackGroundViewForState(CAControlStateNormal, allSelectionBg1);
        allSelection->setBackGroundViewForState(CAControlStateHighlighted, allSelectionBg2);
        this->getView()->addSubview(allSelection);
        allSelection->addTarget(this, CAControl_selector(DMSelectionDownloadController::onAllSelection), CAControlEventTouchUpInSide);
        allSelection->setTextTag("no");
        
        CADipRect rect2 = rect1;
        rect2.origin.x = rect.size.width * 3 / 4.0f;
        CAButton* download = CAButton::createWithCenter(rect2, CAButtonTypeCustom);
        CAImageView* downloadBg1 = CAImageView::createWithImage(CAImage::create(downloadButtonIcon));
        CAImageView* downloadBg2 = CAImageView::createWithImage(CAImage::create(downloadButtonIcon));
        downloadBg2->setColor(CAColor_gray);
        download->setBackGroundViewForState(CAControlStateNormal, downloadBg1);
        download->setBackGroundViewForState(CAControlStateHighlighted, downloadBg2);
        this->getView()->addSubview(download);
        download->addTarget(this, CAControl_selector(DMSelectionDownloadController::onDownload), CAControlEventTouchUpInSide);
        
        CADipRect lineRect = rect;
        lineRect.origin.y = 108 + 96;
        lineRect.size.height = _dip(1);
        CAView* line = CAView::createWithFrame(lineRect, CAColor_gray);
        this->getView()->addSubview(line);
        
        CADipRect rect3 = rect;
        rect3.origin.y = 110 + 96;
        rect3.size.height -= rect3.origin.y;
        m_pCollectionView = CACollectionView::createWithFrame(rect3);
        m_pCollectionView->setCollectionViewDataSource(this);
        m_pCollectionView->setCollectionViewDelegate(this);
        m_pCollectionView->setAllowsSelection(true);
        m_pCollectionView->setAllowsMultipleSelection(true);
        m_pCollectionView->setBackGroundColor(ccc4(244, 244, 244, 255));
        m_pCollectionView->setHoriInterval(_px(20));
        m_pCollectionView->setVertInterval(_px(20));
        this->getView()->addSubview(m_pCollectionView);
    }
    
}

void DMSelectionDownloadController::viewDidUnload()
{
    
}

void DMSelectionDownloadController::viewDidAppear()
{
    this->getDownloadInfo()->m_obDownloadFinishedListUrls.clear();
    this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.clear();
    this->getDownloadInfo()->m_obDownloadPausesUrls.clear();
    this->getDownloadInfo()->m_obDownloadingUrls.clear();
    
    std::vector<unsigned long> vec = CADownloadManager::getInstance()->getDownloadIdsFromTextTag(this->getDownloadInfo()->m_obFilePathTextTag);
    for (int i=0; i<vec.size(); i++)
    {
        unsigned long id = vec.at(i);
        const char* url = CADownloadManager::getInstance()->getDownloadUrl(id);
        CC_BREAK_IF(url == NULL);
        m_pDownloadInfo->m_obItemInfoIDMaps[url] = id;
        if (CADownloadManager::getInstance()->isFinished(id))
        {
            
            if (m_pDownloadInfo->m_obListItemInfoMaps.count(url))
            {
                this->getDownloadInfo()->m_obDownloadFinishedListUrls.push_back(url);
            }
            else
            {
                this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.push_back(url);
            }
        }
        else if (CADownloadManager::getInstance()->isDownloading(id))
        {
            this->getDownloadInfo()->m_obDownloadingUrls.push_back(url);
        }
        else
        {
            this->getDownloadInfo()->m_obDownloadPausesUrls.push_back(url);
        }
    }
    
    m_pCollectionView->reloadData();
}

void DMSelectionDownloadController::viewDidDisappear()
{

}

void DMSelectionDownloadController::onReturn(CAControl* control, CCPoint point)
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}

void DMSelectionDownloadController::onSearch(CAControl* control, CCPoint point)
{
    DMSearchController* searchController = DMSearchController::create();
    this->presentModalViewController(searchController, true);
}

void DMSelectionDownloadController::onAllSelection(CAControl* control, CCPoint point)
{
    CAButton* btn = dynamic_cast<CAButton*>(control);
    if (btn->getTextTag().compare("no") == 0)
    {
        btn->setTextTag("yes");
        
        CAImageView* allSelectionBg1 = CAImageView::createWithImage(CAImage::create(allUnSelectionButtonIcon));
        CAImageView* allSelectionBg2 = CAImageView::createWithImage(CAImage::create(allUnSelectionButtonIcon));
        allSelectionBg2->setColor(CAColor_gray);
        btn->setBackGroundViewForState(CAControlStateNormal, allSelectionBg1);
        btn->setBackGroundViewForState(CAControlStateHighlighted, allSelectionBg2);
        
        
        this->getDownloadInfo()->m_obWillDownloadUrls.clear();
        for (int a=0; a<2; a++)
        {
            const std::vector<DMIntroductionItemInfo>& vec = (a == 0 ? m_obListItemInfos : m_obAloneItemInfos);
            CC_CONTINUE_IF(vec.empty());
            int size = 0;
            if (vec.size() % m_uiItems == 0)
            {
                size =  vec.size() / m_uiItems;
            }
            else
            {
                size = vec.size() / m_uiItems + 1;
            }
            
            for (int i=0; i<size; i++)
            {
                for (int j=0; j<m_uiItems; j++)
                {
                    CC_CONTINUE_IF(i * m_uiItems + j >= vec.size());
                    const DMIntroductionItemInfo& info = vec.at(i * m_uiItems + j);
                    std::string url = DMDownloadBookInfo::getDownloadUrl(m_obInfo, info);
                    CC_CONTINUE_IF(this->isDisabledFromUrl(url));
                    this->getDownloadInfo()->m_obWillDownloadUrls.push_back(url);
                    m_pCollectionView->setSelectRowAtIndexPath(a, i, j);
                }
            }
        }
    }
    else
    {
        btn->setTextTag("no");
        
        CAImageView* allSelectionBg1 = CAImageView::createWithImage(CAImage::create(allSelectionButtonIcon));
        CAImageView* allSelectionBg2 = CAImageView::createWithImage(CAImage::create(allSelectionButtonIcon));
        allSelectionBg2->setColor(CAColor_gray);
        btn->setBackGroundViewForState(CAControlStateNormal, allSelectionBg1);
        btn->setBackGroundViewForState(CAControlStateHighlighted, allSelectionBg2);
        
        for (int a=0; a<2; a++)
        {
            const std::vector<DMIntroductionItemInfo>& vec = a == 0 ? m_obListItemInfos : m_obAloneItemInfos;
            CC_CONTINUE_IF(vec.empty());
            int size = 0;
            if (vec.size() % m_uiItems == 0)
            {
                size =  vec.size() / m_uiItems;
            }
            else
            {
                size = vec.size() / m_uiItems + 1;
            }
            
            for (int i=0; i<size; i++)
            {
                for (int j=0; j<m_uiItems; j++)
                {
                    m_pCollectionView->setUnSelectRowAtIndexPath(a, i, j);
                }
            }
        }
        this->getDownloadInfo()->m_obWillDownloadUrls.clear();
    }
}

void DMSelectionDownloadController::onDownload(CAControl* control, CCPoint point)
{
    DMDownloadController* controller = new DMDownloadController(m_obInfo, this->getDownloadInfo());
    control->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
}

void DMSelectionDownloadController::onSort(CAControl* control, CCPoint point)
{
    std::reverse(m_obListItemInfos.begin(), m_obListItemInfos.end());
    std::reverse(m_obAloneItemInfos.begin(), m_obAloneItemInfos.end());
    m_pCollectionView->reloadData();
    this->getDownloadInfo()->m_obWillDownloadUrls.clear();
}

bool DMSelectionDownloadController::isDisabledFromUrl(const std::string& url)
{
    for (std::vector<std::string>::iterator itr = this->getDownloadInfo()->m_obDownloadFinishedListUrls.begin();
         itr!=this->getDownloadInfo()->m_obDownloadFinishedListUrls.end(); itr++)
    {
        if (itr->compare(url) == 0)
            return true;
    }
    for (std::vector<std::string>::iterator itr = this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.begin();
         itr!=this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.end(); itr++)
    {
        if (itr->compare(url) == 0)
            return true;
    }
    for (std::vector<std::string>::iterator itr = this->getDownloadInfo()->m_obDownloadPausesUrls.begin();
         itr!=this->getDownloadInfo()->m_obDownloadPausesUrls.end(); itr++)
    {
        if (itr->compare(url) == 0)
            return true;
    }
    for (std::vector<std::string>::iterator itr = this->getDownloadInfo()->m_obDownloadingUrls.begin();
         itr!=this->getDownloadInfo()->m_obDownloadingUrls.end(); itr++)
    {
        if (itr->compare(url) == 0)
            return true;
    }
    
    return false;
}

unsigned int DMSelectionDownloadController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return _px(70);
}

CACollectionViewCell* DMSelectionDownloadController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = NULL;

    const std::vector<DMIntroductionItemInfo>& vec = section == 0 ? m_obListItemInfos : m_obAloneItemInfos;
    
    if (row * m_uiItems + item < vec.size())
    {
        cell = m_pCollectionView->dequeueReusableCellWithIdentifier("item");
        
        if (cell == NULL)
        {
            cell = new DMIntroductionItemCell();
            ((DMIntroductionItemCell*)cell)->initWithReuseIdentifier("item", cellSize);
            cell->autorelease();
        }

        DMIntroductionItemCell* itemCell = dynamic_cast<DMIntroductionItemCell*>(cell);
        const DMIntroductionItemInfo& info = vec.at(row * m_uiItems + item);
        itemCell->getLabel()->setText(info.chapter_name);
        
        std::string url = DMDownloadBookInfo::getDownloadUrl(m_obInfo, info);
        itemCell->setDisabled(this->isDisabledFromUrl(url));
    }

    return cell;
}

unsigned int DMSelectionDownloadController::numberOfSections(CACollectionView *collectionView)
{
    return m_obAloneItemInfos.empty() ? 1 : 2;
}

unsigned int DMSelectionDownloadController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    const std::vector<DMIntroductionItemInfo>& vec = section == 0 ? m_obListItemInfos : m_obAloneItemInfos;
    if (vec.size() % m_uiItems == 0)
    {
        return vec.size() / m_uiItems;
    }
    else
    {
        return vec.size() / m_uiItems + 1;
    }
}

unsigned int DMSelectionDownloadController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return m_uiItems;
}


unsigned int DMSelectionDownloadController::collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
{
    return _px(70);
}


CAView* DMSelectionDownloadController::collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
{
    if (m_pSectionView[section] == NULL)
    {
        m_pSectionView[section] = CAView::createWithColor(ccc4(244, 244, 244, 255));
        m_pSectionView[section]->retain();
        //    CCRect lineRect;
        //    lineRect.size.width = viewSize.width - 40 * __scale;
        //    lineRect.size.height = 1;
        //    lineRect.origin.x = 20 * __scale;
        //    lineRect.origin.y = viewSize.height - lineRect.size.height;
        //    CAView* line = CAView::createWithFrame(lineRect, CAColor_gray);
        //    view->addSubview(line);
        
        float winSizeWidth = _dip(viewSize.width);
        
        CADipRect rect = CADipRect(150, 45, 260, 70);
        CALabel* label = CALabel::createWithCenter(rect);
        label->setFontSize(_px(24));
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setColor(ccc4(153, 153, 153, 255));
        m_pSectionView[section]->addSubview(label);
        
        char text[32];
        if (section == 0)
        {
            sprintf(text, "连载(%lu话)", m_obListItemInfos.size());
        }
        else
        {
            sprintf(text, "单行本(%lu话)", m_obAloneItemInfos.size());
        }
        
        label->setText(text);
        
        if (section == 0)
        {
            CADipRect rect1 = CADipRect(winSizeWidth - 250, 45, 200, 70);
            CALabel* label = CALabel::createWithCenter(rect1);
            label->setFontSize(_px(24));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(153, 153, 153, 255));
            label->setText("排序方式:");
            m_pSectionView[section]->addSubview(label);
            
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
            m_pSectionView[section]->addSubview(control);
            control->addTarget(this, CAControl_selector(DMSelectionDownloadController::onSort));
        }

    }
    return m_pSectionView[section];
}


static bool compareIntroduction(const std::string& itr1, const std::string& itr2)
{
    return itr1.c_str() < itr2.c_str() == false;
}

void DMSelectionDownloadController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    const std::vector<DMIntroductionItemInfo>& vec = section == 0 ? m_obListItemInfos : m_obAloneItemInfos;
    std::string url = DMDownloadBookInfo::getDownloadUrl(m_obInfo, vec.at(row * m_uiItems + item));
    for (std::vector<std::string>::iterator itr=this->getDownloadInfo()->m_obWillDownloadUrls.begin();
         itr!=this->getDownloadInfo()->m_obWillDownloadUrls.end(); itr++)
    {
        CC_RETURN_IF(itr->compare(url) == 0);
    }
    this->getDownloadInfo()->m_obWillDownloadUrls.push_back(url);
    std::sort(this->getDownloadInfo()->m_obWillDownloadUrls.begin(),
              this->getDownloadInfo()->m_obWillDownloadUrls.end(),
              compareIntroduction);
    CCLog("0----se %lu", this->getDownloadInfo()->m_obWillDownloadUrls.size());
}

void DMSelectionDownloadController::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    const std::vector<DMIntroductionItemInfo>& vec = section == 0 ? m_obListItemInfos : m_obAloneItemInfos;
    std::string url = DMDownloadBookInfo::getDownloadUrl(m_obInfo, vec.at(row * m_uiItems + item));
    for (std::vector<std::string>::iterator itr=this->getDownloadInfo()->m_obWillDownloadUrls.begin();
         itr!=this->getDownloadInfo()->m_obWillDownloadUrls.end(); itr++)
    {
        if (itr->compare(url) == 0)
        {
            this->getDownloadInfo()->m_obWillDownloadUrls.erase(itr);
            break;
        }
    }
    
    CCLog("0----dese %lu", this->getDownloadInfo()->m_obWillDownloadUrls.size());
}
