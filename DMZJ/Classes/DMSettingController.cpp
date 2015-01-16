//
//  DMSettingController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-7-29.
//
//

#include "DMSettingController.h"
#include "DMUserManager.h"


//标题色
#define TITLE_COLOR ccc4(153,153,153,255)
//标题字号
#define TITLE_SIZE 28
//内容文字颜色
#define CONTENT_COLOR ccc4(60,60,60,255)
//标题区的背影色
#define TITLE_BAC_COLOR ccc4(244,244,244,255)

//Cell中间的颜色
#define CELL_ITEM_COLOR ccc4(177, 177, 177, 255)


DMSettingController::DMSettingController()
:m_pSettingmanger(new DMSettingManager())
,m_pLoginOutBtn(NULL)
,m_pWanderOraintaionLayer(NULL)
,m_pTableView(NULL)
{
    
}

DMSettingController::~DMSettingController()
{
    CC_SAFE_DELETE(m_pSettingmanger);

}

bool DMSettingController::init()
{
    if (!CAViewController::init()) {
        return false;
    }
    return true;
}

void DMSettingController::viewDidLoad()
{
    this->m_pTableView = CATableView::createWithFrame(this->getView()->getBounds());
    
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setTableViewDelegate(this);
    
    m_pTableView->setBackGroundColor(TITLE_BAC_COLOR);
    
    this->getView()->addSubview(m_pTableView);
    
}

void DMSettingController::viewDidAppear()
{
    DMUser user = DMUserManager::sharedDMUserManager()->onLineUser();
    if (user.uid.length()>0 && user.status ==online) {
        onUserOnline();
    }else{
        onUserOffLine();
    }
}

void DMSettingController::onActionPerformed(LoginAction action)
{
    if (action == LoginActionLoginSuccess) {
        onUserOnline();
    }
}

void DMSettingController::onUserOnline()
{
    CC_RETURN_IF(m_pLoginOutBtn == NULL);
    m_pLoginOutBtn->setTitleForState(CAControlStateAll, "退出");
    m_pLoginOutBtn->addTarget(this, CAControl_selector(DMSettingController::userLogout), CAControlEventTouchUpInSide);

}  


void DMSettingController::onUserOffLine()
{
    CC_RETURN_IF(m_pLoginOutBtn == NULL);
    m_pLoginOutBtn->setTitleForState(CAControlStateAll, "登陆");
    m_pLoginOutBtn->addTarget(this, CAControl_selector(DMSettingController::userLogin), CAControlEventTouchUpInSide);
}


void DMSettingController::userLogin()
{
    
    DMLoginController *controller = new DMLoginController();
    controller->addUserLoginDelegate(this);
    controller->init();
    controller->autorelease();
    
    CANavigationController *nController = new CANavigationController();
    nController->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    nController->initWithRootViewController(controller);
    nController->autorelease();
    
    this->presentModalViewController(nController, true);
    
}

void DMSettingController::addWanderOraintationLayer()
{
    
    m_pWanderOraintaionLayer = CAView::createWithFrame(this->getView()->getFrame(), ccc4(100, 100, 100, 100));
    m_pWanderOraintaionLayer->setVisible(false);
    this->getView()->addSubview(m_pWanderOraintaionLayer);
    
    CCSize hViewSize = this->getView()->getFrame().size;
    CCRect cancelR = CCRect(_px(10), hViewSize.height - _px(90), hViewSize.width-_px(20), _px(80));
    CAButton *cancelBtn = CAButton::createWithFrame(cancelR, CAButtonTypeRoundedRect);
    cancelBtn->setTitleForState(CAControlStateAll, "取消");
//    cancelBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onCancelBtnClick), CAControlEventTouchUpInSide);
    m_pWanderOraintaionLayer->addSubview(cancelBtn);
    
    CCRect albumR = CCRect(_px(10), cancelR.origin.y - _px(90), hViewSize.width-_px(20), _px(80));
    CAButton *albumBtn = CAButton::createWithFrame(albumR, CAButtonTypeRoundedRect);
    albumBtn->setTitleForState(CAControlStateAll, "从相册选择");
//    albumBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onAlbumBtnClick), CAControlEventTouchUpInSide);
    m_pWanderOraintaionLayer->addSubview(albumBtn);
    
    CCRect cameraR = CCRect(_px(10), albumR.origin.y - _px(90), hViewSize.width-_px(20), _px(80));
    CAButton *cameraBtn = CAButton::createWithFrame(cameraR, CAButtonTypeRoundedRect);
    cameraBtn->setTitleForState(CAControlStateAll, "拍照");
//    cameraBtn->addTarget(this, CAControl_selector(DMLoginRegistController::onCameraBtnClick), CAControlEventTouchUpInSide);
    m_pWanderOraintaionLayer->addSubview(cameraBtn);
    
    
    
}


