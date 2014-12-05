

#include "DMDownloadController.h"
#include "DMIntroductionController.h"
#include "DMSettingManager.h"
#include "NetWorkUtils/NetWorkUtils.h"

DMDownloadController::DMDownloadController(const DMIntroductionInfo& info, DMDownloadBookInfo* downloadInfo)
: m_obInfo(info)
, m_pDownloadInfo(NULL)
, m_bEdit(false)
, m_pEditButton(NULL)
, m_iSortRules(1)
{
    this->setTitle(info.title);
    this->setDownloadInfo(downloadInfo);
    
    if (m_iSortRules == 1)
    {
        downloadInfo->sortReverseDownloadFinishUrls();
    }
    else
    {
        downloadInfo->sortPositivDownloadFinishUrls();
    }
    
    m_pSectionView[0] = 0;
    m_pSectionView[1] = 0;
    
    float width = _dip(CAApplication::getApplication()->getWinSize().width);
    m_uiItems = MIN(8, (width - 20) / 150);
    
    DMSettingManager* settingmanger = new DMSettingManager();
    m_bDownLoadWatch = settingmanger->getSetDownLoadWatch() == On || NetWorkUtils::getInstance()->getNewWorkType() == NetWorkTypeWifi;
    delete settingmanger;
    
    CADownloadManager::getInstance()->setDownloadMaxCount(m_bDownLoadWatch ? 1 : 0);
}

DMDownloadController::~DMDownloadController()
{
    CC_SAFE_RELEASE(m_pSectionView[0]);
    CC_SAFE_RELEASE(m_pSectionView[1]);
    CC_SAFE_RELEASE(m_pDownloadInfo);
    m_obSelectedList.clear();
}

