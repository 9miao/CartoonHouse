//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMLoginController__
#define __HelloCpp__DMLoginController__

#include <iostream>
#include "CartoonHouse.h"

typedef enum {
    LoginActionLoginSuccess,
    LoginActionLoginFailed
}LoginAction;

class DMUserLoginDelegate{
public:
    virtual void onActionPerformed(LoginAction action)=0;
};

class DMLoginController :public CAViewController
{
public:
    
    static DMLoginController *getInstance();
    
    DMLoginController();
    
    ~DMLoginController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    void onRegistClick();
    
    void onLoginClick();
    
    void onBackBtnClick();
    
    void onLoginRequestFinished(HttpResponseStatus status, const CSJson::Value& json);
    
    void addUserLoginDelegate(DMUserLoginDelegate *delegate);
    
    void threePartyLand(std::string channel,std::string token,std::string uid);
    
    void onThreePartyLandFinish(HttpResponseStatus status, const CSJson::Value& json);
    
    
    void onThreePartyIconClick(CAControl* control, CCPoint point);
    
    
private:
    
    DMUserLoginDelegate *m_pUserLoginDelegate;
    
    CATextField * m_pAccount;
    CATextField * m_pPassword;
    
    
    CAButton *m_pRegitstBtn;
    CAButton *m_pLoginBtn;
    
    CAButton *m_pQQLogin;
    CAButton *m_pSinaLogin;
    
    CAScrollView *m_pLayoutView;
    
};




#endif /* defined(__HelloCpp__DMLoginController__) */
