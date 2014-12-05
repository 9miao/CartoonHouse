
#include "DMClassificationViewController.h"
#include "DMChartsController.h"
#include "DMClassifCatalogViewController.h"
#include "DMUserManager.h"

DMClassificationViewController::DMClassificationViewController()
:m_pCollectionView(NULL)
,m_pMyInfoLabel(NULL)
{
    
}

DMClassificationViewController::~DMClassificationViewController()
{
    CC_SAFE_RELEASE(m_pHeadImgStatus);
}

void DMClassificationViewController::onActionPerformed(LoginAction action)
{
    if (action == LoginActionLoginSuccess) {
        checkUserStatus();
    }
}

DMClassificationViewController* DMClassificationViewController::create()
{
    DMClassificationViewController* viewController = new DMClassificationViewController();
    if (viewController && viewController->init())
    {
        viewController->autorelease();
        return viewController;
    }
    CC_SAFE_DELETE(viewController);
    return NULL;
}

bool DMClassificationViewController::init()
{
    if (!CAViewController::init())
    {
        return false;
    }
    
    m_sMyInfoString = "未登录";
    
    return true;
}

void DMClassificationViewController::viewDidAppear()
{
    checkUserStatus();
    UMENG_PageStart("分类");
}

void DMClassificationViewController::viewDidDisappear()
{
    UMENG_PageEnd("分类");
}

void DMClassificationViewController::checkUserStatus()
{
    DMUserManager* uMgr = DMUserManager::sharedDMUserManager();
    DMUser user = uMgr->onLineUser();
    if (user.uid.length()==0) {
        m_pMyInfoLabel->setText(m_sMyInfoString);
        CAImageView *m_pHeadDefImg = CAImageView::createWithImage(CAImage::create(dmDefaultHeadImage));
        m_pHeadButton->setBackGroundViewForState(CAControlStateNormal, m_pHeadDefImg);
        CAImageView* imageView2 = CAImageView::createWithImage(CAImage::create(dmDefaultHeadImage));
        imageView2->setColor(ccc4(166, 166, 166, 255));
        m_pHeadButton->setBackGroundViewForState(CAControlStateHighlighted, imageView2);
        m_pHeadButton->addTarget(this, CAControl_selector(DMClassificationViewController::onHeadeClicke), CAControlEventTouchUpInSide);
        addHeadForegrand();
        return;
    }
    if (user.status==online) {
        m_pMyInfoLabel->setText(user.nickname);
        m_pHeadButton->removeTarget(this, CAControl_selector(DMClassificationViewController::onHeadeClicke), CAControlEventTouchUpInSide);
    }else{
        m_pMyInfoLabel->setText(m_sMyInfoString);
        m_pHeadButton->addTarget(this, CAControl_selector(DMClassificationViewController::onHeadeClicke), CAControlEventTouchUpInSide);
    }
    if (user.photo.length()>0) {
        m_pHeadImgStatus->setUrl(user.photo);
        m_pHeadButton->setBackGroundViewForState(CAControlStateAll, m_pHeadImgStatus);
    }
    addHeadForegrand();

}

void DMClassificationViewController::addHeadForegrand()
{
    if (m_pHeadButton->getSubviewByTag(11)) {
        m_pHeadButton->removeSubviewByTag(11);
    }
    CAImageView *headTopView = CAImageView::create();
    headTopView->setFrame(CADipRect(0, 0, 130, 130));
    headTopView->setImage(CAImage::create(dmHeadTopImage));
    headTopView->setTag(11);
    m_pHeadButton->addSubview(headTopView);
}