void DMSettingController::onHorizontoalSet()
{
    
}

void DMSettingController::onVeticialSet()
{
    
}

void DMSettingController::onNonSetClick()
{
    
}


void DMSettingController::userLogout()
{
    DMUserManager::sharedDMUserManager()->userLogout();
    onUserOffLine();
}


void DMSettingController::viewDidUnload()
{
}

CATableViewCell * DMSettingController::generateCell(const CCSize& cellSize){
    CATableViewCell *cell = CATableViewCell::create("");
//    cell->setFrame(CCRect(0, 0, cellSize.width, cellSize.height));
    cell->setColor(CAColor_white);
    
    CALabel *mLLable = CALabel::createWithFrame(CCRect(_px(30), 0, cellSize.width*0.6, cellSize.height));
    mLLable->setColor(CONTENT_COLOR);
    mLLable->setFontSize(_px(32));
    mLLable->setTag(100);
    mLLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    cell ->addSubview(mLLable);
    
    CAView *cellBac = CAView::createWithFrame(CCRect(0, 0, cellSize.width, cellSize.height));
    cellBac->setColor(CAColor_white);
    cell->setBackgroundView(cellBac);
    
    CCRect rect = CCRect(cellSize.width-_px(120), cellSize.height/2, _px(90), _px(30));
    CASwitch *vSwitch = CASwitch::createWithCenter(rect);
    
    vSwitch->setTag(101);
    cell->addSubview(vSwitch);
    
    
    
    return cell;
}

