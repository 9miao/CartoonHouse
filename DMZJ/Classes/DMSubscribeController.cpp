

#include "DMSubscribeController.h"
#include "DMSearchController.h"
#include "DMLoginController.h"
#include "DMIntroductionController.h"
#include "DMUserManager.h"


DMSubscribeController::DMSubscribeController()
:m_pPageView(NULL)
,m_pTableView1(NULL)
,m_pTableView2(NULL)
,m_pSegmentedControl(NULL)
,m_pIntroInfoManager(NULL)
{
    m_pIntroInfoManager = DMIntroductionInfoManager::sharedDMIntroductionInfoManager();;
}

DMSubscribeController::~DMSubscribeController()
{
    CC_SAFE_DELETE(m_pIntroInfoManager);
}

bool DMSubscribeController::init()
{
    if (!CAViewController::init())
    {
        return false;
    }
    CASegmentedControl* segmentedControl = CASegmentedControl::createWithFrame(CCRectZero, 2);
    segmentedControl->setTitleAtIndex("订阅", 0, CAControlStateAll);
    segmentedControl->setTitleAtIndex("浏览记录", 1, CAControlStateAll);
    for (int i=0; i<2; i++)
    {
        segmentedControl->setTitleColorAtIndex(ccc4(87, 154, 244, 255), i, CAControlStateSelected);
        segmentedControl->setTitleColorAtIndex(CAColor_white, i, CAControlStateHighlighted);
        segmentedControl->setTitleColorAtIndex(CAColor_white, i, CAControlStateNormal);
    }
    
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal1), 0, CAControlStateNormal);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal1), 0, CAControlStateHighlighted);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segSelected1), 0, CAControlStateSelected);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal2), 1, CAControlStateNormal);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal2), 1, CAControlStateHighlighted);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segSelected2), 1, CAControlStateSelected);
    segmentedControl->addTarget(this, CAControl_selector(DMSubscribeController::onSegmented));
    segmentedControl->setSelectedAtIndex(0);
    segmentedControl->setBounds(CADipRect(0, 0, 400, 100));
    m_pSegmentedControl = segmentedControl;
    
    
    CABarButtonItem* buttonItem = CABarButtonItem::create("", CAImage::create(loginIcon), NULL);
    buttonItem->setTarget(this, CAControl_selector(DMSubscribeController::onButton1));
    CABarButtonItem* buttonItem2 = CABarButtonItem::create("", CAImage::create(searchIcon), NULL);
    buttonItem2->setTarget(this, CAControl_selector(DMSubscribeController::onButton2));
    
    
    CANavigationBarItem* item = CANavigationBarItem::create("0");
    item->setTitleView(segmentedControl);
    item->addLeftButtonItem(buttonItem);
    item->addRightButtonItem(buttonItem2);
    this->setNavigationBarItem(item);
    
    
    return true;
}

void DMSubscribeController::viewDidLoad()
{
    
    CCRect bounds = this->getView()->getBounds();
    
    m_pPageView = CAPageView::createWithFrame(bounds, CAPageView::CAPageViewDirectionHorizontal);
    this->getView()->addSubview(m_pPageView);
    m_pPageView->setPageViewDelegate(this);
    
    CCRect rect = bounds;
    m_pTableView1 = new CATableView();
    m_pTableView1->initWithFrame(rect);
    m_pTableView1->setTableViewDataSource(this);
    m_pTableView1->setTableViewDelegate(this);
    m_pTableView1->setAllowsSelection(true);
    m_pTableView1->autorelease();
    
    rect.origin.x += rect.size.width;
    m_pTableView2 = new CATableView();
    m_pTableView2->initWithFrame(rect);
    m_pTableView2->setTableViewDataSource(this);
    m_pTableView2->setTableViewDelegate(this);
    m_pTableView2->setAllowsSelection(true);
    m_pTableView2->autorelease();
    
    
    CAVector<CAView*> views;
    views.pushBack(m_pTableView1);
    views.pushBack(m_pTableView2);
    m_pPageView->setViews(views);
    
    
    m_pStatusLable = CALabel::createWithFrame(CCRect((this->getView()->getFrame().size.width - _px(200))/2, _px(300), _px(200), _px(40)));
    m_pStatusLable->setColor(CAColor_black);
    m_pStatusLable->setFontSize(_px(36));
    m_pStatusLable->setTextAlignment(CATextAlignmentCenter);
    this->showStatusLable(0);
    this->getView()->addSubview(m_pStatusLable);
    
}