void DMDownloadController::viewDidLoad()
{
    CADipRect rect = this->getView()->getBounds();
    this->getView()->setColor(ccc4(244, 244, 244, 255));
    
    
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
        button->addTarget(this, CAControl_selector(DMDownloadController::onReturn), CAControlEventTouchUpInSide);
        
        CADipRect rightRect = barRect;
        rightRect.size.width = rightRect.size.height * 0.9f;
        rightRect.size.height *= 0.8f;
        rightRect.origin.x = barRect.size.width - rightRect.size.width * 0.7f;
        rightRect.origin.y = barRect.size.height / 2;
        m_pEditButton = CAButton::createWithCenter(rightRect, CAButtonTypeCustom);
        m_pEditButton->setTitleForState(CAControlStateNormal, "编辑");
        m_pEditButton->setTitleForState(CAControlStateSelected, "取消");
        m_pEditButton->setTitleColorForState(CAControlStateAll, CAColor_white);
        m_pEditButton->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
        bar->addSubview(m_pEditButton);
        m_pEditButton->setAllowsSelected(true);
        m_pEditButton->addTarget(this, CAControl_selector(DMDownloadController::onEdit), CAControlEventTouchUpInSide);
    }
    
    
    {
        CADipRect rect1;
        rect1.origin.x = 0;
        rect1.origin.y = 103;
        rect1.size.width = rect.size.width;
        rect1.size.height = 96;
        std::vector<CATabBarItem*> items;
        
        for (int i=0; i<2; i++)
        {
            items.push_back(CATabBarItem::create(downloadTabBatTitle[i], NULL));
        }
        
        m_pTabBarView = CATabBar::create(items, rect1.size);
        m_pTabBarView->setBackGroundView(CAView::createWithColor(CAColor_white));
        m_pTabBarView->setSelectedBackGroundView(CAView::createWithColor(CAColor_clear));
        m_pTabBarView->setSelectedIndicatorView(CAView::createWithColor(CAColor_clear));
        m_pTabBarView->setTitleColorForNormal(CAColor_black);
        m_pTabBarView->setTitleColorForSelected(ccc4(87, 154, 244, 255));
        m_pTabBarView->setDelegate(this);
        m_pTabBarView->setFrameOrigin(rect1.origin);
        this->getView()->insertSubview(m_pTabBarView, 2);
        
        CCRect lineRect0 = CCRect(m_pTabBarView->getFrame().size.width/2,
                                  m_pTabBarView->getFrame().size.height/2,
                                  2,
                                  m_pTabBarView->getFrame().size.height/2);
        CAView* lineV = CAView::createWithCenter(lineRect0, CAColor_gray);
        m_pTabBarView->insertSubview(lineV, 2);

        CADipRect lineRect = rect;
        lineRect.origin.y = 101;
        lineRect.size.height = _dip(1);
        CAView* line = CAView::createWithFrame(lineRect, CAColor_gray);
        m_pTabBarView->insertSubview(line, 2);
        
        CADipRect tabBarBgRect;
        tabBarBgRect.origin = rect1.size/2;
        tabBarBgRect.size = rect.size;
        tabBarBgRect.size.height = 110;
        m_pTabBarView->insertSubview(CAView::createWithCenter(tabBarBgRect), -1);
        
        CADipRect rect3 = rect;
        rect3.origin.y = 96;
        rect3.size.height -= rect3.origin.y;
        m_pPageView = CAPageView::createWithFrame(rect3, CAPageView::CAPageViewDirectionHorizontal);
        m_pPageView->setBackGroundColor(CAColor_clear);
        this->getView()->insertSubview(m_pPageView, 1);
        m_pPageView->setPageViewDelegate(this);
        
        CADipRect bgRect = CCRectZero;
        bgRect.size = rect3.size;
        
        CAView* backgroundView[2];
        backgroundView[0] = CAView::createWithFrame(bgRect, CAColor_clear);
        backgroundView[1] = CAView::createWithFrame(bgRect, CAColor_clear);
        CAVector<CAView*> vec;
        vec.pushBack(backgroundView[0]);
        vec.pushBack(backgroundView[1]);
        m_pPageView->setViews(vec);
        
        if (this->getDownloadInfo()->m_obWillDownloadUrls.empty()
            &&
            this->getDownloadInfo()->m_obDownloadingUrls.empty()
            &&
            this->getDownloadInfo()->m_obDownloadPausesUrls.empty())
        {
            m_pTabBarView->setSelectedAtIndex(0);
            m_pPageView->setCurrPage(0, false);
        }
        else
        {
            m_pTabBarView->setSelectedAtIndex(1);
            m_pPageView->setCurrPage(1, false);
        }
        
        
        
        {
            CADipRect viewRect = bgRect;
            viewRect.origin.y = 110;
            viewRect.size.height -= 110;
            
            m_pCollectionView = CACollectionView::createWithFrame(viewRect);
            m_pCollectionView->setCollectionViewDataSource(this);
            m_pCollectionView->setCollectionViewDelegate(this);
            m_pCollectionView->setAllowsSelection(true);
            m_pCollectionView->setAllowsMultipleSelection(false);
            m_pCollectionView->setBackGroundColor(CAColor_clear);
            m_pCollectionView->setHoriInterval(_px(20));
            m_pCollectionView->setVertInterval(_px(20));
            backgroundView[0]->addSubview(m_pCollectionView);
            
            CADipRect viewEditRect = bgRect;
            viewEditRect.origin.y = viewRect.size.height + 110 - 96;
            viewEditRect.size.height = 96;
            m_pEditButtonView0 = CAView::createWithFrame(viewEditRect, CAColor_clear);
            backgroundView[0]->addSubview(m_pEditButtonView0);
            m_pEditButtonView0->setVisible(false);
            
            
            CADipRect buttonRect = m_pEditButtonView0->getBounds();
            buttonRect.size.width /= 2;
            CAButton* button1 = CAButton::createWithFrame(buttonRect, CAButtonTypeCustom);
            button1->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_gray));
            button1->setTitleForState(CAControlStateNormal, "全选");
            button1->setTitleForState(CAControlStateSelected, "取消全选");
            button1->setTitleColorForState(CAControlStateSelected, ccc4(87, 154, 244, 255));
            button1->setTitleColorForState(CAControlStateHighlighted, CAColor_white);
            m_pEditButtonView0->addSubview(button1);
            button1->setAllowsSelected(true);
            button1->addTarget(this, CAControl_selector(DMDownloadController::onAllSelection), CAControlEventTouchUpInSide);
            
            buttonRect.origin.x = buttonRect.size.width;
            CAButton* button2 = CAButton::createWithFrame(buttonRect, CAButtonTypeCustom);
            button2->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_gray));
            button2->setTitleForState(CAControlStateNormal, "删除");
            button2->setTitleColorForState(CAControlStateNormal, ccc4(87, 154, 244, 255));
            button2->setTitleColorForState(CAControlStateHighlighted, CAColor_white);
            m_pEditButtonView0->addSubview(button2);
            button2->addTarget(this, CAControl_selector(DMDownloadController::deleteDownloaded), CAControlEventTouchUpInSide);
            
            CADipRect lineRect;
            lineRect.origin.x = 0;
            lineRect.origin.y = buttonRect.origin.y;
            lineRect.size.width = viewRect.size.width;
            lineRect.size.height = _dip(1);
            CAView* line = CAView::createWithFrame(CADipRect(lineRect), CAColor_gray);
            m_pEditButtonView0->addSubview(line);
            
            CADipRect line2Rect;
            line2Rect.origin.x = buttonRect.size.width - _dip(0.5f);
            line2Rect.origin.y = buttonRect.origin.y;
            line2Rect.size.width = _dip(1);
            line2Rect.size.height = buttonRect.size.height;
            CAView* line2 = CAView::createWithFrame(CADipRect(line2Rect), CAColor_gray);
            m_pEditButtonView0->addSubview(line2);
        
        }
        
        {
            CADipRect viewRect = bgRect;
            viewRect.origin.y = 110;
            viewRect.size.height -= 110 + 96;
            
            m_pListView = CAListView::createWithFrame(viewRect);
            m_pListView->setListViewDataSource(this);
            m_pListView->setListViewDelegate(this);
            m_pListView->setBackGroundColor(CAColor_clear);
            m_pListView->setSeparatorViewHeight(0);
            backgroundView[1]->addSubview(m_pListView);
            
            {
                CADipRect buttonRect = bgRect;
                buttonRect.origin.y = viewRect.size.height + 110;
                buttonRect.size.height = 96;
                m_pEditButtonView[0] = CAView::createWithFrame(buttonRect, CAColor_clear);
                backgroundView[1]->addSubview(m_pEditButtonView[0]);
                
                CAButton* button = CAButton::createWithFrame(m_pEditButtonView[0]->getBounds(), CAButtonTypeCustom);
                button->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_gray));
                button->setTitleForState(CAControlStateNormal, "全部暂停");
                button->setTitleForState(CAControlStateSelected, "全部开始");
                button->setTitleColorForState(CAControlStateHighlighted, CAColor_white);
                m_pEditButtonView[0]->addSubview(button);
                button->setAllowsSelected(true);
                button->addTarget(this, CAControl_selector(DMDownloadController::onDownload), CAControlEventTouchUpInSide);
                m_pDownloadControlbutton = button;
                if (this->getDownloadInfo()->m_obWillDownloadUrls.empty()
                    &&
                    this->getDownloadInfo()->m_obDownloadingUrls.empty())
                {
                    m_pDownloadControlbutton->setControlStateSelected();
                }
                
                CADipRect lineRect;
                lineRect.origin.x = 0;
                lineRect.origin.y = 0;
                lineRect.size.width = viewRect.size.width;
                lineRect.size.height = _dip(1);
                CAView* line = CAView::createWithFrame(lineRect, CAColor_gray);
                m_pEditButtonView[0]->addSubview(line);
                
                CADipRect lineRect2;
                lineRect2.origin.x = 0;
                lineRect2.origin.y = 0;
                lineRect2.size.width = _dip(1);
                lineRect2.size.height = viewRect.size.height;
                CAView* line2 = CAView::createWithFrame(lineRect2, CAColor_gray);
                m_pEditButtonView[0]->addSubview(line2);
            }
            
            {
                m_pEditButtonView[1] = CAView::createWithFrame(m_pEditButtonView[0]->getFrame(), CAColor_clear);
                backgroundView[1]->addSubview(m_pEditButtonView[1]);
                
                CADipRect buttonRect = bgRect;
                buttonRect.origin.y = 0;
                buttonRect.size.width /= 2;
                buttonRect.size.height = 96;
                CAButton* button1 = CAButton::createWithFrame(buttonRect, CAButtonTypeCustom);
                button1->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_gray));
                button1->setTitleForState(CAControlStateNormal, "全选");
                button1->setTitleForState(CAControlStateSelected, "取消全选");
                button1->setTitleColorForState(CAControlStateSelected, ccc4(87, 154, 244, 255));
                button1->setTitleColorForState(CAControlStateHighlighted, CAColor_white);
                m_pEditButtonView[1]->addSubview(button1);
                button1->setAllowsSelected(true);
                button1->addTarget(this, CAControl_selector(DMDownloadController::onAllSelectionFromListView), CAControlEventTouchUpInSide);
                
                buttonRect.origin.x = buttonRect.size.width;
                CAButton* button2 = CAButton::createWithFrame(buttonRect, CAButtonTypeCustom);
                button2->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_gray));
                button2->setTitleForState(CAControlStateNormal, "删除");
                button2->setTitleColorForState(CAControlStateNormal, ccc4(87, 154, 244, 255));
                button2->setTitleColorForState(CAControlStateHighlighted, CAColor_white);
                m_pEditButtonView[1]->addSubview(button2);
                button2->addTarget(this, CAControl_selector(DMDownloadController::deleteDownloading), CAControlEventTouchUpInSide);
                
                CADipRect lineRect;
                lineRect.origin.x = 0;
                lineRect.origin.y = 0;
                lineRect.size.width = viewRect.size.width;
                lineRect.size.height = _dip(1);
                CAView* line = CAView::createWithFrame(CADipRect(lineRect), CAColor_gray);
                m_pEditButtonView[1]->addSubview(line);
                
                CADipRect line2Rect;
                line2Rect.origin.x = buttonRect.size.width - _dip(0.5f);
                line2Rect.origin.y = buttonRect.origin.y;
                line2Rect.size.width = _dip(1);
                line2Rect.size.height = buttonRect.size.height;
                CAView* line2 = CAView::createWithFrame(CADipRect(line2Rect), CAColor_gray);
                m_pEditButtonView[1]->addSubview(line2);
                
                m_pEditButtonView[1]->setVisible(false);
            }
        }
        
    }
    
    CADownloadManager::getInstance()->setDownloadManagerDelegate(this);
    
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
    m_pLoading->setTargetOnCancel(this, callfunc_selector(DMDownloadController::initWithDownload));
    m_pLoading->stopAnimating();
    
    UMENG_Event("download");
    
}

