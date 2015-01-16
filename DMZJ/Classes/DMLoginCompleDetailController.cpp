//
//  DMLoginCompleDetailController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-7-31.
//
//

#include "DMLoginCompleDetailController.h"




DMLoginCompleDetailController::DMLoginCompleDetailController()
{
}

DMLoginCompleDetailController::~DMLoginCompleDetailController()
{
}

bool DMLoginCompleDetailController::init()
{
    if(!CAViewController::init()){
        return false;
    }
    return true;
}

void DMLoginCompleDetailController::viewDidLoad()
{
    CCRect screenR = this->getView()->getFrame();
    
    
    m_pLayoutView = CAScrollView::createWithFrame(CCRect(0, 0, screenR.size.width, screenR.size.height));
    m_pLayoutView->setBounceHorizontal(false);
    m_pLayoutView->setTouchMovedListenHorizontal(false);
    m_pLayoutView->setShowsHorizontalScrollIndicator(false);
    this->getView()->addSubview(m_pLayoutView);
    m_pLayoutView->setBackGroundColor(ccc4(244, 244, 244, 255));
    
    //昵称项
    CAView *nickLayout = this->crateItemLayout(CCRect(0, _px(30), screenR.size.width, _px(92)));
    
    CCRect nickIR = CCRect(_px(200), (_px(92)-_px(42))/2, screenR.size.width-_px(200)-_px(20), _px(42));
    m_pNickName = CATextField::createWithFrame(nickIR);
    nickLayout->addSubview(m_pNickName);
    m_pLayoutView->addSubview(nickLayout);
    
    //性别项
    CAView *genderLayout = this->crateItemLayout(CCRect(0, _px(172), screenR.size.width, _px(92)));
    CAImageView *gIcon = (CAImageView *)genderLayout->getSubviewByTag(200);
    gIcon->setImage(CAImage::create("dm_resource/login_regist/gender_m.png"));
    CALabel *gTitle = (CALabel *)genderLayout->getSubviewByTag(300);
    gTitle->setText("性别");
    m_pLayoutView->addSubview(genderLayout);
    //性别项-----(男(图))-----
    CAImageView *manImg = CAImageView::createWithFrame(CCRect(gTitle->getFrame().origin.x+_px(104)+_px(64), gTitle->getFrame().origin.y, _px(32), _px(32)));
    manImg->setImage(CAImage::create("dm_resource/login_regist/man_blue.png"));
    genderLayout->addSubview(manImg);
    //性别项-----(男(文))-----
    CALabel *manTxt = CALabel::createWithFrame(CCRect(gTitle->getFrame().origin.x+_px(104)+_px(96), gTitle->getFrame().origin.y, _px(32), _px(32)));
    manTxt ->setFontSize(_px(32));
    manTxt->setColor(CAColor_black);
    manTxt->setText("男");
    genderLayout->addSubview(manTxt);
    
    //性别项-----(女(图))-----
    CAImageView *wmanImg = CAImageView::createWithFrame(CCRect(gTitle->getFrame().origin.x+_px(304)+_px(64), gTitle->getFrame().origin.y, _px(32), _px(32)));
    wmanImg->setImage(CAImage::create("dm_resource/login_regist/wom_gray.png"));
    genderLayout->addSubview(wmanImg);
    //性别项-----(女(文))-----
    CALabel *wmanTxt = CALabel::createWithFrame(CCRect(gTitle->getFrame().origin.x+_px(405), gTitle->getFrame().origin.y, _px(32), _px(32)));
    wmanTxt ->setFontSize(_px(32));
    wmanTxt->setColor(CAColor_black);
    wmanTxt->setText("女");
    genderLayout->addSubview(wmanTxt);
    
    
    //生日项
    CAView *birithLayout = this->crateItemLayout(CCRect(0, _px(262), screenR.size.width, _px(92)));
    CAImageView *bIcon = (CAImageView *)birithLayout->getSubviewByTag(200);
    bIcon->setImage(CAImage::create("dm_resource/login_regist/comp_details.png"));
    CALabel *bTitle = (CALabel *)birithLayout->getSubviewByTag(300);
    bTitle->setText("生日");
    m_pLayoutView->addSubview(birithLayout);
    
    //生日项（生日）
    CALabel *birthday = CALabel::createWithFrame(CCRect(_px(300), _px(30), _px(200), _px(32)));
    birthday ->setFontSize(_px(32));
    birthday->setColor(CAColor_black);
    birthday->setText("1988-9-9");
    birithLayout->addSubview(birthday);
    //确定按钮
    CAButton *confirmBtn = CAButton::createWithFrame(CCRect(_px(30), birithLayout->getFrame().origin.y+_px(200), screenR.size.width-_px(60), _px(80)),CAButtonTypeCustom);
    CAScale9ImageView *confirmDef = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/login_bac.png"));
    CAScale9ImageView *confirmHigh = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/login_bac.png"));
    confirmHigh->setColor(ccc4(166, 166, 166, 255));
    confirmBtn->setBackGroundViewForState(CAControlStateNormal, confirmDef);
    confirmBtn->setBackGroundViewForState(CAControlStateHighlighted, confirmHigh);
    confirmBtn->setTitleForState(CAControlStateAll, "确定");
    m_pLayoutView->addSubview(confirmBtn);
    
    
    //跳过按钮
    CAButton *jumpBtn = CAButton::createWithFrame(CCRect(_px(30), confirmBtn->getFrame().origin.y+confirmBtn->getFrame().size.height+_px(60), screenR.size.width-_px(60), _px(80)),CAButtonTypeCustom);
    CAScale9ImageView *jumpDef = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/input_bac.png"));
    CAScale9ImageView *jumpHigh = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/login_regist/input_bac.png"));
    jumpHigh->setColor(ccc4(166, 166, 166, 255));
    jumpBtn->setBackGroundViewForState(CAControlStateNormal, jumpDef);
    jumpBtn->setBackGroundViewForState(CAControlStateHighlighted, jumpHigh);
    jumpBtn->setTitleForState(CAControlStateAll, "跳过");
    m_pLayoutView->addSubview(jumpBtn);
    
    
}

