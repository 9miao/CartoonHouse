

#include "DMChartsController.h"
#include "DMIntroductionController.h"

DMChartsController::DMChartsController()
: m_pLoading(NULL)
, m_pTableView(NULL)
, m_pMonthView(NULL)
, m_pmWeekView(NULL)
{

}

DMChartsController::~DMChartsController()
{
    all_Set.clear();
    month_Set.clear();
    week_Set.clear();
    current_Set.clear();
    m_pCurrTableViews.clear();
}

bool DMChartsController::init()
{
    if (!CAViewController::init())
    {
        return false;
    }
    
    return true;
}

void DMChartsController::viewDidLoad()
{
    
    CCRect rect = this->getView()->getBounds();
    rect.size.height = _px(96);
    
    CAView* navigaView = CAView::createWithFrame(rect, ccc4(87, 154, 244, 255));
    this->getView()->addSubview(navigaView);
    
    CCRect btnRect;
    btnRect.size.width = rect.size.height * 0.9f;
    btnRect.size.height = rect.size.height * 0.8f;
    btnRect.origin.x = btnRect.size.width * 0.7f;
    btnRect.origin.y = rect.size.height * 0.5f;
    CAButton* button = CAButton::createWithCenter(btnRect, CAButtonTypeCustom);
    button->setImageForState(CAControlStateNormal, CAImage::create("source_material/btn_left_white.png"));
    button->setImageColorForState(CAControlStateHighlighted, CAColor_gray);
    navigaView->addSubview(button);
    button->addTarget(this, CAControl_selector(DMChartsController::onReturn), CAControlEventTouchUpInSide);
    
    CASegmentedControl *segmentedControl = CASegmentedControl::createWithFrame(CCRect(0, 0, 10, 10), 3);
    
    segmentedControl->setTitleAtIndex("总", 0, CAControlStateAll);
    segmentedControl->setTitleAtIndex("月", 1, CAControlStateAll);
    segmentedControl->setTitleAtIndex("周", 2, CAControlStateAll);
    
    for (int i=0; i<3; i++)
    {
        segmentedControl->setTitleColorAtIndex(ccc4(87, 154, 244, 255), i, CAControlStateSelected);
        segmentedControl->setTitleColorAtIndex(CAColor_white, i, CAControlStateHighlighted);
        segmentedControl->setTitleColorAtIndex(CAColor_white, i, CAControlStateNormal);
    }
    
    
    
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal1), 0, CAControlStateNormal);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal1), 0, CAControlStateHighlighted);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segSelected1), 0, CAControlStateSelected);
    
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal3), 1, CAControlStateNormal);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal3), 1, CAControlStateHighlighted);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segSelected3), 1, CAControlStateSelected);
    
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal2), 2, CAControlStateNormal);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segNormal2), 2, CAControlStateHighlighted);
    segmentedControl->setBackgroundImageAtIndex(CAImage::create(segSelected2), 2, CAControlStateSelected);
    
    segmentedControl->addTarget(this, CAControl_selector(DMChartsController::onSegmented));
    segmentedControl->setSelectedAtIndex(0);
    
    float segmentwid = 300;
    CCRect segR = CCRect((this->getView()->getFrame().size.width-_px(segmentwid))/2, _px(15), _px(segmentwid), _px(62));
    segmentedControl->setFrame(segR);
    navigaView->addSubview(segmentedControl);
    
    this->searchTopItems();
    
    {
        CCRect loadingRect = this->getView()->getBounds();
        loadingRect.origin.y = _px(96);
        loadingRect.size.height -= loadingRect.origin.y;
        m_pLoading = CAActivityIndicatorView::createWithFrame(loadingRect);
        CAImageView* indicator = CAImageView::createWithFrame(CADipRect(0, 0, 50, 50));
        indicator->setImage(CAImage::create(loadingIcon));
        m_pLoading->setActivityIndicatorView(indicator);
        CAView* bg = CAView::createWithFrame(CADipRect(0, 0, 275, 300), CAColor_clear);
        CAImageView* bg2 = CAImageView::createWithFrame(CADipRect(0, 0, 275, 100));
        bg2->setImage(CAImage::create(loadingBackground));
        bg->addSubview(bg2);
        m_pLoading->setActivityBackView(bg);
        m_pLoading->setLoadingMinTime(0.3f);
        m_pLoading->setColor(CAColor_white);
        this->getView()->insertSubview(m_pLoading, CAWindowZoderTop);
        m_pLoading->setTargetOnCancel(this, callfunc_selector(DMChartsController::initWithTableView));
    }
}