void DMDownloadController::viewDidUnload()
{
    CADownloadManager::getInstance()->setDownloadManagerDelegate(NULL);
}

void DMDownloadController::initWithDownload()
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "download/";
    CCFileUtils::sharedFileUtils()->createDirectory(path.c_str());
    
    std::string bookPath = path + m_obInfo.ID + "/";
    CCFileUtils::sharedFileUtils()->createDirectory(bookPath.c_str());
    
    for (std::vector<std::string>::iterator itr=this->getDownloadInfo()->m_obWillDownloadUrls.begin();
         itr!=this->getDownloadInfo()->m_obWillDownloadUrls.end(); )
    {
        std::string filePath = bookPath;
        if (this->getDownloadInfo()->m_obListItemInfoMaps.count(*itr) > 0)
        {
            filePath += this->getDownloadInfo()->m_obListItemInfoMaps.at(*itr).ID + ".zip";
        }
        else
        {
            filePath += this->getDownloadInfo()->m_obAloneItemInfoMaps.at(*itr).ID + ".zip";
        }
       
        std::string url = *itr;

        unsigned long id = CADownloadManager::getInstance()->enqueueDownload(url, filePath, "Referer: http://images.dmzj.com/", m_pDownloadInfo->m_obFilePathTextTag);
        if (m_bDownLoadWatch == false)
        {
            CADownloadManager::getInstance()->pauseDownload(id);
        }
        itr = this->getDownloadInfo()->m_obWillDownloadUrls.erase(itr);
        if (id != 0)
        {
            this->getDownloadInfo()->m_obDownloadingUrls.push_back(url);
            this->getDownloadInfo()->m_obItemInfoIDMaps[url] = id;
        }
    }
    m_pListView->reloadData();
}

void DMDownloadController::tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index)
{
    m_pPageView->setCurrPage(index, true);
}

void DMDownloadController::pageViewDidEndTurning(CAPageView* pageView)
{
    m_pTabBarView->setSelectedAtIndex(m_pPageView->getCurrPage());
}

void DMDownloadController::onReturn(CAControl* control, CCPoint point)
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}

void DMDownloadController::onEdit(CAControl* control, CCPoint point)
{
    CC_RETURN_IF(m_pLoading->isAnimating());
    this->setEdit(!static_cast<CAButton*>(control)->isSelected());
}

void DMDownloadController::onAllSelection(CAControl* control, CCPoint point)
{
    CAButton* btn = dynamic_cast<CAButton*>(control);
    if (btn->isSelected() == false)
    {
        for (int a=0; a<2; a++)
        {
            const std::vector<std::string>& vec =
            (
             a == 0
             ? this->getDownloadInfo()->m_obDownloadFinishedListUrls
             : this->getDownloadInfo()->m_obDownloadFinishedAloneUrls
             );
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
                    m_pCollectionView->setSelectRowAtIndexPath(a, i, j);
                }
            }
        }
        for (std::vector<std::string>::iterator itr=this->getDownloadInfo()->m_obDownloadFinishedListUrls.begin();
             itr!=this->getDownloadInfo()->m_obDownloadFinishedListUrls.end(); itr++)
        {
            m_obSelectedList.insert(*itr);
        }
        for (std::vector<std::string>::iterator itr=this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.begin();
             itr!=this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.end(); itr++)
        {
            m_obSelectedList.insert(*itr);
        }
    }
    else
    {
        for (int a=0; a<2; a++)
        {
            const std::vector<std::string>& vec =
            (
             a == 0
             ? this->getDownloadInfo()->m_obDownloadFinishedListUrls
             : this->getDownloadInfo()->m_obDownloadFinishedAloneUrls
             );
            
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
        m_obSelectedList.clear();
    }
}

