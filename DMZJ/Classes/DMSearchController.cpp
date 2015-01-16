

#include "DMSearchController.h"
#include "DMSearchHistoryManager.h"
#include "DMIntroductionController.h"


#pragma mark -DMSearchController
//#############################DMSearchController#############################

DMSearchController::DMSearchController()
: m_pLoading(NULL)
, m_pDMSearchIndexView(NULL)
, m_pSearchBriefView(NULL)
, m_pSearchResultView(NULL)
, m_pKeyWordTextFiled(NULL)
{
}

DMSearchController::~DMSearchController()
{
    m_pHotModels.clear();
}


void DMSearchController::viewDidLoad()
{
    CCRect rect = this->getView()->getBounds();
    rect.size.height = _px(96);
    
    CAView* view = CAView::createWithFrame(rect, ccc4(87, 154, 244, 255));
    this->getView()->insertSubview(view, 1);
    
    CAButton* button = CAButton::createWithFrame(CADipRect(_dip(rect.size.width) - 120, 0, 120, 96), CAButtonTypeCustom);
    button->setTitleForState(CAControlStateNormal, "取消");
    button->setTitleColorForState(CAControlStateNormal, CAColor_white);
    button->setTitleColorForState(CAControlStateHighlighted, CAColor_gray);
    view->addSubview(button);
    button->addTarget(this, CAControl_selector(DMSearchController::onButton), CAControlEventTouchUpInSide);
    
    /****/
    //顶部搜索框
    CCRect sBacR = CCRect(_px(30), _px(15), this->getView()->getFrame().size.width-button->getFrame().size.width-_px(40), _px(65));
    CAView *searchBac = CAView::createWithFrame(sBacR);
    searchBac->setColor(CAColor_clear);
    CAScale9ImageView *scaleImageView = CAScale9ImageView::create();
    scaleImageView->setFrame(CCRect(0, 0, sBacR.size.width, sBacR.size.height));
    scaleImageView->setImage(CAImage::create("dm_resource/search/search_bac.png"));
    searchBac->addSubview(scaleImageView);
    this->getView()->insertSubview(searchBac, 2);
    //搜索框中的放大镜
    CCRect sIconR =CCRect(_px(15), _px(9), _px(45), _px(45));
    CAImageView *searchIcon = CAImageView::createWithFrame(sIconR);
    searchIcon->setImage(CAImage::create("dm_resource/title_search.png"));
    scaleImageView->addSubview(searchIcon);
    //搜索框
    m_pKeyWordTextFiled =CATextField::createWithFrame(CCRect(sIconR.origin.x+sIconR.size.width+_px(10), _px(sIconR.origin.y), sBacR.size.width - sIconR.size.width - _px(40), sIconR.size.height));
    scaleImageView->addSubview(m_pKeyWordTextFiled);
    m_pKeyWordTextFiled->setDelegate(this);
    m_pKeyWordTextFiled->setVertMargins(_px(5));
    CCRect bacR = CCRect(0, 0, m_pKeyWordTextFiled->getFrame().size.width, m_pKeyWordTextFiled->getFrame().size.height);
    m_pKeyWordTextFiled->setBackgroundView(CAView::createWithFrame(bacR,CAColor_clear));
    m_pKeyWordTextFiled->setPlaceHolder("漫画名、作者");
    m_pKeyWordTextFiled->setSpaceHolderColor(CAColor_white);
    m_pKeyWordTextFiled->setTextColor(CAColor_white);
    m_pKeyWordTextFiled->setKeyboardReturnType(KEY_BOARD_RETURN_SEARCH);
    
    this->comicsearchhot();
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
        m_pLoading->setLoadingMinTime(0.5f);
        m_pLoading->setColor(CAColor_white);
        this->getView()->insertSubview(m_pLoading, CAWindowZoderTop);
        m_pLoading->setTargetOnCancel(this, callfunc_selector(DMSearchController::initWithTableView));
        
        
        
    }
    
    
    
    
}

