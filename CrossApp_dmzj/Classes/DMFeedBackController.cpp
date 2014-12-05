//
//  DMFeedBackController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-7-30.
//
//

#include "DMFeedBackController.h"
#include "DMUserManager.h"

DMFeedBackController::DMFeedBackController()
{

}

DMFeedBackController::~DMFeedBackController()
{
    
}

bool DMFeedBackController::init()
{
    if (!CAViewController::init()) {
        return false;
    }
    return true;
}

void DMFeedBackController::viewDidLoad()
{
    CCRect rect = CCRect(_px(30), _px(30), this->getView()->getFrame().size.width-_px(60), _px(360));
    CCRect inputRect = CCRect(0, 0, this->getView()->getFrame().size.width-_px(60), _px(360));
    
    m_pFeedBackInput = CATextView::createWithFrame(inputRect);
    
    m_pFeedBackInput->setBackGroundColor(CAColor_gray);
    
    m_pFeedBackInput->setFontSize(_px(30));
    
    m_pFeedBackInput->setFontColor(CAColor_black);
    
    m_pInputBac = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/setting/more_set_exit.png"));
    
    m_pInputBac->setFrame(rect);
    
    m_pInputBac->addSubview(m_pFeedBackInput);

    this->getView()->addSubview(m_pInputBac);
    
    CABarButtonItem *barButtonItem = CABarButtonItem::create("发送", NULL, NULL);
    
    this->getNavigationBarItem()->addRightButtonItem(barButtonItem);
    
    barButtonItem->setTarget(this, CAControl_selector(DMFeedBackController::onSendButtonClick));
    
}




void DMFeedBackController::viewDidUnload()
{
    
    
}


void DMFeedBackController::onSendButtonClick()
{

}

void DMFeedBackController::onFeedBackFinish()
{
    CAAlertView * alert = CAAlertView::createWithText("提示", "提交成功!", "确定", NULL);
    alert->setTarget(this, CAAlertView_selector(DMFeedBackController::onSuccessBtnClick));
    alert->show();
}

void DMFeedBackController::onSuccessBtnClick(int index){
    
    this->getNavigationController()->popViewControllerAnimated(true);
    
}