void DMDownloadController::onAllSelectionFromListView(CAControl* control, CCPoint point)
{
    CAButton* btn = dynamic_cast<CAButton*>(control);
    if (btn->isSelected() == false)
    {
        for (int i=0; i<this->getDownloadInfo()->m_obDownloadingUrls.size() + this->getDownloadInfo()->m_obDownloadPausesUrls.size(); i++)
        {
            m_pListView->setSelectAtIndex(i);
        }
        
        for (int i=0; i<this->getDownloadInfo()->m_obDownloadingUrls.size(); i++)
        {
            m_obEditDownloading.insert(this->getDownloadInfo()->m_obDownloadingUrls.at(i));
        }
        for (int i=0; i<this->getDownloadInfo()->m_obDownloadPausesUrls.size(); i++)
        {
            m_obEditDownloading.insert(this->getDownloadInfo()->m_obDownloadPausesUrls.at(i));
        }
    }
    else
    {
        for (int i=0; i<this->getDownloadInfo()->m_obDownloadingUrls.size() + this->getDownloadInfo()->m_obDownloadPausesUrls.size(); i++)
        {
            m_pListView->setUnSelectAtIndex(i);
        }
        m_obEditDownloading.clear();
    }

}

void DMDownloadController::onDownload(CAControl* control, CCPoint point)
{
    CAButton* button = dynamic_cast<CAButton*>(control);
    if (button->isSelected())
    {
        getDownloadInfo()->m_obDownloadingUrls.insert(getDownloadInfo()->m_obDownloadingUrls.end(),
                                                getDownloadInfo()->m_obDownloadPausesUrls.begin(),
                                                getDownloadInfo()->m_obDownloadPausesUrls.end());
        getDownloadInfo()->m_obDownloadPausesUrls.clear();
        for (int i=0; i<getDownloadInfo()->m_obDownloadingUrls.size(); i++)
        {
            const std::string& url = getDownloadInfo()->m_obDownloadingUrls.at(i);
            CADownloadManager::getInstance()->resumeDownload(getDownloadInfo()->m_obItemInfoIDMaps.at(url));
        }
    }
    else
    {
        getDownloadInfo()->m_obDownloadPausesUrls.insert(getDownloadInfo()->m_obDownloadPausesUrls.end(),
                                                getDownloadInfo()->m_obDownloadingUrls.begin(),
                                                getDownloadInfo()->m_obDownloadingUrls.end());
        getDownloadInfo()->m_obDownloadingUrls.clear();
        for (int i=0; i<getDownloadInfo()->m_obDownloadPausesUrls.size(); i++)
        {
            const std::string& url = getDownloadInfo()->m_obDownloadPausesUrls.at(i);
            CADownloadManager::getInstance()->pauseDownload(getDownloadInfo()->m_obItemInfoIDMaps.at(url));
        }
    }
    m_pListView->reloadData();
}

void DMDownloadController::deleteDownloaded(CAControl* control, CCPoint point)
{
    std::vector<std::string> eraseUrl;
    std::set<std::string>::iterator itr;
    for (itr = m_obSelectedList.begin(); itr != m_obSelectedList.end(); itr++)
    {
        unsigned long id = getDownloadInfo()->m_obItemInfoIDMaps.at(*itr);
        CADownloadManager::getInstance()->eraseDownload(id);
        eraseUrl.push_back(*itr);
    }
    for (int i=0; i<eraseUrl.size(); i++)
    {
        std::vector<std::string>::iterator itr;
        for (itr=getDownloadInfo()->m_obDownloadFinishedListUrls.begin();
             itr!=getDownloadInfo()->m_obDownloadFinishedListUrls.end(); itr++)
        {
            if (eraseUrl.at(i).compare(*itr) == 0)
            {
                getDownloadInfo()->m_obDownloadFinishedListUrls.erase(itr);
                break;
            }
        }
        
        std::vector<std::string>::iterator itr2;
        for (itr2=getDownloadInfo()->m_obDownloadFinishedAloneUrls.begin();
             itr2!=getDownloadInfo()->m_obDownloadFinishedAloneUrls.end(); itr2++)
        {
            if (eraseUrl.at(i).compare(*itr2) == 0)
            {
                getDownloadInfo()->m_obDownloadFinishedAloneUrls.erase(itr2);
                break;
            }
        }
    }
    eraseUrl.clear();
    this->setEdit(false);
    m_pEditButton->setControlStateNormal();
    m_pCollectionView->reloadData();
}

void DMDownloadController::deleteDownloading(CAControl* control, CCPoint point)
{
    std::set<std::string>::iterator itr1;
    for (itr1 = m_obEditDownloading.begin(); itr1 != m_obEditDownloading.end(); itr1++)
    {
        std::vector<std::string>::iterator itr2;
        for (itr2=getDownloadInfo()->m_obDownloadingUrls.begin();
             itr2!=getDownloadInfo()->m_obDownloadingUrls.end(); itr2++)
        {
            if (itr1->compare(*itr2) == 0)
            {
                getDownloadInfo()->m_obDownloadingUrls.erase(itr2);
                break;
            }
        }
        std::vector<std::string>::iterator itr3;
        for (itr3=getDownloadInfo()->m_obDownloadPausesUrls.begin();
             itr3!=getDownloadInfo()->m_obDownloadPausesUrls.end(); itr3++)
        {
            if (itr1->compare(*itr3) == 0)
            {
                getDownloadInfo()->m_obDownloadPausesUrls.erase(itr3);
                break;
            }
        }
        unsigned long id = getDownloadInfo()->m_obItemInfoIDMaps.at(*itr1);
        CADownloadManager::getInstance()->eraseDownload(id);
    }
    m_obEditDownloading.clear();
    this->setEdit(false);
    m_pEditButton->setControlStateNormal();
}

void DMDownloadController::onSort(CAControl* control, CCPoint point)
{
    CASegmentedControl* segmented = dynamic_cast<CASegmentedControl*>(control);
    if (segmented)
    {
        m_iSortRules = segmented->getSelectedIndex();
        if (m_iSortRules == 0)
        {
            getDownloadInfo()->sortPositivDownloadFinishUrls();
        }
        else
        {
            getDownloadInfo()->sortReverseDownloadFinishUrls();
        }
        m_pCollectionView->reloadData();
    }
}