void DMSearchController::viewDidUnload()
{
    
    
}

void DMSearchController::initWithTableView()
{
    //搜索结果部分
    //全部搜索布局
    CCRect searchResultR = CCRect(0, _px(96), this->getView()->getBounds().size.width, this->getView()->getBounds().size.height - _px(96));
    m_pSearchResultView = DMSearchResultTabeViewAssist::createWithFrame(searchResultR);
    m_pSearchResultView->setVisible(false);
    m_pSearchResultView->setViewController(this);
    this->getView()->addSubview(m_pSearchResultView);
    //模糊搜索布局
    m_pSearchBriefView = DMSearchBriefTabeViewAssist::createWithFrame(searchResultR);
    m_pSearchBriefView->setVisible(false);
    m_pSearchBriefView->setViewController(this);
    this->getView()->addSubview(m_pSearchBriefView);
    //热门搜索布局
    m_pDMSearchIndexView = DMSearchIndexView::createWithFrame(searchResultR);
    this->getView()->addSubview(m_pDMSearchIndexView);
    m_pDMSearchIndexView->setBeLongedController(this);
    m_pDMSearchIndexView->reloadData(&m_pHotModels);
    
    if (m_ExtraKeyword.length()>0) {
        comicsearch(m_ExtraKeyword);
        m_pKeyWordTextFiled->setText(m_ExtraKeyword);
        saveSearchKeyword(m_ExtraKeyword);
    }
    
}

//全部搜索
void DMSearchController::comicsearch(std::string value)
{
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeComicSearch, EncodeURL(value));
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMSearchController::onComicRequestFinished));
}

void DMSearchController::onComicRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    showLayerInfo();
    m_pSearchResultView->reloadData(json["data"]);
}



//模糊搜索
void DMSearchController::comicfuzzysearch(std::string value)
{
    if (value.length()>0) {
//        m_pSearchBriefView->setVisible(true);
        showLayerBrifeInfo();
    }else{
//        m_pSearchBriefView->setVisible(false);
        showLayerHot();
    }

    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeComicFuzzySearch , EncodeURL(value));
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMSearchController::onComicfuzzyFinished));
}

//显示热搜词层
void DMSearchController::showLayerHot()
{
    m_pDMSearchIndexView->setVisible(true);
    m_pSearchBriefView->setVisible(false);
    m_pSearchResultView->setVisible(false);
}

//输入时，显示概要信息层
void DMSearchController::showLayerBrifeInfo()
{
    m_pDMSearchIndexView->setVisible(false);
    m_pSearchBriefView->setVisible(true);
    m_pSearchResultView->setVisible(false);
}

//显示基本信息层
void DMSearchController::showLayerInfo()
{
    m_pDMSearchIndexView->setVisible(false);
    m_pSearchBriefView->setVisible(false);
    m_pSearchResultView->setVisible(true);
}


void DMSearchController::onComicfuzzyFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (m_pSearchBriefView)
    {
        m_pSearchBriefView->reloadData(json["data"]);
    }
   
}

//热门搜索
void DMSearchController::comicsearchhot()
{
    HTTP_URL::S_HTTP_URL s_http_url = HTTP_URL::get_HTTP_URL(HTTP_URL::HttpUrlTypeComicSearchHot);
    CommonHttpManager::getInstance()->send_post(s_http_url.url, s_http_url.key_value, this, CommonHttpJson_selector(DMSearchController::onComicSearchHotFinished));
}

//关键词搜索
void DMSearchController::onKeyWordConfirmed(CAControl* control, CCPoint ccp)
{
    
    CAButton *button = ((CAButton*)control);
    comicsearch(button->getTextTag());
    saveSearchKeyword(button->getTextTag());
    m_pKeyWordTextFiled->setText(button->getTextTag());
    
}

void DMSearchController::saveSearchKeyword(std::string keyword)
{
    DMSearchHistoryManager mgr;
    DMModelSearchHistory history;
    history._keyword = keyword;
    mgr.delteKey(keyword);
    mgr.save(history);
}