void DMClassificationViewController::viewDidLoad()
{
    CADipRect bounds = this->getView()->getBounds();
    
    this->getView()->setColor(CAColor_clear);
    
    m_iItems = bounds.size.width / 250;
    m_iItems = MAX(m_iItems, 3);
    m_iItems = MIN(m_iItems, 6);
    
    CADipRect headerRect = bounds;
    headerRect.size.height = 270;
    
    m_pHeadImgStatus = CommonUrlImageView::createWithImage(CAImage::create(dmDefaultHeadImage));
    m_pHeadImgStatus->retain();
    
    
    
    CAImageView* imageView2 = CAImageView::createWithImage(CAImage::create(dmDefaultHeadImage));
    imageView2->setColor(ccc4(166, 166, 166, 255));
    
    m_pHeadButton = CAButton::create(CAButtonTypeCustom);
    m_pHeadButton->setBackGroundViewForState(CAControlStateNormal, m_pHeadImgStatus);
    m_pHeadButton->setBackGroundViewForState(CAControlStateHighlighted, imageView2);
    m_pHeadButton->setCenter(CADipRect(bounds.size.width/2, 125, 130, 130));
    this->getView()->addSubview(m_pHeadButton);
    m_pHeadButton->addTarget(this, CAControl_selector(DMClassificationViewController::onHeadeClicke), CAControlEventTouchUpInSide);
    
    addHeadForegrand();
    
//    CAImageView *headTopView = CAImageView::create();
//    headTopView->setFrame(CADipRect(0, 0, 130, 130));
//    headTopView->setImage(CAImage::create(dmHeadTopImage));
//    m_pHeadButton->addSubview(headTopView);
    
    m_pMyInfoLabel = CALabel::createWithFrame(CADipRect(0, 200, bounds.size.width, 50));
    m_pMyInfoLabel->setTextAlignment(CATextAlignmentCenter);
    m_pMyInfoLabel->setFontSize(_px(30));
    m_pMyInfoLabel->setColor(ccc4(92, 100, 112, 255));
    m_pMyInfoLabel->setText(m_sMyInfoString);
    this->getView()->addSubview(m_pMyInfoLabel);
    
    
    CADipRect collectionRect = bounds;
    collectionRect.size.height -= 270;
    collectionRect.origin.y = 270;
    m_pCollectionView = CACollectionView::createWithFrame(collectionRect);
    m_pCollectionView->setCollectionViewDataSource(this);
    m_pCollectionView->setCollectionViewDelegate(this);
    m_pCollectionView->setAllowsSelection(true);
    m_pCollectionView->setShowsVerticalScrollIndicator(false);
    m_pCollectionView->setVertInterval(_px(20));
    m_pCollectionView->setHoriInterval(_px(30));
    m_pCollectionView->setBackGroundColor(ccc4(255, 255, 255, 255));
    this->getView()->addSubview(m_pCollectionView);
    
    this->searchClassfy();

}

void DMClassificationViewController::viewDidUnload()
{
    
}

void DMClassificationViewController::onButton(CrossApp::CAControl* control, CrossApp::CCPoint point)
{
    
}

void DMClassificationViewController::onHeadeClicke(CAControl* control, CCPoint point){
    
    DMLoginController* loginController = new DMLoginController();
    loginController->init();
    loginController->addUserLoginDelegate(this);
    CANavigationController *loginNController = new CANavigationController();
    loginNController ->autorelease();
    loginNController->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    loginNController->initWithRootViewController(loginController);
    
    this->presentModalViewController(loginNController, true);
    loginController->autorelease();
    
}

void DMClassificationViewController::searchClassfy()
{
    std::string url = get_url(HttpUrlTypeClassifyify);
    std::map<string, string> key_value;
    CommonHttpManager::getInstance()->send_get(url, key_value, this, CommonHttpJson_selector(DMClassificationViewController::onSearchClassifyFinished));
}

void DMClassificationViewController::onSearchClassifyFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        for (int i = 0 ; i< json.size(); i++) {
            
            DMModelClassify fModel;
            const CSJson::Value& fValue = json[i];
            if (fValue["id"].isInt()) {
                int mId = fValue["id"].asInt();
                char va[64];
                sprintf(va, "%d",mId);
                string fId = va;
                fModel._id = fId;
            } else {
                fModel._id = fValue["id"].asString();
            }
            
            fModel.icon = fValue["icon"].asString();
            fModel.title = fValue["title"].asString();
            const CSJson::Value& sValues = fValue["subnodes"];
            for (int j = 0; j<sValues.size(); j++) {
                const CSJson::Value& sValue = sValues[j];
                DMModelClassify sModel;
                sModel._id = atoi(sValue["id"].asString().c_str());
                
                if (sValue["id"].isInt()) {
                    int mId = sValue["id"].asInt();
                    char va[64];
                    sprintf(va, "%d",mId);
                    string fId = va;
                    sModel._id = fId;
                } else {
                    sModel._id = sValue["id"].asString();
                }
                sModel.icon = sValue["icon"].asString();
                sModel.title = sValue["title"].asString();
                fModel.subnodes.push_back(sModel);
            }
            m_vClassifyModeles.push_back(fModel);
        }
        m_pCollectionView->reloadData();
    }
}


