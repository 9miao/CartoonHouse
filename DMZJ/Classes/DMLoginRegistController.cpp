//
//  DMLoginRegistController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-8-1.
//
//

#include "DMLoginRegistController.h"


DMLoginRegistController::DMLoginRegistController()
{
}

DMLoginRegistController::~DMLoginRegistController()
{
}

bool DMLoginRegistController::init()
{
    if (!CAViewController::init()) {
        return false;
    }
    return true;
}

void DMLoginRegistController::viewDidLoad()
{
    
    
    CCRect screenR = this->getView()->getFrame();
    
    m_pLayoutView = CAScrollView::createWithFrame(CCRect(0, 0, screenR.size.width, screenR.size.height));
    m_pLayoutView->setBounceHorizontal(false);
    m_pLayoutView->setTouchMovedListenHorizontal(false);
    m_pLayoutView->setShowsHorizontalScrollIndicator(false);
    this->getView()->addSubview(m_pLayoutView);
    m_pLayoutView->setBackGroundColor(ccc4(244, 244, 244, 255));
    
    //头像框
    CCRect headR = CCRect(screenR.size.width/2-_px(100), _px(30), _px(200), _px(200));
    m_pUserHead = CAButton::createWithFrame(headR,CAButtonTypeCustom);
    CAImageView *headBac = CAImageView::createWithFrame(headR);
    headBac->setImage(CAImage::create("dm_resource/login_regist/head_def.png"));
    m_pUserHead->setBackGroundViewForState(CAControlStateNormal, headBac);
    m_pLayoutView->addSubview(m_pUserHead);
    m_pUserHead->addTarget(this, CAControl_selector(DMLoginRegistController::onHeadClick), CAControlEventTouchUpInSide);
    
    int margin = _px(20);
    //昵称
    m_pNickName = CATextField::createWithFrame(CCRect(_px(30), _px(260), screenR.size.width-_px(60), _px(80)));
    m_pNickName->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/input_bac.png")));
    m_pNickName->setPlaceHolder("昵称");
    m_pNickName->setHoriMargins(margin);
    m_pLayoutView->addSubview(m_pNickName);
    m_pNickName->setTag(101);
    m_pNickName->setDelegate(this);
    
    
    //手机号或者邮箱
    m_pTeleEmailNum = CATextField::createWithFrame(CCRect(_px(30), m_pNickName->getFrame().origin.y+m_pNickName->getFrame().size.height+_px(30), screenR.size.width-_px(60), _px(80)));
    m_pTeleEmailNum->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/input_bac.png")));
    m_pLayoutView->addSubview(m_pTeleEmailNum);
    m_pTeleEmailNum->setTag(102);
    m_pTeleEmailNum->setPlaceHolder("邮箱");
    m_pTeleEmailNum->setHoriMargins(margin);
    m_pTeleEmailNum->setDelegate(this);
    
    //密码
    m_pPassword = CATextField::createWithFrame(CCRect(_px(30), m_pTeleEmailNum->getFrame().origin.y+m_pTeleEmailNum->getFrame().size.height+_px(30), screenR.size.width-_px(60), _px(80)));
    m_pPassword->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/input_bac.png")));
    m_pPassword->setHoriMargins(margin);
    m_pPassword->setPlaceHolder("创建密码(6-16位)");
    m_pPassword->setInputType(KEY_BOARD_INPUT_PASSWORD);
    m_pLayoutView->addSubview(m_pPassword);
    
    //重复密码
    m_pConfirmPwd = CATextField::createWithFrame(CCRect(_px(30), m_pPassword->getFrame().origin.y+m_pPassword->getFrame().size.height+_px(30), screenR.size.width-_px(60), _px(80)));
    m_pConfirmPwd->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/input_bac.png")));
    
    m_pConfirmPwd->setHoriMargins(margin);
    m_pConfirmPwd->setPlaceHolder("确认密码");
    m_pConfirmPwd->setInputType(KEY_BOARD_INPUT_PASSWORD);
    m_pLayoutView->addSubview(m_pConfirmPwd);
    
    CAButton *confirmBtn = CAButton::createWithFrame(CCRect(_px(30), m_pConfirmPwd->getFrame().origin.y+ m_pConfirmPwd->getFrame().size.height +_px(60), screenR.size.width-_px(60), _px(80)),CAButtonTypeCustom);
    CAScale9ImageView *confirmDef = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/login_bac.png"));
    CAScale9ImageView *confirmHigh = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/login_bac.png"));
    confirmHigh->setColor(ccc4(166, 166, 166, 255));
    confirmBtn->setBackGroundViewForState(CAControlStateNormal, confirmDef);
    confirmBtn->setBackGroundViewForState(CAControlStateHighlighted, confirmHigh);
    confirmBtn->setTitleForState(CAControlStateAll, "确定");
    confirmBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onSumbitBtnClick), CAControlEventTouchUpInSide);
    m_pLayoutView->addSubview(confirmBtn);
    
    m_pLayoutView->setViewSize(CCSize(screenR.size.width, confirmBtn->getFrame().origin.y + confirmBtn->getFrame().size.height+_px(40)));
    
    addPicChoserLayer();
    
}

void DMLoginRegistController::onSumbitBtnClick()
{
    if (m_pPassword->getText().compare(m_pConfirmPwd->getText())) {
        CAAlertView *alert = CAAlertView::createWithText("提示!", "密码不相同", "确定", NULL);
        alert->show();
        return;
    }
    
    submitUserInfo();
}