void DMSearchController::onComicSearchHotFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    vector<CAColor4B> colors;
    colors.push_back(ccc4(146, 207, 104, 255));
    colors.push_back(ccc4(88, 181, 255, 255));
    colors.push_back(ccc4(250, 101, 103, 255));
    colors.push_back(ccc4(252, 201, 86, 255));
    colors.push_back(ccc4(246, 119, 74, 255));
    
    const CSJson::Value& json0 = json["data"];
    
    int colorPos = 0;
    vector<DMSearchHotModel> models;
    for (int i=0; i<json0.size(); i++)
    {
        colorPos = rand()%5;
        const CSJson::Value& value = json0[i];
        string _id = value["id"].asString();
        string name = value["name"].asString();
        DMSearchHotModel model ;
        model._id = _id;
        model.name = name;
        model.bacColor = colors.at(colorPos);
        models.push_back(model);
    }

    //两边=50 热门两边=25
    
    float tWidth = this->getView()->getFrame().size.width-_px(100);
    float externalW = _px(50);
    float hotSize =_px(28);
    vector<DMSearchHotModel> midvector;
    for (int i=0; i<models.size(); i++) {
        midvector.push_back(models.at(i));
        int totalW = 0;
        for (int j =0; j<midvector.size(); j++) {
            totalW += (CAImage::getStringWidth("", hotSize, midvector.at(j).name.c_str())+externalW);
        }
        
        if ((totalW > tWidth && midvector.size()>1) || (midvector.size()>4)) {
            vector<DMSearchHotModel > tarVec;
            for (int m =0 ; m<midvector.size()-1; m++) {
                tarVec.push_back(midvector.at(m));
            }
            m_pHotModels.push_back(tarVec);
            midvector.clear();
            midvector.push_back(models.at(i));
            totalW=0;
        }
        if(totalW > tWidth && midvector.size()==1){
            vector<DMSearchHotModel > tarVec;
            tarVec.push_back(midvector.at(0));
            m_pHotModels.push_back(tarVec);
            midvector.clear();
            totalW=0;
        }
    }
    
    m_pLoading->stopAnimating();
}

void DMSearchController::onButton(CAControl* control, CCPoint point)
{
    this->dismissModalViewController(true);
}

//If the sender doesn't want to insert the text, return true;
bool DMSearchController::onTextFieldInsertText(CATextField * sender, const char * text, int nLen)
{
    string crrent = sender->getText();
    comicfuzzysearch(crrent);
    
    return false;
}

//If the sender doesn't want to delete the delText, return true;
bool DMSearchController::onTextFieldDeleteBackward(CATextField * sender, const char * delText, int nLen)
{
    string crrent = sender->getText();
    comicfuzzysearch(crrent);
    
    return false;
}

bool DMSearchController::onTextFieldDetachWithIME(CATextField * sender)
{
    
    if (sender->getText().length()==0) {
        return false;
    }
    
    if (sender->getText().length()>0 && m_pSearchBriefView->getDataModels().size()==0) {
        showLayerHot();
    }
    
//    showLayerInfo();
//    string crrent = sender->getText();
//    comicsearch(crrent);
    
    return false;
}

bool DMSearchController::keyBoardCallBack(CATextField *sender)
{
    
    if (sender->getText().length()==0) {
        return false;
    }
    
    showLayerInfo();
    string crrent = sender->getText();
    saveSearchKeyword(sender->getText());
    comicsearch(crrent);
    return false;
}

#pragma mark -DMSearchResultCell
//#############################DMSearchResultCell#############################