void DMDownloadController::setEdit(bool var)
{
    CC_RETURN_IF(m_bEdit == var);
    m_bEdit = var;
    
    m_pListView->reloadData();
    m_pListView->setAlpha(0);
    m_pTabBarView->stopAllActions();
    m_pCollectionView->stopAllActions();
    m_pListView->stopAllActions();
    m_pEditButtonView[0]->stopAllActions();
    m_pEditButtonView[1]->stopAllActions();
    
    CADipRect rect = this->getView()->getBounds();
    rect.origin.y = 96;
    rect.size.height -= rect.origin.y;
    
    if (m_bEdit)
    {
        m_pPageView->setScrollEnabled(false);
        m_pListView->setAllowsSelection(true);
        m_pListView->setAllowsMultipleSelection(true);
        m_pCollectionView->setAllowsMultipleSelection(true);
        m_pEditButtonView[0]->setVisible(false);
        m_pEditButtonView[1]->setVisible(true);
        m_pEditButtonView[1]->setAlpha(0);
        m_pEditButtonView0->setVisible(true);
        m_pEditButtonView0->setAlpha(0);
        
        CCFadeOut* fadeOut = CCFadeOut::create(0.3f);
        CCCallFuncND* callfuncND = CCCallFuncND::create(m_pTabBarView, callfuncND_selector(CATabBar::setVisible), false);
        CCSequence* allActions= CCSequence::createWithTwoActions(fadeOut, callfuncND);
        m_pTabBarView->runAction(allActions);
        
        CADipRect viewRect = rect;
        viewRect.origin.y = 0;
        viewRect.size.height -= 96;
        
        m_pCollectionView->runAction(CCEaseSineInOut::create(CCFrameTo::create(0.3f, viewRect)));
        m_pListView->runAction(CCEaseSineInOut::create(CCFrameTo::create(0.0f, viewRect)));
        m_pListView->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.05f), CCFadeIn::create(0.25f)));
        m_pEditButtonView[1]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.05f), CCFadeIn::create(0.25f)));
        m_pEditButtonView0->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.05f), CCFadeIn::create(0.25f)));
        
        for (std::vector<std::string>::iterator itr=getDownloadInfo()->m_obDownloadingUrls.begin();
             itr!=getDownloadInfo()->m_obDownloadingUrls.end(); itr++)
        {
            unsigned long id = getDownloadInfo()->m_obItemInfoIDMaps.at(*itr);
            CADownloadManager::getInstance()->pauseDownload(id);
        }
    }
    else
    {
        m_pTabBarView->setVisible(true);
        m_pPageView->setScrollEnabled(true);
        m_pListView->setAllowsSelection(false);
        m_pListView->setAllowsMultipleSelection(false);
        m_pCollectionView->setAllowsMultipleSelection(false);
        m_pEditButtonView[1]->setVisible(false);
        m_pEditButtonView[0]->setVisible(true);
        m_pEditButtonView[0]->setAlpha(0);
        m_pEditButtonView0->setVisible(false);
        
        CCFadeIn* fadeIn = CCFadeIn::create(0.3f);
        m_pTabBarView->runAction(fadeIn);
        
        CADipRect viewRect = rect;
        viewRect.origin.y = 110;
        viewRect.size.height -= 110;
        m_pCollectionView->runAction(CCEaseSineInOut::create(CCFrameTo::create(0.3f, viewRect)));
        viewRect.size.height -= 96;
        m_pListView->runAction(CCEaseSineInOut::create(CCFrameTo::create(0.0f, viewRect)));
        m_pListView->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.05f), CCFadeIn::create(0.25f)));
        m_pEditButtonView[0]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.05f), CCFadeIn::create(0.25f)));
        
        for (std::vector<std::string>::iterator itr=getDownloadInfo()->m_obDownloadingUrls.begin();
             itr!=getDownloadInfo()->m_obDownloadingUrls.end(); itr++)
        {
            unsigned long id = getDownloadInfo()->m_obItemInfoIDMaps.at(*itr);
            CADownloadManager::getInstance()->resumeDownload(id);
        }
    }
    
    m_obEditDownloading.clear();
    m_obSelectedList.clear();
}

bool DMDownloadController::isEdit()
{
    return m_bEdit;
}

unsigned int DMDownloadController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return _px(70);
}

CACollectionViewCell* DMDownloadController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = NULL;
    
    const std::vector<std::string>& vec =
    (
     section == 0
     ? this->getDownloadInfo()->m_obDownloadFinishedListUrls
     : this->getDownloadInfo()->m_obDownloadFinishedAloneUrls
     );
    
    const std::map<std::string, DMIntroductionItemInfo>& m =
    (
     section == 0
     ? this->getDownloadInfo()->m_obListItemInfoMaps
     : this->getDownloadInfo()->m_obAloneItemInfoMaps
     );
    
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
        const DMIntroductionItemInfo& info = m.at(vec.at(row * m_uiItems + item));
        itemCell->getLabel()->setText(info.chapter_name);
    }
    
    return cell;
}

unsigned int DMDownloadController::numberOfSections(CACollectionView *collectionView)
{
    return this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.empty() ? 1 : 2;
}

unsigned int DMDownloadController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    const std::vector<std::string>& vec =
    (
     section == 0
     ? this->getDownloadInfo()->m_obDownloadFinishedListUrls
     : this->getDownloadInfo()->m_obDownloadFinishedAloneUrls
     );
    
    if (vec.size() % m_uiItems == 0)
    {
        return vec.size() / m_uiItems;
    }
    else
    {
        return vec.size() / m_uiItems + 1;
    }
}

unsigned int DMDownloadController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return m_uiItems;
}


unsigned int DMDownloadController::collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
{
    return _px(70);
}


CAView* DMDownloadController::collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
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
            sprintf(text, "连载(%lu话)", this->getDownloadInfo()->m_obDownloadFinishedListUrls.size());
        }
        else
        {
            sprintf(text, "单行本(%lu话)", this->getDownloadInfo()->m_obDownloadFinishedListUrls.size());
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
            control->addTarget(this, CAControl_selector(DMDownloadController::onSort));
        }
        
    }
    return m_pSectionView[section];
}