//Necessary
CACollectionViewCell* DMClassificationViewController::collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
    CACollectionViewCell* cell = NULL;
    const DMModelClassify& model = m_vClassifyModeles.at(section);
    if (row * m_iItems + item < model.subnodes.size())
    {
        cell = collectionView->dequeueReusableCellWithIdentifier("cell");
        if (cell == NULL)
        {
            cell = CACollectionViewCell::create("cell");
            cell->setAllowsSelected(false);
            
            CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(CAImage::create(classifyItemBg));
            cell->setBackgroundView(imageView);
            
            CCRect rect;
            rect.origin = cellSize/2;
            rect.size = cellSize;
            
            CALabel* label = CALabel::createWithCenter(rect);
            label->setFontSize(_px(24));
            label->setTextAlignment(CATextAlignmentCenter);
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setTag(100);
            label->setColor(ccc4(51, 51, 51, 255));
            cell->addSubview(label);
        }
        CALabel* label = (CALabel*)cell->getSubviewByTag(100);
        label->setText(model.subnodes.at(row * 3 + item).title);
    }

    return cell;
}

//Necessary
unsigned int DMClassificationViewController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
    return _px(60);
}

//Necessary
unsigned int DMClassificationViewController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
    return m_iItems;
}

//Necessary
unsigned int DMClassificationViewController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
    unsigned int size = m_vClassifyModeles.at(section).subnodes.size();
    if (size % m_iItems == 0)
    {
        return size / m_iItems;
    }
    else
    {
        return size / m_iItems + 1;
    }
}

unsigned int DMClassificationViewController::numberOfSections(CACollectionView *collectionView)
{
    return m_vClassifyModeles.size();
}

unsigned int DMClassificationViewController::collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section)
{
    return _px(50);
}

CAView* DMClassificationViewController::collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section)
{
    CAButton* button = CAButton::create(CAButtonTypeCustom);
    button->setBackGroundViewForState(CAControlStateNormal, CAView::create());
    button->setTag(section);
    button->addTarget(this, CAControl_selector(DMClassificationViewController::onButton), CAControlEventTouchUpInSide);
    
    const DMModelClassify& model = m_vClassifyModeles.at(section);

    /*
    CAImageView* iconView = CAImageView::createWithImage(CAImage::create(classifyPath + model.icon));
    iconView->setFrame(CADipRect(100, 7, 46, 46));
    button->addSubview(iconView);
    */
    
    CAView* line = CAView::createWithFrame(CCRect(0, viewSize.height - 1, viewSize.width, 1), ccc4(220, 220, 220, 255));
    button->addSubview(line);
    
    CALabel* label = CALabel::createWithFrame(CADipRect(30, 15, 150, 46));
    label->setFontSize(_px(24));
    label->setColor(ccc4(92, 100, 112, 255));
    label->setText(model.title);
    button->addSubview(label);
    
    return button;
}

void DMClassificationViewController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    switch (section)
    {
            //排行榜
        case 0:
        {
            RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
            DMChartsController* chartsController = new DMChartsController();
            chartsController->init();
            chartsController->autorelease();
            CANavigationController* nav = RootWindow::getInstance()->getRootNavigationController();
            if (dynamic_cast<DMClassifCatalogViewController*>(nav->getBackViewController()) ||
                dynamic_cast<DMChartsController*>(nav->getBackViewController()) )
            {
                nav->replaceViewController(chartsController, false);
            }
            else
            {
                nav->pushViewController(chartsController, false);
            }
        }
            break;
        default:
        {
            RootWindow::getInstance()->getDrawerController()->hideLeftViewController(true);
            const DMModelClassify& model = m_vClassifyModeles.at(section).subnodes.at(row * 3 + item);
            DMClassifCatalogViewController* catalog = new DMClassifCatalogViewController(model._id);
            catalog->setTitle(model.title);
            catalog->init();
            catalog->autorelease();
            CANavigationController* nav = RootWindow::getInstance()->getRootNavigationController();
            if (dynamic_cast<DMClassifCatalogViewController*>(nav->getBackViewController()) ||
                dynamic_cast<DMChartsController*>(nav->getBackViewController()) )
            {
                nav->replaceViewController(catalog, false);
            }
            else
            {
                nav->pushViewController(catalog, false);
            }
        }
            break;
    }
}