DMSearchResultCell::DMSearchResultCell()
{
}
DMSearchResultCell:: ~DMSearchResultCell()
{
}
bool DMSearchResultCell::init(const char* reuseIdentifier, const CCSize& cellSize)
{
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier)) {
        return false;
    }
    //图片
    int padding = _px(2);
    CCRect imgR = CCRect(_px(30), _px(30), _px(160), _px(210));
    CCRect bacR = CCRect(imgR.origin.x-padding, imgR.origin.y-padding, imgR.size.width+2*padding, imgR.size.height+2*padding);
    CAScale9ImageView *bacImg = CAScale9ImageView::createWithImage(CAImage::create("dm_resource/catalog/book_bg.png"));
    bacImg->setFrame(bacR);
    this->addSubview(bacImg);
    m_pMainImg = CommonUrlImageView::createWithFrame(imgR);
    this->addSubview(m_pMainImg);
    
    //----公用右侧宽度和起始值----
    float rightRestWidth = cellSize.width - (imgR.origin.x+imgR.size.width+_px(60));
    float rightOrix = imgR.origin.x+imgR.size.width+_px(30);
    //标题
    CCRect nameR = CCRect(rightOrix, _px(30), rightRestWidth, _px(34));
    m_pName = CALabel::createWithFrame(nameR);
    m_pName->setColor(ccc4(100, 100, 100, 255));
    m_pName->setFontSize(_px(30));
    m_pName->sizeToFit();
    this->addSubview(m_pName);
    
    CAColor4B dColor = ccc4(153, 153, 153, 255);
    float dSize = _px(24);
    //作者
    CCRect authorR = CCRect(rightOrix, nameR.origin.y + nameR.size.height + _px(20), rightRestWidth, _px(24));
    m_pAuthor = CALabel::createWithFrame(authorR);
    m_pAuthor->setColor(dColor);
    m_pAuthor->setFontSize(dSize);
    m_pAuthor->sizeToFit();
    this->addSubview(m_pAuthor);
    
    //题材
    CCRect classR = CCRect(rightOrix, authorR.origin.y+authorR.size.height +_px(18), rightRestWidth, _px(24));
    m_pClass = CALabel::createWithFrame(classR);
    m_pClass->setColor(dColor);
    m_pClass->setFontSize(dSize);
    m_pClass->sizeToFit();
    this->addSubview(m_pClass);
    
    //最新收录
    CCRect lastedR = CCRect(rightOrix, classR.origin.y+authorR.size.height +_px(18), rightRestWidth, _px(48));
    m_pLasted = CALabel::createWithFrame(lastedR);
    m_pLasted->setNumberOfLine(2);
    m_pLasted->setColor(dColor);
    m_pLasted->setFontSize(dSize);
    m_pLasted->sizeToFit();
    this->addSubview(m_pLasted);
    
    return true;
}


static DMSearchResultModel parseDMSearchResultModel(const CSJson::Value &json)
{
    DMSearchResultModel model ;
    model.Id = json["id"].asString();
    model.last_update_chapter_name = json["last_update_chapter_name"].asString();
    model.cover = json["cover"].asString();
    model.authors = json["authors"].asString();
    model.types = json["types"].asString();
    model.sum_chapters = json["sum_chapters"].asString();
    model.sum_source = json["sum_source"].asString();
    model.title = json["title"].asString();
    return model;
}


#pragma mark -DMSearchIndexView
//#############################DMSearchIndexView#############################


DMSearchIndexView::DMSearchIndexView()
{
}

DMSearchIndexView::~DMSearchIndexView()
{
    CC_SAFE_DELETE(m_pHistoryManager);
    historys.clear();
}