void DMDownloadController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    if (m_bEdit)
    {
        const std::vector<std::string>& vec =
        (
         section == 0
         ? this->getDownloadInfo()->m_obDownloadFinishedListUrls
         : this->getDownloadInfo()->m_obDownloadFinishedAloneUrls
         );
        
        m_obSelectedList.insert(vec.at(row * m_uiItems + item));
    }
    else
    {
        
    }
    
}

void DMDownloadController::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    if (m_bEdit)
    {
        const std::vector<std::string>& vec =
        (
         section == 0
         ? this->getDownloadInfo()->m_obDownloadFinishedListUrls
         : this->getDownloadInfo()->m_obDownloadFinishedAloneUrls
         );
        
        m_obSelectedList.erase(vec.at(row * m_uiItems + item));
    }
}


unsigned int DMDownloadController::numberOfIndex(CAListView *listView)
{
    return this->getDownloadInfo()->m_obDownloadingUrls.size() + this->getDownloadInfo()->m_obDownloadPausesUrls.size();
}

unsigned int DMDownloadController::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
    return _px(100);
}

CAListViewCell* DMDownloadController::listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index)
{
    DMDownloadListCell* cell = (DMDownloadListCell*)listView->dequeueReusableCellWithIdentifier("a");
    if (cell == NULL)
    {
        cell = DMDownloadListCell::create("a", cellSize);
        cell->setDownloadController(this);
    }
    int size = this->getDownloadInfo()->m_obDownloadingUrls.size();
    if (index < size)
    {
        cell->setDownloadUrl(this->getDownloadInfo()->m_obDownloadingUrls.at(index));
        cell->setPauseDownload(false);
    }
    else
    {
        cell->setDownloadUrl(this->getDownloadInfo()->m_obDownloadPausesUrls.at(index - size));
        cell->setPauseDownload(true);
    }
    cell->setEdit(this->isEdit());
    return cell;
}

void DMDownloadController::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{
    std::string url;
    unsigned int size = this->getDownloadInfo()->m_obDownloadingUrls.size();
    if (index < size)
    {
        url = this->getDownloadInfo()->m_obDownloadingUrls.at(index);
    }
    else
    {
        url = this->getDownloadInfo()->m_obDownloadPausesUrls.at(index - size);
    }
    
    m_obEditDownloading.insert(url);
}

void DMDownloadController::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{
    std::string url;
    unsigned int size = this->getDownloadInfo()->m_obDownloadingUrls.size();
    if (index < size)
    {
        url = this->getDownloadInfo()->m_obDownloadingUrls.at(index);
    }
    else
    {
        url = this->getDownloadInfo()->m_obDownloadPausesUrls.at(index - size);
    }
    
    m_obEditDownloading.erase(url);
}

void DMDownloadController::onError(unsigned long download_id, CADownloadManager::ErrorCode errorCode)
{
    //const std::string& url = DownloadManager::getInstance()->getDownloadUrl(download_id);
}

void DMDownloadController::onProgress(unsigned long download_id, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded)
{
    const std::string& url = CADownloadManager::getInstance()->getDownloadUrl(download_id);
    m_mDownloadProgresss[url] = nowDownloaded;
}

void DMDownloadController::onPauseDownload(unsigned long download_id)
{
    //const std::string& url = CADownloadManager::getInstance()->getDownloadUrl(download_id);
}

void DMDownloadController::onResumeDownload(unsigned long download_id)
{
    //const std::string& url = CADownloadManager::getInstance()->getDownloadUrl(download_id);
}

void DMDownloadController::onSuccess(unsigned long download_id)
{
    const std::string& url = CADownloadManager::getInstance()->getDownloadUrl(download_id);
    std::vector<std::string>::iterator itr = this->getDownloadInfo()->m_obDownloadingUrls.begin();
    CCLog("    %s", url.c_str());
    for (; itr!=this->getDownloadInfo()->m_obDownloadingUrls.end(); itr++)
    {
        CC_BREAK_IF(itr->compare(url) == 0);
    }
    CC_RETURN_IF(itr == this->getDownloadInfo()->m_obDownloadingUrls.end());
    if (this->getDownloadInfo()->m_obAloneItemInfoMaps.count(url))
    {
        this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.push_back(*itr);
    }
    if (this->getDownloadInfo()->m_obListItemInfoMaps.count(url))
    {
        this->getDownloadInfo()->m_obDownloadFinishedListUrls.push_back(*itr);
    }
    
    this->getDownloadInfo()->m_obDownloadingUrls.erase(itr);
    if (m_iSortRules == 0)
    {
        getDownloadInfo()->sortPositivDownloadFinishUrls();
    }
    else
    {
        getDownloadInfo()->sortReverseDownloadFinishUrls();
    }
    m_pListView->reloadData();
    m_pCollectionView->reloadData();
    
    if (this->getDownloadInfo()->m_obWillDownloadUrls.empty()
        &&
        this->getDownloadInfo()->m_obDownloadingUrls.empty())
    {
        m_pDownloadControlbutton->setControlStateSelected();
    }
    
    int downloadNumber = this->getDownloadInfo()->m_obDownloadFinishedAloneUrls.size()
    + this->getDownloadInfo()->m_obDownloadFinishedListUrls.size();
    
    DMDownloadInfo info;
    info.bookID = m_obInfo.ID;
    info.bookTitle = m_obInfo.title;
    info.imageUrl = m_obInfo.cover;
    
    char alreadyCache[64];
    sprintf(alreadyCache, "%d话", downloadNumber);
    
    int cacheNumber = this->getDownloadInfo()->m_obListItemInfoMaps.size()
    + this->getDownloadInfo()->m_obAloneItemInfoMaps.size();
    char cache[64];
    sprintf(cache, "%d话", cacheNumber);
    
    char notCache[64];
    sprintf(notCache, "%d话", cacheNumber - downloadNumber);
    
    info.alreadyCache = alreadyCache;
    info.notCache = notCache;
    info.sizeExplain = cache;
    DMDownloadInfoManager* infoManager = DMDownloadInfoManager::sharedDMDownloadInfoManager();
    
    if (downloadNumber == 1)
    {
        
        infoManager->_saved(info);
    }
    else
    {
        infoManager->_update(info);
    }
}

