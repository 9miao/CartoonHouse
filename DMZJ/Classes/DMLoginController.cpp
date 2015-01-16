//
//  DMLoginController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-7-31.
//
//

#include "DMLoginController.h"
#include "DMLoginRegistController.h"
#include "DMUserManager.h"
#include "ThreePartyHelper.h"

static DMLoginController *_instance = NULL;

DMLoginController *DMLoginController::getInstance()
{
    return _instance;
}

DMLoginController::DMLoginController()
:m_pUserLoginDelegate(NULL)
,m_pAccount(NULL)
,m_pPassword(NULL)
,m_pRegitstBtn(NULL)
,m_pLoginBtn(NULL)
,m_pQQLogin(NULL)
,m_pSinaLogin(NULL)
,m_pLayoutView(NULL)
{
}

DMLoginController::~DMLoginController()
{
}

bool DMLoginController::init()
{
    
    if (!CAViewController::init()) {
        return false;
    }
    
    CANavigationBarItem *naviBarItem = CANavigationBarItem::create("登陆");
    CABarButtonItem *leftItem = CABarButtonItem::create("", CAImage::create("source_material/btn_left_white.png"), NULL);
    naviBarItem->addLeftButtonItem(leftItem);
    leftItem->setTarget(this, CAControl_selector(DMLoginController::onBackBtnClick));
    this->setNavigationBarItem(naviBarItem);
    
    _instance = this;
    
    return true;
}
//dm_resource/login_regist/reg_bac.png
void DMLoginController::viewDidLoad()
{
    
    CCRect screenF = this->getView()->getFrame();
    
    m_pLayoutView = CAScrollView::createWithFrame(CCRect(0, 0, screenF.size.width, screenF.size.height));
    m_pLayoutView->setBounceHorizontal(false);
    m_pLayoutView->setTouchMovedListenHorizontal(false);
    m_pLayoutView->setShowsHorizontalScrollIndicator(false);
    this->getView()->addSubview(m_pLayoutView);
    //用户名
    CCRect accR = CCRect(_px(30), _px(30), screenF.size.width - _px(60), _px(82));
    m_pAccount = CATextField::createWithFrame(accR);
    m_pAccount->setHoriMargins(_px(20));
    m_pAccount->setPlaceHolder("账号(直接点登陆)");
    
    m_pLayoutView->CrossApp::CAView::addSubview(m_pAccount);
    
    //用户密码
    CCRect pwdR = CCRect(_px(30), accR.origin.y+accR.size.height+_px(30), screenF.size.width - _px(60), _px(82));
    m_pPassword = CATextField::createWithFrame(pwdR);
    m_pPassword->setHoriMargins(_px(20));
    m_pPassword->setPlaceHolder("密码(直接点登陆)");
    m_pPassword->setInputType(KEY_BOARD_INPUT_PASSWORD);
    m_pLayoutView->CrossApp::CAView::addSubview(m_pPassword);
    
    
    //注册按钮
    CAScale9ImageView *regBac = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/reg_bac.png"));
    CAScale9ImageView *regBacLig = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/reg_bac.png"));
    regBacLig->setColor(ccc4(166, 166, 166, 255));
    CCRect regR = CCRect(pwdR.origin.x, pwdR.origin.y+pwdR.size.height+_px(60), _px(230), _px(80));
    m_pRegitstBtn = CAButton::createWithFrame(regR, CAButtonTypeCustom);
    m_pRegitstBtn->setTitleForState(CAControlStateNormal, "注册");
    m_pRegitstBtn->setBackGroundViewForState(CAControlStateNormal, regBac);
    m_pRegitstBtn->setBackGroundViewForState(CAControlStateHighlighted, regBacLig);
    m_pLayoutView->CrossApp::CAView::addSubview(m_pRegitstBtn);
    m_pRegitstBtn->addTarget(this, CAControl_selector(DMLoginController::onRegistClick), CAControlEventTouchUpInSide);
    
    
    //登陆按钮
    CCRect logR = CCRect(screenF.size.width-_px(260), pwdR.origin.y+pwdR.size.height+_px(60), _px(230), _px(80));;
    m_pLoginBtn = CAButton::createWithFrame(logR, CAButtonTypeCustom);
    CAScale9ImageView *logBac = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/login_bac.png"));
    CAScale9ImageView *logBacHig = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/login_bac.png"));
    logBacHig->setColor(ccc4(166, 166, 166, 255));
    m_pLoginBtn->setBackGroundViewForState(CAControlStateNormal, logBac);
    m_pLoginBtn->setBackGroundViewForState(CAControlStateHighlighted, logBacHig);
    m_pLoginBtn->setTitleForState(CAControlStateAll, "登陆");
    m_pLoginBtn->setTitleColorForState(CAControlStateAll, CAColor_white);
    m_pLayoutView->addSubview(m_pLoginBtn);
    m_pLoginBtn->addTarget(this, CAControl_selector(DMLoginController::onLoginClick), CAControlEventTouchUpSide);
    
    //使用三方登陆提示图片
    float pH = screenF.size.width * 27 / 640;
    CCRect tLRect = CCRect(screenF.size.width/2, logR.origin.y+logR.size.height+_px(92), screenF.size.width, pH);
    CAImageView *thirdLog = CAImageView::createWithCenter(tLRect);
    thirdLog->setImage(CAImage::create("dm_resource/login_regist/third_p_login.png"));
    m_pLayoutView->addSubview(thirdLog);
    
    //QQ
    CCRect qqR = CCRect(screenF.size.width/4, (tLRect.origin.y+tLRect.size.height+_px(120)), _px(120), _px(120));
    m_pQQLogin = CAButton::createWithCenter(qqR, CAButtonTypeCustom);
    CAImageView *qqN = CAImageView::createWithImage(CAImage::create("dm_resource/login_regist/more_qq_ricon.png"));
    CAImageView *qqH = CAImageView::createWithImage(CAImage::create("dm_resource/login_regist/more_qq_ricon.png"));
    qqH->setColor(ccc4(166, 166, 166, 255));
    m_pQQLogin->setBackGroundViewForState(CAControlStateNormal, qqN);
    m_pQQLogin->setBackGroundViewForState(CAControlStateHighlighted, qqH);
    m_pLayoutView->CrossApp::CAView::addSubview(m_pQQLogin);
    m_pQQLogin->setTag(0);
    m_pQQLogin->addTarget(this, CAControl_selector(DMLoginController::onThreePartyIconClick), CAControlEventTouchUpInSide);
    //QQ文字
    CALabel *qqLable = CALabel::createWithCenter(CCRect(screenF.size.width/4, qqR.origin.y+qqR.size.height-_px(30), qqR.size.width, _px(30)));
    qqLable->setColor(ccc4(60, 60, 60, 255));
    qqLable->setFontSize(_px(24));
    qqLable->setText("腾讯QQ");
    qqLable->setTextAlignment(CATextAlignmentCenter);
    qqLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_pLayoutView->CrossApp::CAView::addSubview(qqLable);
    
    
    //Sina
    CCRect sinaR = CCRect(screenF.size.width*0.75, (tLRect.origin.y+tLRect.size.height+_px(120)), _px(120), _px(120));
    m_pSinaLogin = CAButton::createWithCenter(sinaR, CAButtonTypeCustom);
    CAImageView *sinaN = CAImageView::createWithImage(CAImage::create("dm_resource/login_regist/more_sina_ricon.png"));
    CAImageView *sinaH = CAImageView::createWithImage(CAImage::create("dm_resource/login_regist/more_sina_ricon.png"));
    sinaH->setColor(ccc4(166, 166, 166, 255));
    m_pSinaLogin->setBackGroundViewForState(CAControlStateNormal, sinaN);
    m_pSinaLogin->setBackGroundViewForState(CAControlStateHighlighted, sinaH);
    m_pLayoutView->CrossApp::CAView::addSubview(m_pSinaLogin);
    m_pSinaLogin->setTag(1);
    m_pSinaLogin->addTarget(this, CAControl_selector(DMLoginController::onThreePartyIconClick), CAControlEventTouchUpInSide);
    
    //Sina文字
    CALabel *sinaLable = CALabel::createWithCenter(CCRect(screenF.size.width*0.75, sinaR.origin.y+sinaR.size.height-_px(30), sinaR.size.width, _px(30)));
    sinaLable->setColor(ccc4(60, 60, 60, 255));
    sinaLable->setFontSize(_px(24));
    sinaLable->setText("新浪微博");
    sinaLable->setTextAlignment(CATextAlignmentCenter);
    sinaLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_pLayoutView->CrossApp::CAView::addSubview(sinaLable);
    
}


