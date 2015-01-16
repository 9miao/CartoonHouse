//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMRecommendController__
#define __HelloCpp__DMRecommendController__

#include <iostream>
#include "CartoonHouse.h"

class DMRecommendController: public CATabBarController
{
public:
    
    DMRecommendController();
    
    virtual ~DMRecommendController();
    
    bool initWithRecommendController();
    
    virtual void viewDidLoad();
    
    virtual bool showSelectedViewControllerAtIndex(unsigned int index);
    
protected:
    
    void onButton(CAControl* control, CCPoint point);
    
    void onSegmentedControl(CAControl* control, CCPoint point);

    int m_nLastSelectedIndex;
    
    CASegmentedControl* m_pNewTabBar;
    
    CAImageView* m_pSelectedImageView;
};


#endif /* defined(__HelloCpp__DMRecommendController__) */