CATableViewCell* DMSettingController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    CSJson::Value doc ;
    
    
    
    switch (section) {
        //==================网络设置==================
        case 0:
            switch (row) {
                //----------------移动网络观看------------------
                case 0:
                {
                    CATableViewCell *cell = this->generateCell(cellSize);
                    CALabel *mLLable = (CALabel *)cell->getSubviewByTag(100);
                    mLLable->setText("移动网络观看");
                    
                    CASwitch *mLSwitch = (CASwitch *)cell->getSubviewByTag(101);
                    
                    if (m_pSettingmanger->getSetOnLineWatch() == On) {
                        mLSwitch->setIsOn(true, false);
                    }else
                    {
                        mLSwitch->setIsOn(false, false);
                    }
                    
                    mLSwitch->addTarget(this, CAControl_selector(DMSettingController::onOnlineWatchSwitch));
                    
                    //header的底部线
                    CAView *botLine = CAView::createWithFrame(CCRect(0, 0, cell->getFrame().size.width, _px(1)));
                    botLine->setColor(CELL_ITEM_COLOR);
                    cell->addSubview(botLine);
                    

                    return cell;

                }
                break;
                //----------------移动网络下载------------------
                case 1:
                {
                    CATableViewCell *cell = this->generateCell(cellSize);
                    CALabel *mLLable = (CALabel *)cell->getSubviewByTag(100);
                    mLLable->setText("移动网络下载");
                    CASwitch *mLSwitch = (CASwitch *)cell->getSubviewByTag(101);
                    
                    if (m_pSettingmanger->getSetDownLoadWatch() == On) {
                        mLSwitch->setIsOn(true, false);
                    }else
                    {
                        mLSwitch->setIsOn(false, false);
                    }
                    
                    mLSwitch->addTarget(this, CAControl_selector(DMSettingController::onDownloadWatch));
                    
                    return cell;
                }
                break;
            }
            break;
        
        case 1:
        //==================新消息通知==================
            
        {
            CATableViewCell *cell = CATableViewCell::create("");
            CALabel *mWLabel = CALabel::createWithFrame(CCRect(_px(30), _px(18), cellSize.width-_px(60), cellSize.height-_px(18)));
            std:string str = "如果你要关闭或者开启违心的新消息通知，请在iPhone的'设置'-'通知'功能中，找到应用程序'漫画之家'进行修改";
            mWLabel->setText(str);
            mWLabel->setFontSize(_px(28));
            mWLabel->setColor(CONTENT_COLOR);
            cell->addSubview(mWLabel);
            cell->setColor(TITLE_BAC_COLOR);
            return cell;
        }
          
            break;
        
        case 2:
            //==================阅读设置==================
        {
            
            /*
            CATableViewCell *cell = CATableViewCell::create("");
            cell->setColor(TITLE_BAC_COLOR);
            
            CALabel *mSLable = CALabel::createWithFrame(CCRect(_px(30), 0, cellSize.width*0.6, cellSize.height));
            mSLable->setColor(CONTENT_COLOR);
            mSLable->setFontSize(_px(32));
            mSLable->setText("显示模式设置");
            mSLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            
            CAView *bacView = CAView::createWithFrame(CCRect(0, 0, cellSize.width, cellSize.height));
            bacView->setColor(CAColor_white);
            cell->setBackgroundView(bacView);
            
            CAView *botLine = CAView::createWithFrame(CCRect(0, 0, cellSize.width, _px(1)));
            botLine->setColor(CELL_ITEM_COLOR);
            cell->addSubview(botLine);
            
            cell ->addSubview(mSLable);
             */
            
            CATableViewCell *cell = this->generateCell(cellSize);
            CALabel *mLLable = (CALabel *)cell->getSubviewByTag(100);
            mLLable->setText("开启横屏模式");
            CASwitch *mLSwitch = (CASwitch *)cell->getSubviewByTag(101);
            
            if (m_pSettingmanger->getSetDisplayModel() == On) {
                mLSwitch->setIsOn(true, false);
            }else
            {
                mLSwitch->setIsOn(false, false);
            }
            
            mLSwitch->addTarget(this, CAControl_selector(DMSettingController::onWatchModel));
            
            
            return cell;
        }
            
            break;
            
        //==================退出==================
        case 3:
            
            CAScale9ImageView * btn = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/setting/more_set_exit.png"));
            btn->setFrame(CCRect(_px(30), 0, cellSize.width-_px(60), cellSize.height));
            
            CATableViewCell *cell = CATableViewCell::create("");
            cell->setFrame(CCRect(0, 0, cellSize.width, cellSize.height));
            cell->setColor(TITLE_BAC_COLOR);
            
            cell->addSubview(btn);
            
            return cell;
            break;

    }
    
    return NULL;
}


//移动网络观看
void DMSettingController::onOnlineWatchSwitch(CAControl* c, CCPoint p)
{
    
    CASwitch *swith = (CASwitch *)c;
    if(swith->isOn()){
        m_pSettingmanger->setSetOnLineWatch(On);
    }else
    {
        m_pSettingmanger->setSetOnLineWatch(Off);
    }
}
//移动网络下载
void DMSettingController::onDownloadWatch(CAControl* c, CCPoint p)
{
    
    CASwitch *swith = (CASwitch *)c;
    if(swith->isOn()){
        m_pSettingmanger->setSetDownLoadWatch(On);
    }else
    {
        m_pSettingmanger->setSetDownLoadWatch(Off);
    }
}
//观看模式
void DMSettingController::onWatchModel(CAControl* c, CCPoint p)
{
    CASwitch *swith = (CASwitch *)c;
    if(swith->isOn()){
        m_pSettingmanger->setSetDisplayModel(On);
    }else
    {
        m_pSettingmanger->setSetDisplayModel(Off);
    }
}

unsigned int DMSettingController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    switch (section) {
        case 0:
            return 2;
            break;
        case 1:
            return 0;
            break;
        case 2:
            return 1;
            break;
        case 3:
            return 0;
            break;
        default:
            break;
    }
    return 0;
}

unsigned int DMSettingController::numberOfSections(CATableView *table)
{
    return 4;
}


void DMSettingController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
}