void DMLoginController::onThreePartyIconClick(CAControl* control, CCPoint point)
{
//    CAButton *button = (CAButton*)control;
//    ThreePartyHelper mHelper ;
//    switch (button->getTag()) {
//        case 0:
//            mHelper.onThreePartyLogin(1);
//            break;
//        case 1:
//            mHelper.onThreePartyLogin(2);
//            break;
//        default:
//            break;
//    }
}

void DMLoginController::addUserLoginDelegate(DMUserLoginDelegate *delegate)
{
    this->m_pUserLoginDelegate = delegate;
}

void DMLoginController::onRegistClick()
{
    DMLoginRegistController *registController = new DMLoginRegistController();
    registController->init();
    registController->setNavigationBarItem(CANavigationBarItem::create("注册帐号"));
    this->getNavigationController()->pushViewController(registController, true);
}

void DMLoginController::onLoginRequestFinished(HttpResponseStatus status, const CSJson::Value& json)
{
    
    const CSJson::Value& value = json["data"];
    
    int result = json["result"].asInt();
    
    CCLog("threePaytyLogin=%s",value.toStyledString().c_str());
    
    if (result==1)
    {
        printf("登陆成功了");
                std::string uid = value["uid"].asString();
        std::string nickname = value["nickname"].asString();
        std::string photo = value["photo"].asString();
        
        //持久化
        DMUser user ;
        user.uid = uid;
        user.nickname = nickname;
        user.photo = photo;
        user.status = online;
        DMUserManager::sharedDMUserManager()->userLogin(user);
        
        
        CAAlertView *alertView = CAAlertView::createWithText("提示！", "登陆成功！", "确定", NULL);
        alertView->show();
        
        if (m_pUserLoginDelegate)
        {
            m_pUserLoginDelegate->onActionPerformed(LoginActionLoginSuccess);
        }
        
        this->dismissModalViewController(true);
        RootWindow::getInstance()->getDrawerController()->hideLeftViewController(false);
    }
    else
    {
        CAAlertView *alertView = CAAlertView::createWithText("提示！", "登陆失败！", "确定", NULL);
        alertView->show();
    }
   
}

void DMLoginController::onLoginClick()
{
    string accout = m_pAccount->getText();
    string passwd = m_pPassword->getText();
    
    accout = "liuguoyan21@126.com";
    passwd = "123456";
    
    if (accout.length()==0 || passwd.length()==0) {
        CAAlertView *alertView = CAAlertView::createWithText("提示！", "用户名或密码不能为空！", "确定", NULL);
        alertView->show();
        return;
    }
    
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeUserLogin, accout, passwd);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMLoginController::onLoginRequestFinished));
    
    
    
}

void DMLoginController::onBackBtnClick()
{
    RootWindow::getInstance()->getDrawerController()->hideLeftViewController(false);
    this->dismissModalViewController(true);
}

void DMLoginController::viewDidUnload()
{
}


void DMLoginController::threePartyLand(std::string channel,std::string token,std::string uid)
{
    CCLog("----- threePartyLand ----- channel = %s,token= %s,uid = %s",channel.c_str(),token.c_str(),uid.c_str());
    
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeThreePartyLand);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMLoginController::onLoginRequestFinished));
}

void DMLoginController::onThreePartyLandFinish(HttpResponseStatus status, const CSJson::Value& json)
{
    
}