#pragma DMDownloadListCell

DMDownloadListCell* DMDownloadListCell::create(const std::string& reuseIdentifier, const CADipSize& size)
{
    DMDownloadListCell* cell = new DMDownloadListCell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier, size))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool DMDownloadListCell::initWithReuseIdentifier(const std::string& reuseIdentifier, const CADipSize& size)
{
    m_cSize = size;
    m_bEdit = false;
    m_pController = NULL;
    m_pIconImageView = NULL;
    if (!CAListViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }
    
    
    CADipRect chapterFrame = CADipRect(42, 12, 300, 50);
    m_pChapterName = CALabel::createWithFrame(chapterFrame);
    m_pChapterName->setNumberOfLine(1);
    m_pChapterName->setColor(ccc4(100, 100, 100, 255));
    m_pChapterName->setFontSize(_px(28));
    this->addSubview(m_pChapterName);
    
    CADipRect progressCenter;
    progressCenter.size = CADipSize(size.width - 150, 0);
    progressCenter.origin.y = size.height/2;
    progressCenter.origin.x = 42 + progressCenter.size.width/2;
    m_pProgressView = CAProgress::create();
    m_pProgressView->setCenter(progressCenter);
    m_pProgressView->setProgressTintColor(ccc4(87, 154, 244, 255));
    this->addSubview(m_pProgressView);
    
    CADipRect progressLabelFrame;
    progressLabelFrame.origin.x = 42;
    progressLabelFrame.origin.y = _dip(m_pProgressView->getFrame().origin.y + m_pProgressView->getFrame().size.height) + 15;
    progressLabelFrame.size = CADipSize(300, 60);
    m_pProgressLabel = CALabel::createWithFrame(progressLabelFrame);
    m_pProgressLabel->setNumberOfLine(1);
    m_pProgressLabel->setColor(ccc4(153, 153, 153, 255));
    m_pProgressLabel->setFontSize(_px(22));
    this->addSubview(m_pProgressLabel);
    
    CADipRect progress2LabelFrame;
    progress2LabelFrame.origin.x = size.width - 150 + 42 - 100;
    progress2LabelFrame.origin.y = _dip(m_pProgressView->getFrame().origin.y + m_pProgressView->getFrame().size.height) + 15;
    progress2LabelFrame.size = CADipSize(100, 60);
    m_pProgress2Label = CALabel::createWithFrame(progress2LabelFrame);
    m_pProgress2Label->setNumberOfLine(1);
    m_pProgress2Label->setColor(ccc4(153, 153, 153, 255));
    m_pProgress2Label->setFontSize(_px(22));
    m_pProgress2Label->setTextAlignment(CATextAlignmentRight);
    this->addSubview(m_pProgress2Label);
    
    CADipRect buttonCenter;
    buttonCenter.size = CADipSize(70, 70);
    buttonCenter.origin.y = size.height/2;
    buttonCenter.origin.x = size.width - 65;
    m_pButton = CAButton::createWithCenter(buttonCenter, CAButtonTypeCustom);
    m_pButton->setImageForState(CAControlStateNormal, CAImage::create(downloadButtonPause));
    m_pButton->setImageForState(CAControlStateSelected, CAImage::create(downloadButtonStart));
    m_pButton->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
    m_pButton->setAllowsSelected(true);
    this->addSubview(m_pButton);
    m_pButton->addTarget(this, CAControl_selector(DMDownloadListCell::onButton), CAControlEventTouchUpInSide);
    
    m_pIconImageView = CAImageView::createWithCenter(CADipRect(50, 50, 35, 35));
    this->addSubview(m_pIconImageView);
    
    CAScheduler::schedule(schedule_selector(DMDownloadListCell::update), this, 0);
    
    return true;
}

void DMDownloadListCell::onButton(CAControl* control, CCPoint point)
{
    CAButton* button = dynamic_cast<CAButton*>(control);
    int size = m_pController->getDownloadInfo()->m_obDownloadingUrls.size();
    if (button->isSelected())
    {
        m_pController->getDownloadInfo()->m_obDownloadingUrls.push_back(m_sDownloadUrl);
        m_pController->getDownloadInfo()->m_obDownloadPausesUrls.erase(m_pController->getDownloadInfo()->m_obDownloadPausesUrls.begin() + m_nIndex - size);
        CADownloadManager::getInstance()->resumeDownload(m_pController->getDownloadInfo()->m_obItemInfoIDMaps.at(m_sDownloadUrl));
    }
    else
    {
        m_pController->getDownloadInfo()->m_obDownloadPausesUrls.push_back(m_sDownloadUrl);
        m_pController->getDownloadInfo()->m_obDownloadingUrls.erase(m_pController->getDownloadInfo()->m_obDownloadingUrls.begin() + m_nIndex);
        CADownloadManager::getInstance()->pauseDownload(m_pController->getDownloadInfo()->m_obItemInfoIDMaps.at(m_sDownloadUrl));
    }
    button->retain()->autorelease();
    m_pController->m_pListView->reloadData();
}

void DMDownloadListCell::update(float dt)
{
    this->setNowDownloadSize(m_pController->m_mDownloadProgresss[m_sDownloadUrl]);
}