//0 空  ; 1 订阅 ; 2 历史记录
void DMSubscribeController::showStatusLable(int flag)
{
    switch (flag) {
            //空
        case 0:
            m_pStatusLable->setText("");
            break;
            //订阅
        case 1:
            m_pStatusLable->setText("暂无订阅");
            break;
            //历史
        case 2:
            m_pStatusLable->setText("暂无历史");
            break;
    }
}

void DMSubscribeController::viewDidUnload()
{

}

void DMSubscribeController::viewDidAppear()
{
    //浏览记录
    CCLog("viewDidAppear");
    introductionInfos = m_pIntroInfoManager->getRecent();
    if (m_pTableView2) {
        m_pTableView2->reloadData();
        
    }
    
    //订阅
    DMUser user = DMUserManager::sharedDMUserManager()->onLineUser();
    if (user.uid.length()>0 && user.status == online) {
        requestSubscribeData(user.uid);
    }else if (m_pTableView1){
        subscribes.clear();
        m_pTableView1->reloadData();
        if (m_pPageView->getCurrPage()==0) {
            showStatusLable(1);
        }
    }
    m_pPageView->getSubViewAtIndex(1 - m_pPageView->getCurrPage())->setVisible(true);
}

void DMSubscribeController::viewDidDisappear()
{
    CCLog("viewDidDisappear");
    m_pPageView->getSubViewAtIndex(1 - m_pPageView->getCurrPage())->setVisible(false);
}

void DMSubscribeController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    CCLog("OK");
}

void DMSubscribeController::onSegmented(CAControl* control, CCPoint point)
{
    
    
    CASegmentedControl* segmentedControl = (CASegmentedControl*)control;
    int index = segmentedControl->getSelectedIndex();
    
    if (index ==0 &&  subscribes.size()==0  ) {
        showStatusLable(1);
    }else if (index == 1 && introductionInfos.size()==0){
        showStatusLable(2);
    }else{
        showStatusLable(0);
    }
    
    CC_RETURN_IF(m_pPageView == NULL);
    CC_RETURN_IF(segmentedControl->getSelectedIndex() == m_pPageView->getCurrPage());
    m_pPageView->setCurrPage(index, true);
    
}

void DMSubscribeController::onButton1(CAControl* control, CCPoint point)
{
    DMLoginController* loginController = new DMLoginController();
    loginController->init();
    CANavigationController *loginNController = new CANavigationController();
    loginNController ->autorelease();
    loginNController->getNavigationBar()->setBackGroundImage(CAImage::create(navigationBackground));
    loginNController->initWithRootViewController(loginController);
    
    this->presentModalViewController(loginNController, true);
    loginController->autorelease();
}

void DMSubscribeController::onButton2(CAControl* control, CCPoint point)
{
    DMSearchController* searchController = DMSearchController::create();
    this->presentModalViewController(searchController, true);
}

void DMSubscribeController::pageViewDidEndTurning(CAPageView *pageView)
{
    CC_RETURN_IF(m_pSegmentedControl->getSelectedIndex() == pageView->getCurrPage());
    m_pSegmentedControl->setSelectedAtIndex(pageView->getCurrPage());
}

void DMSubscribeController::requestSubscribeData(std::string id1)
{
    
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeSubScribComic , id1);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMSubscribeController::onSubScribeFinish));
    
}

void DMSubscribeController::onSubScribeFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    subscribes.clear();
    if (json["result"].asInt()==1) {
        const CSJson::Value &data = json["data"];
        CCLog("data size == %d",data.size());
        for (int i = 0; i<data.size(); i++) {
            const CSJson::Value &sub = data[i];
            DMModelSubscribe subscibe;
            subscibe.sub_id = sub["sub_id"].asString();
            subscibe.sub_name = sub["sub_name"].asString();
            subscibe.sub_img = sub["sub_img"].asString();
            subscibe.sub_update = sub["sub_update"].asString();
            
            subscribes.push_back(subscibe);
        }
        
       
        
    }
    
    if (subscribes.size()==0 && m_pPageView->getCurrPage()==0) {
        showStatusLable(1);
    }
    
    m_pTableView1->reloadData();
}

