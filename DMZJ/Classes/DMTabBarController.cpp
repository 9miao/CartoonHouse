

#include "DMTabBarController.h"
#include "DMClassificationViewController.h"
#include "DMRecommendController.h"
#include "DMSubscribeController.h"
#include "DMMoreController.h"
#include "DMDownLoadMainController.h"
#include "DMFeedBackController.h"
#include "DMAboutUsController.h"
#include "DMChartsController.h"


DMTabBarController::DMTabBarController()
:m_pFirstViewBackGround(NULL)
,m_nLastSelectedIndex(-1)
,m_pNewTabBar(NULL)
,m_bAppear(true)
{

}

DMTabBarController::~DMTabBarController()
{

}

bool DMTabBarController::initWithTabBarController()
{
    std::vector<CAViewController*> vec;

    DMRecommendController* recommendController = new DMRecommendController();
    recommendController->initWithRecommendController();
    recommendController->autorelease();
    vec.push_back(recommendController);
    
    DMSubscribeController* subscribeController = DMSubscribeController::create();
    
    CANavigationController* navigationController = new CANavigationController();
    navigationController->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    navigationController->initWithRootViewController(subscribeController);
    vec.push_back(navigationController);
    
    
    //下载--------------------------------------start
    DMDownLoadMainController* downLoadControler = new DMDownLoadMainController();
    downLoadControler->init();
    CANavigationController* navigationControllerdown = new CANavigationController();
    navigationControllerdown->autorelease();
    navigationControllerdown->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    navigationControllerdown->initWithRootViewController(downLoadControler);
    vec.push_back(navigationControllerdown);
    //下载--------------------------------------end
   
    
    
    
    
    
    
    DMMoreController* moreController = DMMoreController::create();
    moreController->setNavigationBarItem(CANavigationBarItem::create("更多"));
    
    CANavigationController* navigationController3 = new CANavigationController();
    navigationController3->autorelease();
    navigationController3->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    navigationController3->initWithRootViewController(moreController);
    vec.push_back(navigationController3);
    
    CATabBarController::initWithViewControllers(vec);
    
    return true;
}

void DMTabBarController::viewDidLoad()
{
    CATabBarController::viewDidLoad();
    
    m_pTabBar->setDelegate(NULL);
    m_pTabBar->setVisible(false);
    
    CAImageView* bg = CAImageView::createWithImage(CAImage::create(tabBarBackground));
    bg->setFrame(m_pTabBar->getFrame());
    this->getView()->addSubview(bg);
    
    CADipRect rect = m_pTabBar->getFrame();
    rect.origin = ccpAdd(rect.origin, rect.size / 2);
    rect.size.width = MIN(rect.size.width, 768);
    m_pNewTabBar = CASegmentedControl::createWithCenter(rect, 5);
    for (int i=0; i<5; i++)
    {
        m_pNewTabBar->setBackgroundImageAtIndex(NULL, i, CAControlStateAll);
        m_pNewTabBar->setImageAtIndex(CAImage::create(menuImageNormal[i]), i, CAControlStateNormal);
        m_pNewTabBar->setImageAtIndex(CAImage::create(menuImageSelected[i]), i, CAControlStateHighlighted);
        m_pNewTabBar->setImageAtIndex(CAImage::create(menuImageSelected[i]), i, CAControlStateSelected);
        m_pNewTabBar->setTitleAtIndex(menuTitle[i], i, CAControlStateNormal);
        m_pNewTabBar->setTitleAtIndex(menuTitle[i], i, CAControlStateHighlighted);
        m_pNewTabBar->setTitleAtIndex(menuTitle[i], i, CAControlStateSelected);
        m_pNewTabBar->setTitleColorAtIndex(ccc4(92, 100, 112, 255), i, CAControlStateNormal);
        m_pNewTabBar->setTitleColorAtIndex(ccc4(87, 154, 244, 255), i, CAControlStateHighlighted);
        m_pNewTabBar->setTitleColorAtIndex(ccc4(87, 154, 244, 255), i, CAControlStateSelected);
    }
    this->getView()->addSubview(m_pNewTabBar);
    m_pNewTabBar->addTarget(this, CAControl_selector(DMTabBarController::onSegmentedControl));
    m_pNewTabBar->setSelectedAtIndex(1);
    m_nLastSelectedIndex = 1;
}

void DMTabBarController::viewDidAppear()
{
    CATabBarController::viewDidAppear();
    m_bAppear = true;
    m_pNewTabBar->setSelectedAtIndex(m_nLastSelectedIndex);
}

void DMTabBarController::viewDidDisappear()
{
    CATabBarController::viewDidDisappear();
    m_bAppear = false;
    m_pNewTabBar->setSelectedAtIndex(0);
}

void DMTabBarController::onButton(CrossApp::CAControl *control, CrossApp::CCPoint point)
{

}

bool DMTabBarController::showSelectedViewControllerAtIndex(unsigned int index)
{
    if (CATabBarController::showSelectedViewControllerAtIndex(index))
    {
        m_pNewTabBar->setSelectedAtIndex(index + 1);
        return true;
    }
    return false;
}

void DMTabBarController::onSegmentedControl(CrossApp::CAControl *control, CrossApp::CCPoint point)
{
    
    CASegmentedControl* segmentedControl = dynamic_cast<CASegmentedControl*>(control);
    CC_RETURN_IF(segmentedControl == NULL);
    int index = segmentedControl->getSelectedIndex();
    if (index == 0)
    {
        if (m_bAppear)
        {
            RootWindow::getInstance()->getDrawerController()->showLeftViewController(true);
        }
    }
    else
    {
        m_nLastSelectedIndex = index;
        this->showSelectedViewControllerAtIndex(index - 1);

        if (m_bAppear == false)
        {
            RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
        }
        
    }
}
