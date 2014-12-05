//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMFeedBackController__
#define __HelloCpp__DMFeedBackController__

#include <iostream>
#include "CartoonHouse.h"

class DMFeedBackController :public CAViewController
{
public:
    
    DMFeedBackController();
    
    ~DMFeedBackController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    void onFeedBackFinish();
    
    void onSuccessBtnClick(int index);
    
private:
    
    
    CATextView * m_pFeedBackInput;
    
    CAScale9ImageView * m_pInputBac;
    
    void onSendButtonClick();
    
    
    
    
};

#endif /* defined(__HelloCpp__DMFeedBackController__) */
