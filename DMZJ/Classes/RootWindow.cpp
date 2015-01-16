

#include "RootWindow.h"
#include "DMTabBarController.h"
#include "DMClassificationViewController.h"

static RootWindow* _window = NULL;

RootWindow* RootWindow::getInstance()
{
    if (_window == NULL)
    {
        _window = new RootWindow();
        _window->init();
        _window->autorelease();
    }
    return _window;
    
}

RootWindow::RootWindow()
:m_pRootNavigationController(NULL)
,m_pRootTabBarController(NULL)
,m_pRootDrawerController(NULL)
,m_pSecondNavigationController(NULL)
,m_bAlertShow(false)
{
    CAApplication::getApplication()->getKeypadDispatcher()->addDelegate(this);
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "brief.db";
    sqlite3_open(fullPath.c_str(), &m_pSqlite3);
}

RootWindow::~RootWindow()
{
    CAApplication::getApplication()->getKeypadDispatcher()->removeDelegate(this);
    sqlite3_close(m_pSqlite3);
}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    this->setColor(ccc4(87, 154, 244, 255));
    
    DMTabBarController* tabBarController = new DMTabBarController();
    tabBarController->initWithTabBarController();
    
    CANavigationController* nav = new CANavigationController();
    nav->initWithRootViewController(tabBarController);
    nav->setNavigationBarHidden(true, false);
    
    DMClassificationViewController* classificationViewController = DMClassificationViewController::create();

    CADrawerController* drawer = new CADrawerController();
    drawer->initWithController(classificationViewController, nav, this->getBounds().size.width * 5/6.0f);
    drawer->setBackgroundView(CAView::createWithColor(CAColor_black));
    tabBarController->release();
    nav->release();
    
    CCRect rect = this->getBounds();
    rect.size.height /= 2;
    rect.origin.y = rect.size.height;
    drawer->getBackgroundView()->addSubview(CAView::createWithFrame(rect));
    
    this->setRootViewController(drawer);
    drawer->release();
    
    m_pRootTabBarController = tabBarController;
    m_pRootNavigationController = nav;
    m_pRootDrawerController = drawer;

    
    return true;
}

void RootWindow::keyBackClicked()
{
    if (this->getModalViewController())
    {
        this->dismissModalViewController(true);
    }
    else if (this->getDrawerController()->isShowLeftViewController())
    {
        this->getDrawerController()->hideLeftViewController(true);
    }
    else if (this->getRootNavigationController()->getViewControllerCount() > 1)
    {
        this->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (this->getRootTabBarController()->getSelectedViewController()->isEqual(m_pSecondNavigationController)
             && this->getSecondNavigationController()->getViewControllerCount() > 1)
    {
        this->getSecondNavigationController()->popViewControllerAnimated(true);
    }
    else
    {
        CC_RETURN_IF(m_bAlertShow);
        m_bAlertShow = true;
        CAAlertView* alertView = CAAlertView::createWithText("提示", "确定要退出？", "确定", "取消", NULL);
        alertView->setTarget(this, CAAlertView_selector(RootWindow::onAlertView));
        alertView->show();
    }
}

void RootWindow::keyMenuClicked()
{

}

void RootWindow::onAlertView(int index)
{
    if (index == 0)
    {
        CAApplication::getApplication()->end();
    }
    m_bAlertShow = false;
}
