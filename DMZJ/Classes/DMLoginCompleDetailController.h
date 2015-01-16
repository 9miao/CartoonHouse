//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMLoginCompleDetailController__
#define __HelloCpp__DMLoginCompleDetailController__

#include <iostream>
#include "CartoonHouse.h"

class DMLoginCompleDetailController :public CAViewController
{
public:
    
    DMLoginCompleDetailController();
    
    ~DMLoginCompleDetailController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    CAView* crateItemLayout(const CCRect &rect);
    
private:
    
    CAScrollView *m_pLayoutView;
    
    CATextField *m_pNickName;
    
    CAButton * m_pConfirmButton;
    
    CAButton *m_pSkipButton;
    
    
};


#endif /* defined(__HelloCpp__DMLoginCompleDetailController__) */