DMSearchIndexView* DMSearchIndexView::createWithFrame(const CCRect& rect)
{
    DMSearchIndexView *view= new DMSearchIndexView();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

bool DMSearchIndexView::initWithFrame(const CCRect& rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
    CCRect r = this->getFrame();
    m_pIndexTableView = CATableView::createWithFrame(this->getBounds());
    m_pIndexTableView->setSeparatorViewHeight(0);
    this->addSubview(m_pIndexTableView);
    m_pIndexTableView->setTableViewDataSource(this);
    m_pIndexTableView->setTableViewDelegate(this);
    m_pHistoryManager = new DMSearchHistoryManager();
    historys = m_pHistoryManager->getRecent();
    return true;
}


CAViewController * DMSearchIndexView::getBeLongedController()
{
    return m_pBeLongedController;
}

void DMSearchIndexView::setBeLongedController(CAViewController *viewController)
{
    this->m_pBeLongedController = viewController;
}

void DMSearchIndexView::reloadData(vector<vector< DMSearchHotModel > >* var)
{
    m_pHotModels = var;
    m_pIndexTableView->reloadData();
}

void DMSearchIndexView::cleanHistory()
{
    m_pHistoryManager->clearHistory();
    historys.clear();
    m_pIndexTableView->reloadData();
}


CATableViewCell* DMSearchIndexView::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    switch (section) {
        //#######################(历史记录)#######################
        case 0:
        {
            DMModelSearchHistory first = historys.at(2*row);
            DMModelSearchHistory second ;
            
            if (historys.size() > (2*row+1)) {
                second = historys.at(2*row+1);
            }
            
            CATableViewCell *cell = table->dequeueReusableCellWithIdentifier("historycell");
            if (cell== NULL) {
                cell = CATableViewCell::create("historycell");
                CAButton *buttonL = CAButton::create(CAButtonTypeCustom);
                buttonL->setTitleColorForState(CAControlStateAll, CAColor_black);
                buttonL->setVisible(false);
                buttonL->setTag(10);
                cell->addSubview(buttonL);
                
                CAButton *buttonR = CAButton::create(CAButtonTypeCustom);
                buttonR->setTitleColorForState(CAControlStateAll, CAColor_black);
                buttonR->setVisible(false);
                buttonR->setTag(20);
                cell->addSubview(buttonR);
            }
            
            CAButton *leftButton = (CAButton *)cell->getSubviewByTag(10);
            CAButton *rightButton = (CAButton *)cell->getSubviewByTag(20);
            
            
            leftButton->setVisible(false);
            rightButton->setVisible(false);
            if (first._id!=-1) {
                leftButton->setVisible(true);
                leftButton->setTitleForState(CAControlStateNormal, first._keyword);
                int width = CAImage::getStringWidth("", _px(30), first._keyword.c_str());
                width = width > ((cellSize.width-_px(100))/2) ? (cellSize.width-_px(100))/2 : width;
                CCRect rect = CCRect(_px(50), (cellSize.height-_px(30))/2, width, _px(55));
                leftButton->setFrame(rect);
            }
            if (second._id!=-1) {
                rightButton->setVisible(true);
                rightButton->setTitleForState(CAControlStateNormal, second._keyword);
                
                int width = CAImage::getStringWidth("", _px(30), second._keyword.c_str());
                width = width > ((cellSize.width-_px(100))/2) ? (cellSize.width-_px(100))/2 : width;
                
                CCRect rect = CCRect(cellSize.width/2, (cellSize.height-_px(30))/2, width, _px(55));
                rightButton->setFrame(rect);
            }
            
            leftButton->setTextTag(first._keyword);
            leftButton->addTarget(this->getBeLongedController(), CAControl_selector(DMSearchController::onKeyWordConfirmed), CAControlEventTouchUpInSide);
            rightButton->setTextTag(second._keyword);
            rightButton->addTarget(this->getBeLongedController(), CAControl_selector(DMSearchController::onKeyWordConfirmed), CAControlEventTouchUpInSide);
            
            return cell;
        }
            break;
        //#######################(热词搜索)#######################
        case 1:
        {
            int max = 0;
            for (int i = 0 ; i<m_pHotModels->size(); i++) {
                if (m_pHotModels->at(i).size()>max) {
                    max= m_pHotModels->size();
                }
            }
            
            vector<DMSearchHotModel> model = m_pHotModels->at(row);
            CATableViewCell *cell = table->dequeueReusableCellWithIdentifier("hotcell");
            
            if (cell==NULL) {
                cell = CATableViewCell::create("hotcell");
                for (int i = 0 ; i< max; i++) {
                    CAButton *button = CAButton::create(CAButtonTypeCustom);
                    button->setTitleColorForState(CAControlStateAll, CAColor_white);
                    button->setTag(i*10);
                    button->setVisible(false);
                    cell->addSubview(button);
                }
            }
            
            for (int i =0 ; i<max; i++) {
                cell->getSubviewByTag(i*10)->setVisible(false);
            }
            
            for (int i=0; i<model.size(); i++) {
                //设置Frame
                CAButton *button = (CAButton *) cell->getSubviewByTag(i*10);
                button->setVisible(true);
                float txtW = CAImage::getStringWidth("", _px(28), model.at(i).name.c_str());
                int nx = _px(50);
                if (i>0) {
                    CCRect froR =  cell->getSubviewByTag((i-1)*10)->getFrame();
                    nx = froR.origin.x + froR.size.width + _px(15);
                }
                
                CCRect btnR = CCRect( nx, _px(15), txtW+_px(50), _px(60));
                if (txtW >cellSize.width-_px(150)) {
                    btnR.size.width = cellSize.width-_px(100);
                }
                button->setFrame(btnR);
                //设置颜色
                CCRect bacR = CCRect(0, 0, btnR.size.width, btnR.size.height);
                CAView *bacV = CAView::createWithFrame(bacR);
                bacV->setColor(model.at(i).bacColor);
                button->setBackGroundViewForState(CAControlStateNormal, bacV);
                //设置文字
                button->setTitleForState(CAControlStateAll, model.at(i).name);
                button->setTextTag(model.at(i).name);
                button->addTarget(this->getBeLongedController(), CAControl_selector(DMSearchController::onKeyWordConfirmed), CAControlTouchUpInSide);
                
                
            }
            
            return cell;
        }
            break;
    }
    return NULL;
}