void DMDownloadListCell::setEdit(bool var)
{
    CC_RETURN_IF(m_bEdit == var);
    m_bEdit = var;
    
    if (m_bEdit)
    {
        CADipRect chapterFrame = CADipRect(102, 12, 300, 50);
        m_pChapterName->setFrame(chapterFrame);
        
        CADipRect progressCenter;
        progressCenter.size = CADipSize(m_cSize.width - 150, 0);
        progressCenter.origin.y = m_cSize.height/2;
        progressCenter.origin.x = 102 + progressCenter.size.width/2;
        m_pProgressView->setCenter(progressCenter);
        
        CADipRect progressLabelFrame;
        progressLabelFrame.origin.x = 102;
        progressLabelFrame.origin.y = _dip(m_pProgressView->getFrame().origin.y + m_pProgressView->getFrame().size.height) + 15;
        progressLabelFrame.size = CADipSize(300, 60);
        m_pProgressLabel->setFrame(progressLabelFrame);
        
        CADipRect progress2LabelFrame;
        progress2LabelFrame.origin.x = m_cSize.width - 150 + 42 - 40;
        progress2LabelFrame.origin.y = _dip(m_pProgressView->getFrame().origin.y + m_pProgressView->getFrame().size.height) + 15;
        progress2LabelFrame.size = CADipSize(100, 60);
        m_pProgress2Label->setFrame(progress2LabelFrame);
        
        m_pButton->setVisible(false);
        
        m_pIconImageView->setImage(CAImage::create("source_material/cell_icon_normal.png"));
    }
    else
    {
        CADipRect chapterFrame = CADipRect(42, 12, 300, 50);
        m_pChapterName->setFrame(chapterFrame);

        CADipRect progressCenter;
        progressCenter.size = CADipSize(m_cSize.width - 150, 0);
        progressCenter.origin.y = m_cSize.height/2;
        progressCenter.origin.x = 42 + progressCenter.size.width/2;
        m_pProgressView->setCenter(progressCenter);
        
        CADipRect progressLabelFrame;
        progressLabelFrame.origin.x = 42;
        progressLabelFrame.origin.y = _dip(m_pProgressView->getFrame().origin.y + m_pProgressView->getFrame().size.height) + 15;
        progressLabelFrame.size = CADipSize(300, 60);
        m_pProgressLabel->setFrame(progressLabelFrame);
        
        CADipRect progress2LabelFrame;
        progress2LabelFrame.origin.x = m_cSize.width - 150 + 42 - 100;
        progress2LabelFrame.origin.y = _dip(m_pProgressView->getFrame().origin.y + m_pProgressView->getFrame().size.height) + 15;
        progress2LabelFrame.size = CADipSize(100, 60);
        m_pProgress2Label->setFrame(progress2LabelFrame);
        
        m_pButton->setVisible(true);
        
        m_pIconImageView->setImage(NULL);
    }
    
}

bool DMDownloadListCell::isEdit()
{
    return m_bEdit;
}

void DMDownloadListCell::normalListViewCell()
{
    CC_RETURN_IF(m_pIconImageView == NULL);
    if (m_bEdit)
    {
        m_pIconImageView->setImage(CAImage::create("source_material/cell_icon_normal.png"));
    }
    else
    {
        m_pIconImageView->setImage(NULL);
    }
    
}

void DMDownloadListCell::highlightedListViewCell()
{
    CC_RETURN_IF(m_pIconImageView == NULL);
    m_pIconImageView->setImage(CAImage::create("source_material/cell_icon_selected.png"));
}

void DMDownloadListCell::selectedListViewCell()
{
    CC_RETURN_IF(m_pIconImageView == NULL);
    m_pIconImageView->setImage(CAImage::create("source_material/cell_icon_selected.png"));
}

void DMDownloadListCell::disabledListViewCell()
{
    CC_RETURN_IF(m_pIconImageView == NULL);
    m_pIconImageView->setImage(NULL);
}

void DMDownloadListCell::setPauseDownload(bool isPause, bool isCallback)
{
    if (isPause)
    {
        m_pButton->setControlStateSelected();
    }
    else
    {
        m_pButton->setControlStateNormal();
    }
    
    CC_RETURN_IF(!isCallback);
    
    int size = m_pController->getDownloadInfo()->m_obDownloadingUrls.size();
    if (m_pButton->isSelected())
    {
        m_pController->getDownloadInfo()->m_obDownloadingUrls.push_back(m_sDownloadUrl);
        m_pController->getDownloadInfo()->m_obDownloadPausesUrls.erase(m_pController->getDownloadInfo()->m_obDownloadPausesUrls.begin() + m_nIndex - size);
        CADownloadManager::getInstance()->resumeDownload(m_pController->getDownloadInfo()->m_obItemInfoIDMaps.at(m_sDownloadUrl));
    }
    else
    {
        m_pController->getDownloadInfo()->m_obDownloadPausesUrls.push_back(m_sDownloadUrl);
        m_pController->getDownloadInfo()->m_obDownloadingUrls.erase(m_pController->getDownloadInfo()->m_obDownloadingUrls.begin() + m_nIndex);
        CADownloadManager::getInstance()->pauseDownload(m_pController->getDownloadInfo()->m_obItemInfoIDMaps.at(m_sDownloadUrl));
    }
    m_pController->m_pListView->reloadData();
}

void DMDownloadListCell::setFileSize(float size)
{
    m_fFileSize = size;
}

void DMDownloadListCell::setNowDownloadSize(unsigned long size)
{
    char pro[64];
    sprintf(pro, "%.2fMB/%.2fMB", ((float)size) / 1048576, m_fFileSize / 1048576);
    m_pProgressLabel->setText(pro);
    
    int progress = size/m_fFileSize * 100;
    
    char pro2[16];
    sprintf(pro2, "%d%%", progress);
    m_pProgress2Label->setText(pro2);

    m_pProgressView->setProgress(progress/100.0f);
}

void DMDownloadListCell::setTitle(const std::string& title)
{
    m_pChapterName->setText(title);
}

void DMDownloadListCell::setDownloadUrl(const std::string& url)
{
    m_sDownloadUrl = url;
    DMIntroductionItemInfo info;
    if (m_pController->getDownloadInfo()->m_obListItemInfoMaps.count(url))
    {
        info = m_pController->getDownloadInfo()->m_obListItemInfoMaps[url];
    }
    else
    {
        info = m_pController->getDownloadInfo()->m_obAloneItemInfoMaps[url];
    }
    this->setTitle(info.chapter_name);
    this->setFileSize(atof(info.filesize.c_str()));
    unsigned long id = m_pController->getDownloadInfo()->m_obItemInfoIDMaps.at(m_sDownloadUrl);
    m_pController->m_mDownloadProgresss[m_sDownloadUrl] = CADownloadManager::getInstance()->getLocalFileSize(id);
}

void DMDownloadListCell::setDownloadController(DMDownloadController* controller)
{
    m_pController = controller;
    if (m_pController->m_bDownLoadWatch == false)
    {
        m_pButton->setAllowsSelected(false);
    }
}