void DMChartsController::initWithTableView() 
{
    CCRect tableR = CCRect(0, _px(96), this->getView()->getFrame().size.width, this->getView()->getFrame().size.height-_px(96));
    //总榜
    m_pTableView = CATableView::createWithFrame(tableR);
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setTableViewDelegate(this);
    m_pTableView->setAllowsSelection(true);
    this->getView()->addSubview(m_pTableView);
    //月榜
    m_pMonthView = CATableView::createWithFrame(tableR);
    m_pMonthView->setTableViewDataSource(this);
    m_pMonthView->setTableViewDelegate(this);
    m_pMonthView->setAllowsSelection(true);
    m_pMonthView->setVisible(false);
    this->getView()->addSubview(m_pMonthView);
    //周榜
    m_pmWeekView = CATableView::createWithFrame(tableR);
    m_pmWeekView->setTableViewDataSource(this);
    m_pmWeekView->setTableViewDelegate(this);
    m_pmWeekView->setAllowsSelection(true);
    m_pmWeekView->setVisible(false);
    this->getView()->addSubview(m_pmWeekView);
    
    m_pCurrTableViews.push_back(m_pTableView);
    m_pCurrTableViews.push_back(m_pMonthView);
    m_pCurrTableViews.push_back(m_pmWeekView);
}

void DMChartsController::onReturn(CAControl* control, CCPoint point)
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}

void DMChartsController::onBackClick()
{
    this->dismissModalViewController(true);
}

void DMChartsController::onSegmented(CAControl* control, CCPoint point)
{
    CASegmentedControl* segmentedControl = (CASegmentedControl*)control;
    int index = segmentedControl->getSelectedIndex();
    
    switch (index) {
        case 0:
            current_Set = all_Set;
            break;
        case 1:
            current_Set = month_Set;
            break;
        case 2:
            current_Set = week_Set;
            break;
    }
    
    for (int i = 0 ; i<m_pCurrTableViews.size(); i++) {
        if (i==index) {
            m_pCurrTableViews.at(i)->setVisible(true);
            m_pCurrTableViews.at(i)->reloadData();
        }else{
            m_pCurrTableViews.at(i)->setVisible(false);
        }
        
    }
    
//    m_pTableView->reloadData();
    
}


void DMChartsController::searchTopItems()
{
    std::string url = get_url(HttpUrlTypeTop);
    std::map<string, string> key_value;
    CommonHttpManager::getInstance()->send_get(url, key_value, this, CommonHttpJson_selector(DMChartsController::onSearchFinished));
}

void DMChartsController::onSearchFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{

    const CSJson::Value &total=json["total"];
    const CSJson::Value &month=json["month"];
    const CSJson::Value &week=json["week"];
    
    for (int i = 0 ; i<total.size(); i++) {
        const CSJson::Value& value = total[i];
        all_Set.push_back(convertTopModel(value));
    }
    for (int i = 0 ; i<month.size(); i++) {
        const CSJson::Value& value = month[i];
        month_Set.push_back(convertTopModel(value));
    }
    for (int i = 0 ; i<week.size(); i++) {
        const CSJson::Value& value = week[i];
        week_Set.push_back(convertTopModel(value));
    }
    current_Set = all_Set;
    m_pLoading->stopAnimating();
    
}

TopModel DMChartsController::convertTopModel(const CSJson::Value &value)
{
    TopModel model ;
    model._id = value["id"].asString();
    model.title = value["title"].asString();
    model.last_update_chapter_name = value["last_update_chapter_name"].asString();
    model.cover = value["cover"].asString();
    model.authors = value["authors"].asString();
    model.types = value["types"].asString();
    model.sum_chapters = value["sum_chapters"].asString();
    model.sum_source = value["sum_source"].asString();
    return model;
}

unsigned int DMChartsController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return current_Set.size();
}

unsigned int DMChartsController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(274);
}