unsigned int DMSearchIndexView::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    switch (section) {
        case 0:
            return historys.size()%2 == 0 ? historys.size()/2 : historys.size()/2+1;
            break;
        case 1:
            return m_pHotModels->size();
    }
    return 0;
}

unsigned int DMSearchIndexView::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    switch (section) {
        case 0:
            return _px(70);
            break;
        case 1:
            return _px(90);
            break;
    }
    return _px(90);
}

void DMSearchIndexView::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
}

unsigned int DMSearchIndexView::numberOfSections(CATableView *table)
{
    return 2;
}

unsigned int DMSearchIndexView::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return _px(90);
}

CAView* DMSearchIndexView::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    CAView *bacView = CAView::createWithFrame(CCRect(0, 0, viewSize.width, viewSize.height));
    float margin = _px(50);
    CALabel * titleLable = CALabel::createWithFrame(CCRect(margin, _px(30), (this->getFrame().size.width-2*margin)/2, _px(60)));
    titleLable->setColor(ccc4(100, 100, 100, 255));
    titleLable->setFontSize(_px(26));
    bacView->addSubview(titleLable);
    switch (section) {
        case 0:
        {
            titleLable->setText("搜索历史");
            float xL = viewSize.width - (_px(50) + CAImage::getStringWidth("", _px(26), "搜索历史"));
            float yT = (viewSize.height - _px(60))/2;
            CCRect cleR = CCRect(xL, yT, CAImage::getStringWidth("", _px(26), "搜索历史"), _px(60));
            CAButton *historyCleaner = CAButton::createWithFrame(cleR, CAButtonTypeCustom);
            historyCleaner->setTitleColorForState(CAControlStateNormal, ccc4(255, 177, 33, 255));
            historyCleaner->setTitleForState(CAControlStateAll, "清空记录");
            historyCleaner->addTarget(this, CAControl_selector(DMSearchIndexView::cleanHistory), CAControlEventTouchUpInSide);
            bacView->addSubview(historyCleaner);
        }
            break;
        case 1:
        {
            titleLable->setText("热门搜索");
        }
            break;
    }
    CCRect botR = CCRect(margin, viewSize.height-_px(1), viewSize.width-2*margin, _px(1));
    CAView *botLine = CAView::createWithFrame(botR);
    botLine->setColor(CAColor_gray);
    bacView->addSubview(botLine);
    
    return bacView;
}