void DMLoginRegistController::submitUserInfo()
{
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeRegist,
                                                             m_pNickName->getText(),
                                                             m_pTeleEmailNum->getText(),
                                                             m_pPassword->getText());
//    key_value["nickname"]="piaomiaohei2";
//    key_value["email"]="liuguoyan@139.com";
//    key_value["passwd"]="12346";
    
    std::string filename = CCFileUtils::sharedFileUtils()->getWritablePath()+"head.jpg";
    bool fileexists = CCFileUtils::sharedFileUtils()->isFileExist(filename);
    if (!fileexists) {
        CAAlertView *alert = CAAlertView::createWithText("提示", "请选择图片", "",NULL);
        alert->show();
        return;
    }
    
    CommonHttpManager::getInstance()->send_postFile(s_http_url.url, s_http_url.key_value, "", this, CommonHttpJson_selector(DMLoginRegistController::onSubmitUserInfoComplete));
    
}

void DMLoginRegistController::onSubmitUserInfoComplete(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    CCLog("onSubmitUserInfoComplete");
    
    int result = json["result"].asInt();
    
    if (result==0)
    {
        std::string msg = json.isMember("msg") ? json["msg"].asString() : "注册失败!";
        CAAlertView *alertView = CAAlertView::createWithText("提示", msg.c_str(), "确定", NULL);
        alertView->show();
    }
    else
    {
        CCLog("注册成功。。。。。。。。");
    }
}


bool DMLoginRegistController::onTextFieldDetachWithIME(CATextField * sender)
{

    //昵称
    if (sender->getTag()==101)
    {
        verifyUser(sender->getText(),"nickname");
    }
    //手机或者邮箱
    else if (sender->getTag()==102)
    {
        verifyUser(sender->getText(),"email");
    }
    
    return false;
}

void DMLoginRegistController::verifyUser(const std::string &name,const std::string type)
{
    
    if (name.length()==0) {
        return;
    }
    
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeRegistVerify,
                                                             type,
                                                             name);
    
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMLoginRegistController::onVerifyUserFinished));
}

void DMLoginRegistController::onVerifyUserFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    int result = json["result"].asInt();
    
    if (result==0) {
        std::string msg = json.isMember("msg") ? json["msg"].asString() : "验证错误!";
        CAAlertView *alertView = CAAlertView::createWithText("提示", msg.c_str(), "确定", NULL);
        alertView->show();
    }
    
}


void DMLoginRegistController::addPicChoserLayer()
{
    m_pPicChoserLayer = CAView::createWithFrame(this->getView()->getFrame(), ccc4(100, 100, 100, 100));
    m_pPicChoserLayer->setVisible(false);
    this->getView()->addSubview(m_pPicChoserLayer);
    
    CCSize hViewSize = this->getView()->getFrame().size;
    CCRect cancelR = CCRect(_px(10), hViewSize.height - _px(90), hViewSize.width-_px(20), _px(80));
    CAButton *cancelBtn = CAButton::createWithFrame(cancelR, CAButtonTypeRoundedRect);
    cancelBtn->setTitleForState(CAControlStateAll, "取消");
    cancelBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onCancelBtnClick), CAControlEventTouchUpInSide);
    m_pPicChoserLayer->addSubview(cancelBtn);
    
    CCRect albumR = CCRect(_px(10), cancelR.origin.y - _px(90), hViewSize.width-_px(20), _px(80));
    CAButton *albumBtn = CAButton::createWithFrame(albumR, CAButtonTypeRoundedRect);
    albumBtn->setTitleForState(CAControlStateAll, "从相册选择");
    albumBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onAlbumBtnClick), CAControlEventTouchUpInSide);
    m_pPicChoserLayer->addSubview(albumBtn);
    
    CCRect cameraR = CCRect(_px(10), albumR.origin.y - _px(90), hViewSize.width-_px(20), _px(80));
    CAButton *cameraBtn = CAButton::createWithFrame(cameraR, CAButtonTypeRoundedRect);
    cameraBtn->setTitleForState(CAControlStateAll, "拍照");
    cameraBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onCameraBtnClick), CAControlEventTouchUpInSide);
    m_pPicChoserLayer->addSubview(cameraBtn);

}

void DMLoginRegistController::getSelectedImage(CAImage *image)
{
    //std::string filename = CCFileUtils::sharedFileUtils()->getWritablePath() + "head.jpg";
    CAImageView *bac = CAImageView::createWithFrame(m_pUserHead->getFrame());
    bac->setImage(image);
    m_pUserHead->setBackGroundViewForState(CAControlStateNormal, bac);
}

void DMLoginRegistController::onHeadClick()
{
    if (!m_pPicChoserLayer->isVisible()) {
        m_pPicChoserLayer->setVisible(true);
    }
}

void DMLoginRegistController::onCameraBtnClick()
{
    CADevice::openCamera(this);
    m_pPicChoserLayer->setVisible(false);
}

void DMLoginRegistController::onAlbumBtnClick()
{
    CADevice::openAlbum(this);
    m_pPicChoserLayer->setVisible(false);
}

void DMLoginRegistController::onCancelBtnClick()
{
    m_pPicChoserLayer->setVisible(false);
}

void DMLoginRegistController::viewDidUnload()
{
    
}



















