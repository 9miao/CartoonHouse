
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
#ifndef __HelloCpp__DMTabBarController__
#define __HelloCpp__DMTabBarController__

#include <iostream>
#include "CartoonHouse.h"
#include "CommonHttpManager.h"

class DMClassificationViewController;
class DMTabBarController: public CATabBarController
{
    
public:
    
    DMTabBarController();
    
    virtual ~DMTabBarController();
    
    bool initWithTabBarController();
    
    virtual void viewDidLoad();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    virtual bool showSelectedViewControllerAtIndex(unsigned int index);
    
protected:

    void onButton(CAControl* control, CCPoint point);
    
    void onSegmentedControl(CAControl* control, CCPoint point);

    CAButton* m_pFirstViewBackGround;
    
    int m_nLastSelectedIndex;
    
    CASegmentedControl* m_pNewTabBar;
    
    bool m_bAppear;
    
};


#endif /* defined(__HelloCpp__DMTabBarController__) */