#pragma mark -DMSearchResultTabeViewAssist
//#############################DMSearchResultTabeViewAssist#############################

DMSearchResultTabeViewAssist::DMSearchResultTabeViewAssist()
{
    
}

DMSearchResultTabeViewAssist:: ~DMSearchResultTabeViewAssist()
{
    m_pDataModels.clear();
}

DMSearchResultTabeViewAssist* DMSearchResultTabeViewAssist::createWithFrame(const CCRect& rect)
{
    
    DMSearchResultTabeViewAssist *view= new DMSearchResultTabeViewAssist();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

bool DMSearchResultTabeViewAssist::initWithFrame(const CCRect& rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
    CCRect r = this->getFrame();
    m_pSearchResultTabeView = CATableView::createWithFrame(this->getBounds());
    this->addSubview(m_pSearchResultTabeView);
    m_pSearchResultTabeView->setTableViewDataSource(this);
    m_pSearchResultTabeView->setTableViewDelegate(this);
    m_pSearchResultTabeView->setAllowsSelection(true);
    return true;
}

void DMSearchResultTabeViewAssist::reloadData(const CSJson::Value& json)
{
    m_pDataModels.clear();
    for (int i=0; i<json.size(); i++) {
        const CSJson::Value& value = json[i];
        m_pDataModels.push_back(parseDMSearchResultModel(value));
    }
    m_pSearchResultTabeView->reloadData();
}


CATableViewCell* DMSearchResultTabeViewAssist::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    DMSearchResultCell *cell = (DMSearchResultCell *)table->dequeueReusableCellWithIdentifier("cell");
    
    if (cell==NULL) {
        cell = new DMSearchResultCell();
        cell->init("cell", CCSize(this->getFrame().size.width, _px(270)));
    }
    
    DMSearchResultModel model = m_pDataModels.at(row);
    
    cell->m_pMainImg->setUrl(model.cover);
    cell->m_pName->setText(model.title);
    cell->m_pAuthor->setText(model.authors);
    cell->m_pClass->setText(model.types);
    cell->m_pLasted->setText(model.last_update_chapter_name);
    return cell;
}

unsigned int DMSearchResultTabeViewAssist::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_pDataModels.size();
}

unsigned int DMSearchResultTabeViewAssist::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(270);
}

void DMSearchResultTabeViewAssist::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    DMSearchResultModel modle = m_pDataModels.at(row);
    DMIntroductionController *controller = DMIntroductionController::create(modle.Id, modle.title);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
    this->getViewController()->dismissModalViewController(false);
    
}

void DMSearchResultTabeViewAssist::setViewController(CAViewController *control)
{
    this->viewController = control;
}

CAViewController * DMSearchResultTabeViewAssist::getViewController()
{
    return viewController;
}


#pragma mark -DMSearchBriefTabeViewAssist
//#############################DMSearchBriefTabeViewAssist#############################

DMSearchBriefTabeViewAssist::DMSearchBriefTabeViewAssist()
{
}

DMSearchBriefTabeViewAssist:: ~DMSearchBriefTabeViewAssist()
{
    m_pDataModels.clear();
}