void DMLoginCompleDetailController::viewDidUnload()
{
    
}

CAView *DMLoginCompleDetailController::crateItemLayout(const CCRect &rect)
{
    CAView *mainView = CAView::createWithFrame(rect);
    mainView->setColor(CAColor_white);
    
    CAView *topLine = CAView::createWithFrame(CCRect(0,0,rect.size.width,_px(1)));
    topLine->setColor(ccc4(177, 177, 177, 255));
    mainView->addSubview(topLine);
    
    CAView *botLine = CAView::createWithFrame(CCRect(0,rect.size.height-_px(1),rect.size.width,_px(1)));
    botLine->setColor(ccc4(177, 177, 177, 255));
    botLine->setTag(100);
    mainView->addSubview(botLine);
    
    CAImageView *titleIcon = CAImageView::createWithFrame(CCRect(_px(30), (rect.size.height-_px(35))/2, _px(35), _px(35)));
    titleIcon->setImage(CAImage::create("dm_resource/login_regist/nick_n.png"));
    titleIcon->setTag(200);
    mainView->addSubview(titleIcon);
    
    CALabel *txtLable = CALabel::createWithFrame(CCRect(_px(85), (rect.size.height-_px(32))/2, _px(64), _px(32)));
    txtLable->setTextAlignment(CATextAlignmentCenter);
    txtLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    txtLable->setText("昵称");
    txtLable->setFontSize(_px(32));
    txtLable->setColor(ccc4(153, 153, 153, 255));
    txtLable->setTag(300);
    mainView->addSubview(txtLable);
    
    CAView *bottomLine = CAView::createWithFrame(CCRect(0,rect.size.height-_px(1),rect.size.width,_px(1)));
    bottomLine->setColor(ccc4(177, 177, 177, 255));
    bottomLine->setTag(400);
    mainView->addSubview(bottomLine);
    
    return mainView;
}