CATableViewCell* DMSubscribeController::tableCellAtIndex(CATableView* table,
                                                         const CCSize& cellSize,
                                                         unsigned int section,
                                                         unsigned int row)
{
    DMSubscribeTableCell* cell = (DMSubscribeTableCell*)table->dequeueReusableCellWithIdentifier("table");
    if (cell == NULL)
    {
        cell = DMSubscribeTableCell::create("table", cellSize);
    }
    
    cell->setBookIconImage(CAImage::create("dm_resource/1.png"));
    
    CCString* str1 = CCString::createWithFormat("火影忍者%d", row);
    CCString* str2 = CCString::createWithFormat("作者：岸本齐史%d", row);
    CCString* str3 = CCString::createWithFormat("看到第十话%d", row);
    CCString* str4 = CCString::createWithFormat("更新到第230话%d", row);
    cell->setBookTitleText(str1->getCString());
    cell->setBookAuthorText(str2->getCString());
    cell->setReadProgressText(str3->getCString());
    cell->setBookUpdateWhereText(str4->getCString());
    
    
    if (table == m_pTableView1 && subscribes.size()>0) {
        
        DMModelSubscribe subscri = subscribes.at(row);
        cell->setBookIconUrl(subscri.sub_img);
        cell->setBookTitleText(subscri.sub_name);
        cell->setBookAuthorText("作者："+subscri.sub_update);
        DMIntroductionInfoManager mgr ;
        DMBrowseInfo model = mgr.selectById(subscri.sub_id);
        if (model.readIndex == -1)
        {
            cell->setReadProgressText("暂无浏览记录");
        }
        else
        {
            cell->setReadProgressText("看到了" + model.readChapTitle);
        }
        
        cell->setBookUpdateWhereText("更新到"+subscri.sub_update);
        cell->visibleAuthorBook(false);
    }
    else if (table == m_pTableView2)
    {
        
        DMBrowseInfo info = introductionInfos.at(row);
        cell->setBookIconUrl(info.cover);
        cell->setBookTitleText(info.title);
        cell->setBookAuthorText("作者："+info.authors);
        if (info.readIndex == -1)
        {
            cell->setReadProgressText("暂无浏览记录");
        }
        else
        {
            cell->setReadProgressText("看到了" + info.readChapTitle);
        }
        
        cell->setBookUpdateWhereText("更新到"+info.last_update_chapter_name);
        cell->setReadRecordText(info.time);
        cell->visibleAuthorBook(true);
    }
    
    return cell;
}

unsigned int DMSubscribeController::numberOfRowsInSection(CATableView *table,
                                                          unsigned int section)
{
    if (table == m_pTableView1) {
        return subscribes.size();
    }else {
        return introductionInfos.size();
    }
    
}

unsigned int DMSubscribeController::tableViewHeightForRowAtIndexPath(CATableView* table,
                                                                     unsigned int section,
                                                                     unsigned int row)
{
    return _px(224);
}

void DMSubscribeController::tableViewDidSelectRowAtIndexPath(CATableView* table,
                                                             unsigned int section,
                                                             unsigned int row)
{
//    this->getNavigationController()->pushViewController(DMSubscribeController::create(), true);
    if (table == m_pTableView1) {
        DMModelSubscribe model = subscribes.at(row);
        DMIntroductionController *controller = DMIntroductionController::create(model.sub_id, model.sub_name);
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
    }else {
        DMBrowseInfo info = introductionInfos.at(row);
        DMIntroductionController *controller = DMIntroductionController::create(info.ID, info.title);
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
    }
}

void DMSubscribeController::tableViewDidDeselectRowAtIndexPath(CATableView* table,
                                                               unsigned int section,
                                                               unsigned int row)
{

    
}


#pragma DMSubscribeTableCell

DMSubscribeTableCell::DMSubscribeTableCell()
:m_pBookIcon(NULL)
,m_pBookTitle(NULL)
,m_pBookAuthor(NULL)
,m_pReadProgress(NULL)
,m_pBookUpdateWhere(NULL)
,m_pReadRecord(NULL)
{
    
}

DMSubscribeTableCell::~DMSubscribeTableCell()
{

}