DMSearchBriefTabeViewAssist* DMSearchBriefTabeViewAssist::createWithFrame(const CCRect& rect)
{
    DMSearchBriefTabeViewAssist *view= new DMSearchBriefTabeViewAssist();
    if (view && view->initWithFrame(rect)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return NULL;
}

bool DMSearchBriefTabeViewAssist::initWithFrame(const CCRect& rect)
{
    if (!CAView::initWithFrame(rect)) {
        return false;
    }
    CCRect r = this->getFrame();
    m_pSearchBrifeTabeView = CATableView::createWithFrame(this->getBounds());
    this->addSubview(m_pSearchBrifeTabeView);
    m_pSearchBrifeTabeView->setTableViewDataSource(this);
    m_pSearchBrifeTabeView->setTableViewDelegate(this);
    m_pSearchBrifeTabeView->setAllowsSelection(true);
    return true;
}

void DMSearchBriefTabeViewAssist::reloadData(const CSJson::Value& json)
{
    m_pDataModels.clear();
    for (int i=0; i<json.size(); i++) {
        const CSJson::Value& value = json[i];
        m_pDataModels.push_back(parseDMSearchResultModel(value));
    }
    
    
    m_pSearchBrifeTabeView->reloadData();
}

CATableViewCell* DMSearchBriefTabeViewAssist::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    DMSearchBrifeCell *cell = (DMSearchBrifeCell *)table->dequeueReusableCellWithIdentifier("cell");
    
    if (cell==NULL) {
        cell = new DMSearchBrifeCell();
        cell->init("cell", CCSize(this->getFrame().size.width, _px(90)));
    }
    
    DMSearchResultModel model = m_pDataModels.at(row);
    
    cell->setData(model);
    
    return cell;
    
}

unsigned int DMSearchBriefTabeViewAssist::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_pDataModels.size();
}

unsigned int DMSearchBriefTabeViewAssist::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(90);
}

void DMSearchBriefTabeViewAssist::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
    
    DMSearchController *controller = (DMSearchController *)this->getViewController();
    controller->showLayerInfo();
    std::vector<DMSearchResultModel> dataModels;
    dataModels.push_back(m_pDataModels.at(row));
    controller->getSearchResultView()->setDataModels(dataModels);
    controller->getSearchResultView()->getSearchResultTabeView()->reloadData();
    if (controller->getKeyWordTextFiled()->getText().length()>0) {
        controller->saveSearchKeyword(controller->getKeyWordTextFiled()->getText());
    }
}

void DMSearchBriefTabeViewAssist::setViewController(CAViewController *control)
{
    this->viewController = control;
}

CAViewController * DMSearchBriefTabeViewAssist::getViewController()
{
    return viewController;
}

#pragma mark -DMSearchBrifeCell
//#############################DMSearchBrifeCell#############################

DMSearchBrifeCell::DMSearchBrifeCell()
{
}

DMSearchBrifeCell:: ~DMSearchBrifeCell()
{
}

void DMSearchBrifeCell::setData(DMSearchResultModel &model)
{
    this->m_pName->setText(model.title);
    this->m_pLasted->setText(model.last_update_chapter_name);
    CAScheduler::schedule(schedule_selector(DMSearchBrifeCell::refreshFrame), this, 1/60.0f, 0, 0);
}

void DMSearchBrifeCell::refreshFrame(float dt)
{
    CCRect pLastR = this->m_pName->getFrame();
    printf("pLastR.size.width = %f" ,pLastR.size.width);
    pLastR.origin.x = this->m_pName->getFrame().origin.x + this->m_pName->getFrame().size.width +_px(20);
    this->m_pLasted->setFrame(pLastR);
}

bool DMSearchBrifeCell::init(const char* reuseIdentifier, const CCSize& cellSize)
{
    
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier)) {
        return false;
    }
    //标题
    CCRect titleR = CCRect(_px(30), _px(30), _px(30), _px(36));
    m_pName = CALabel::createWithFrame(titleR);
    m_pName->setColor(ccc4(100, 100, 100, 255));
    m_pName->setNumberOfLine(1);
    m_pName->sizeToFit();
    m_pName->setFontSize(_px(30));
    this->addSubview(m_pName);
    
    //最近更新
    m_pLasted = CALabel::createWithFrame(CCRect(titleR.origin.x+titleR.size.width+_px(10), _px(30), _px(30), _px(36)));
    m_pLasted->setColor(ccc4(70, 134, 241, 255));
    m_pLasted->setNumberOfLine(1);
    m_pLasted->sizeToFit();
    m_pLasted->setFontSize(_px(30));
    this->addSubview(m_pLasted);
    
    return true;

    
}