unsigned int DMSettingController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
    switch (section) {
        case 0:
            return _px(90);
            break;
        case 1:
            return _px(140);
            break;
        case 2:
            return _px(90);
            break;
        case 3:
            return _px(82);
            break;
        default:
            break;
    }
    
    return 0;
}

unsigned int DMSettingController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    switch (section) {
        case 0:
            return _px(78);
            break;
        case 1:
            return _px(86+110);
            break;
        case 2:
            return _px(88);
            break;
        case 3:
            return _px(192);
            break;
        default:
            break;
    }
    
    return 0;
}

CAView* DMSettingController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    
        switch (section) {
            case 0:
            {
                CAView * mView = CAView::createWithFrame(CCRect(0,0,viewSize.width,_px(86)));
                mView->setColor(TITLE_BAC_COLOR);
                CCRect rect = CCRect(_px(30), _px(30), viewSize.width, _px(86));
                CALabel *lable = CALabel::createWithFrame(rect);
                lable->setText("网络设置");
                lable->setColor(TITLE_COLOR);
                lable->setFontSize(_px(TITLE_SIZE));
                
                
                
                mView->addSubview(lable);
                return mView;
            }
                break;
            case 1:
            {
                
                CAView *hWareView = CAView::createWithFrame(CCRect(0,0,viewSize.width,_px(140)+_px(86)));
                
                CAView * mView = CAView::createWithFrame(CCRect(0,0,viewSize.width,viewSize.height));
                mView->setColor(TITLE_BAC_COLOR);
                CCRect rect1 = CCRect(_px(30), _px(40), viewSize.width, viewSize.height);
                CALabel *lable1 = CALabel::createWithFrame(rect1);
                lable1->setText("新消息通知");
                lable1->setColor(TITLE_COLOR);
                lable1->setFontSize(_px(TITLE_SIZE));
                mView->addSubview(lable1);
                
                
                CALabel *mWLabel = CALabel::createWithFrame(CCRect(_px(30), _px(87), viewSize.width-_px(60), _px(140)-_px(18)));
            std:string str = "如果你要关闭或者开启违心的新消息通知，请在iPhone的'设置'-'通知'功能中，找到应用程序'漫画之家'进行修改";
                mWLabel->setText(str);
                mWLabel->setFontSize(_px(28));
                mWLabel->setColor(CONTENT_COLOR);
                
                hWareView->addSubview(mView);
                hWareView->addSubview(mWLabel);
                
                return hWareView;
            }
                break;
            case 2:
            {
                CAView * mView = CAView::createWithFrame(CCRect(0,0,viewSize.width,viewSize.height));
                mView->setColor(TITLE_BAC_COLOR);
                CCRect rect2 = CCRect(_px(30), _px(40), viewSize.width, viewSize.height);
                CALabel *lable2 = CALabel::createWithFrame(rect2);
                lable2->setText("阅读设置");
                lable2->setColor(TITLE_COLOR);
                lable2->setFontSize(_px(TITLE_SIZE));
                mView->addSubview(lable2);
                return mView;
            }
                break;
            case 3:
            {
                
                CAView *exitBac = CAView::createWithFrame(CCRect(0,0,viewSize.width,viewSize.height));
                exitBac->setColor(TITLE_BAC_COLOR);
                
                CAScale9ImageView * btn = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/setting/more_set_exit.png"));
                CAScale9ImageView * btn2 = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/setting/more_set_exit.png"));
                btn2->setColor(CAColor_gray);
                
                m_pLoginOutBtn = CAButton::createWithFrame(CCRect(_px(30), _px(80), viewSize.width-_px(60), _px(80)), CAButtonTypeCustom);

                m_pLoginOutBtn->setBackGroundViewForState(CAControlStateNormal, btn);
                m_pLoginOutBtn->setBackGroundViewForState(CAControlStateHighlighted, btn2);
                
                m_pLoginOutBtn->setTitleForState(CAControlStateAll, "退出");
                
                m_pLoginOutBtn->setColor(CONTENT_COLOR);
                
                exitBac->addSubview(m_pLoginOutBtn);
                m_pLoginOutBtn->addTarget(this, CAControl_selector(DMSettingController::userLogout), CAControlEventTouchUpInSide);
                return exitBac;
               
            }
                break;
            default:
                break;
        }
    return NULL;
}