void DMSubscribeTableCell::visibleAuthorBook(bool visible)
{
    m_pBookAuthor->setVisible(visible);
    CADipRect r1 = CADipRect(180, 80, 200, 40);
    CADipRect r2 = CADipRect(180, 122, 200, 40);
    CADipRect r3 = CADipRect(180, 164, 200, 40);
    if (visible) {
        m_pBookAuthor->setFrame(r1);
        m_pReadProgress->setFrame(r2);
        m_pBookUpdateWhere->setFrame(r3);
    }else{
        m_pReadProgress->setFrame(r1);
        m_pBookUpdateWhere->setFrame(r2);
    }
}

DMSubscribeTableCell* DMSubscribeTableCell::create(const std::string& reuseIdentifier, const CADipSize& size)
{
    DMSubscribeTableCell* cell = new DMSubscribeTableCell();
    if (cell && cell->initWithReuseIdentifier(reuseIdentifier, size))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}


bool DMSubscribeTableCell::initWithReuseIdentifier(const std::string& reuseIdentifier, const CADipSize& size)
{
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }

    CAScale9ImageView* bookBg = CAScale9ImageView::createWithImage(CAImage::create(book_bg));
    bookBg->setFrame(CADipRect(28, 28, 124, 164));
    this->addSubview(bookBg);
    
    m_pBookIcon = CommonUrlImageView::createWithFrame(CADipRect(32, 32, 116, 156));
    this->addSubview(m_pBookIcon);
    
    m_pBookTitle = CALabel::createWithFrame(CADipRect(180, 30, 200, 40));
    m_pBookTitle->setFontSize(_px(30));
    m_pBookTitle->setColor(ccc4(100, 100, 100, 255));
    this->addSubview(m_pBookTitle);
    
    m_pBookAuthor = CALabel::createWithFrame(CADipRect(180, 80, 200, 40));
    m_pBookAuthor->setFontSize(_px(24));
    m_pBookAuthor->setColor(ccc4(153, 153, 153, 255));
    this->addSubview(m_pBookAuthor);
    
    m_pReadProgress = CALabel::createWithFrame(CADipRect(180, 122, 200, 40));
    m_pReadProgress->setFontSize(_px(24));
    m_pReadProgress->setColor(ccc4(153, 153, 153, 255));
    this->addSubview(m_pReadProgress);
    
    m_pBookUpdateWhere = CALabel::createWithFrame(CADipRect(180, 164, 200, 40));
    m_pBookUpdateWhere->setFontSize(_px(24));
    m_pBookUpdateWhere->setColor(ccc4(153, 153, 153, 255));
    this->addSubview(m_pBookUpdateWhere);
    
    
    CADipRect rect;
    rect.size = CADipSize(200, 40);
    rect.origin.y = 30;
    rect.origin.x = size.width - 30 - 200;
    m_pReadRecord = CALabel::createWithFrame(rect);
    m_pReadRecord->setFontSize(_px(24));
    m_pReadRecord->setColor(ccc4(153, 153, 153, 255));
    m_pReadRecord->setTextAlignment(CATextAlignmentRight);
    this->addSubview(m_pReadRecord);
    
    return true;
}

void DMSubscribeTableCell::normalTableViewCell()
{
    m_pBackgroundView->setColor(CAColor_white);
}

void DMSubscribeTableCell::highlightedTableViewCell()
{
    m_pBackgroundView->setColor(ccc4(200, 200, 200, 255));
}

void DMSubscribeTableCell::selectedTableViewCell()
{
    m_pBackgroundView->setColor(ccc4(200, 200, 200, 255));
}

void DMSubscribeTableCell::disabledTableViewCell()
{
    
}

void DMSubscribeTableCell::setBookIconImage(CrossApp::CAImage *image)
{
    m_pBookIcon->setImage(image);
}

void DMSubscribeTableCell::setBookIconUrl(const std::string& url)
{
    m_pBookIcon->setUrl(url);
}

void DMSubscribeTableCell::setBookTitleText(const std::string& text)
{
    m_pBookTitle->setText(text);
}

void DMSubscribeTableCell::setBookAuthorText(const std::string& text)
{
    m_pBookAuthor->setText(text);
}

void DMSubscribeTableCell::setReadProgressText(const std::string& text)
{
    m_pReadProgress->setText(text);
}

void DMSubscribeTableCell::setBookUpdateWhereText(const std::string& text)
{
    m_pBookUpdateWhere->setText(text);
}

void DMSubscribeTableCell::setReadRecordText(const std::string& text)
{
    m_pReadRecord->setText(text);
}