CATableViewCell* DMChartsController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    DMChartsCell *cell = (DMChartsCell *)table->dequeueReusableCellWithIdentifier("cell");
    if(cell==NULL){
        cell = new DMChartsCell();
        cell->init("cell", CCSize(this->getView()->getFrame().size.width, _px(274)));
        cell->autorelease();
    }
    
    TopModel model = current_Set.at(row);
    char s [128];
    sprintf(s, "dm_resource/charts/c%dx.png" ,row+1);
    const char *name = s;
    cell->getPosition()->setImage(CAImage::create(name));
    cell->getName()->setText(model.title);
    cell->getAuthor()->setText("作者: "+model.authors);
    cell->getClass()->setText("题材: "+model.types);
    cell->getLastWorks()->setText("最新收录: "+model.last_update_chapter_name);
    cell->getMainImg()->setUrl(model.cover);
    
    return cell;
}

void DMChartsController::viewDidUnload()
{
}

void DMChartsController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    TopModel model = current_Set.at(row);
    DMIntroductionController *controller = DMIntroductionController::create(model._id, model.title);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
}



DMChartsCell::DMChartsCell()
{
}

DMChartsCell::~DMChartsCell()
{
}

bool DMChartsCell::init(const char* reuseIdentifier, const CCSize& cellSize)
{
    
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier)) {
        return false;
    }
    
    //名次
    m_pPosition = CAImageView::createWithFrame(CCRect(_px(30), _px(108), _px(60), _px(60)));
    m_pPosition->setImage(CAImage::create("dm_resource/charts/c1x.png"));
    this->addSubview(m_pPosition);
    //图片
    CCRect mImgR = CCRect(_px(110), _px(30), _px(160), _px(212));
    CCRect mImgRealR = CCRect(_px(112), _px(32), _px(156), _px(208));
    m_pMainImg = CommonUrlImageView::createWithFrame(mImgRealR);
    CAScale9ImageView *bacImg = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/catalog/book_bg.png"));
//    m_pMainImg->setImage(CAImage::create("dm_resource/1.png"));
    bacImg->setFrame(mImgR);
    this->addSubview(bacImg);
    this->addSubview(m_pMainImg);
    //漫画名
    CCRect nameR = CCRect(_px(300), _px(30), cellSize.width-_px(300)-_px(30), _px(40));
    m_pName = CALabel::createWithFrame(nameR);
    m_pName->setFontSize(_px(30));
    m_pName->setColor(ccc4(100, 100, 100, 255));
    m_pName->setText("名称:");
    this->addSubview(m_pName);
    
    float cfSize = _px(24);
    CAColor4B color = ccc4(153, 153, 153, 255);
    
    //作者
    m_pAuthor = CALabel::createWithFrame(CCRect(nameR.origin.x, nameR.origin.y+nameR.size.height+_px(20), nameR.size.width, _px(35)));
    m_pAuthor->setFontSize(cfSize);
    m_pAuthor->setColor(color);
    m_pAuthor->setText("作者:");
    this->addSubview(m_pAuthor);
    //题材
    m_pClass = CALabel::createWithFrame(CCRect(nameR.origin.x, m_pAuthor->getFrame().origin.y+cfSize+_px(15), nameR.size.width, _px(35)));
    m_pClass->setFontSize(cfSize);
    m_pClass->setColor(color);
    m_pClass->setText("题材:");
    this->addSubview(m_pClass);
    
    //最新收录
    m_pLastWorks = CALabel::createWithFrame(CCRect(nameR.origin.x, m_pClass->getFrame().origin.y+cfSize+_px(15), nameR.size.width, _px(80)));
    m_pLastWorks->setFontSize(cfSize);
    m_pLastWorks->setColor(color);
    m_pLastWorks->setNumberOfLine(2);
    m_pLastWorks->setText("最新收录:...");
    
    this->addSubview(m_pLastWorks);
    
    return true;
}




CAImageView* DMChartsCell::getPosition()
{
    return m_pPosition;
}
CommonUrlImageView* DMChartsCell::getMainImg()
{
    return m_pMainImg;
}
CALabel* DMChartsCell::getName()
{
    return m_pName;
}
CALabel* DMChartsCell::getAuthor()
{
    return m_pAuthor;
}
CALabel* DMChartsCell::getClass()
{
    return m_pClass;
}
CALabel* DMChartsCell::getLastWorks()
{
    return m_pLastWorks;
}


























