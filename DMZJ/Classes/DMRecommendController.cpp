

#include "DMRecommendController.h"
#include "DMSearchController.h"
#include "DMCatalogController.h"

DMRecommendController::DMRecommendController()
:m_nLastSelectedIndex(0)
,m_pNewTabBar(NULL)
,m_pSelectedImageView(NULL)
{

}

DMRecommendController::~DMRecommendController()
{
    
}

bool DMRecommendController::initWithRecommendController()
{
    
    std::vector<CAViewController*> vec;
    
    DMCatalogController* controller1 = DMCatalogController::create();
    controller1->setTitle(catalogTitleTag[0]);
    vec.push_back(controller1);
    
    DMCatalogController* controller2 = DMCatalogController::create();
    controller2->setTitle(catalogTitleTag[1]);
    vec.push_back(controller2);
    
    DMCatalogController* controller3 = DMCatalogController::create();
    controller3->setTitle(catalogTitleTag[2]);
    vec.push_back(controller3);
    
    DMCatalogController* controller4 = DMCatalogController::create();
    controller4->setTitle(catalogTitleTag[3]);
    vec.push_back(controller4);
    
    DMCatalogController* controller5 = DMCatalogController::create();
    controller5->setTitle(catalogTitleTag[4]);
    vec.push_back(controller5);
    
    m_nSelectedIndex = 0;
    
    CATabBarController::initWithViewControllers(vec, CABarVerticalAlignmentTop);
    
    return true;
}

void DMRecommendController::viewDidLoad()
{
    CATabBarController::viewDidLoad();
    this->setScrollEnabled(true);
    
    m_pTabBar->setDelegate(NULL);
    m_pTabBar->setVisible(false);
    
    CAView* background = CAView::createWithFrame(m_pTabBar->getFrame(), ccc4(87, 154, 244, 255));
    this->getView()->addSubview(background);
    
    CCRect rect = m_pTabBar->getFrame();
    rect.origin.x = _px(20);
    rect.size.width -= _px(30);
    rect.size.height *= 0.8f;
    rect.origin.y = (_px(96) - rect.size.height) / 2;
    m_pNewTabBar = CASegmentedControl::createWithFrame(rect, 5);
    
    for (int i=0; i<5; i++)
    {
        m_pNewTabBar->setBackgroundImageAtIndex(NULL, i, CAControlStateAll);
        m_pNewTabBar->setTitleAtIndex(titleText[i], i, CAControlStateAll);
        m_pNewTabBar->setTitleColorAtIndex(CAColor_white, i, CAControlStateNormal);
        m_pNewTabBar->setTitleColorAtIndex(CAColor_white, i, CAControlStateSelected);
        m_pNewTabBar->setTitleColorAtIndex(CAColor_gray, i, CAControlStateHighlighted);
    }
    this->getView()->addSubview(m_pNewTabBar);
    
    m_pNewTabBar->insertSegmentWithImage(CAImage::create(searchIcon), 5, CAControlStateNormal);
    m_pNewTabBar->setImageColorAtIndex(CAColor_gray, 5, CAControlStateHighlighted);
    m_pNewTabBar->setBackgroundImageAtIndex(NULL, 5, CAControlStateAll);
    
    m_pNewTabBar->addTarget(this, CAControl_selector(DMRecommendController::onSegmentedControl));
    m_pNewTabBar->setSelectedAtIndex(0);
    
    CCPoint point = m_pNewTabBar->getItemSize();
    point.x = point.x * (0 + 0.5f);
    point.y *= 0.525f;
    
    if (m_pSelectedImageView == NULL)
    {
        m_pSelectedImageView = CAImageView::createWithImage(CAImage::create(titleSelected));
        m_pSelectedImageView->setFrame(CADipRect(0, 0, 110, 54));
        m_pSelectedImageView->setCenterOrigin(point);
        m_pNewTabBar->addSubview(m_pSelectedImageView);
    }
    
    m_pSelectedImageView->setCenterOrigin(point);
    
}

void DMRecommendController::onButton(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    m_pNewTabBar->setSelectedAtIndex(m_nLastSelectedIndex);
}

bool DMRecommendController::showSelectedViewControllerAtIndex(unsigned int index)
{
    if (CATabBarController::showSelectedViewControllerAtIndex(index))
    {
        m_pNewTabBar->setSelectedAtIndex(index);
        return true;
    }
    return false;
}

void DMRecommendController::onSegmentedControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    CASegmentedControl* segmentedControl = dynamic_cast<CASegmentedControl*>(control);
    CC_RETURN_IF(segmentedControl == NULL);
    int index = segmentedControl->getSelectedIndex();
    if (index < 5)
    {
        m_nLastSelectedIndex = index;
        this->showSelectedViewControllerAtIndex(index);
        
        CCPoint point = segmentedControl->getItemSize();
        point.x = point.x * (index + 0.5f);
        point.y *= 0.525f;
        
        m_pSelectedImageView->setCenterOrigin(point);
    }
    else
    {
        m_pNewTabBar->setSelectedAtIndex(m_nLastSelectedIndex);
        
        DMSearchController* searchController = DMSearchController::create();
        this->presentModalViewController(searchController, true);
    }
}





    