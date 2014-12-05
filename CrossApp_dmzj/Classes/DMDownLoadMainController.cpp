//
//  DMDownLoadMainController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-7-30.
//
//

#include "DMDownLoadMainController.h"
#include "DMSearchController.h"
#include "DMIntroductionController.h"

DMDownLoadMainController::DMDownLoadMainController()
{
    DMDownloadInfoManager::sharedDMDownloadInfoManager();
}

DMDownLoadMainController::~DMDownLoadMainController()
{

}

bool DMDownLoadMainController::init()
{
    if (!CAViewController::init()) {
        return false;
    }
    CABarButtonItem* buttonItem2 = CABarButtonItem::create("", CAImage::create(searchIcon), NULL);
    buttonItem2->setTarget(this, CAControl_selector(DMDownLoadMainController::onSearch));
    CANavigationBarItem* item = CANavigationBarItem::create("下载");
    item->addRightButtonItem(buttonItem2);
    this->setNavigationBarItem(item);
    
    
    
    return true;
}


void DMDownLoadMainController::onSearch(CAControl* control, CCPoint point)
{
    DMSearchController* searchController = DMSearchController::create();
    this->presentModalViewController(searchController, true);
}

void DMDownLoadMainController::onButton(CAControl* control, CCPoint point)
{
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(control->getSuperview());
    int tag = cell->getRow();
    DMIntroductionController *controller = DMIntroductionController::create(m_obDownloadInfos.at(tag).bookID, m_obDownloadInfos.at(tag).bookTitle);
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
}

void DMDownLoadMainController::viewDidLoad()
{
    CCRect rect = this->getView()->getBounds();
    
    m_pTableView = CATableView::createWithFrame(rect);
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setTableViewDelegate(this);
    this->getView()->addSubview(m_pTableView);
    m_pTableView->setAllowsSelection(true);
}

void DMDownLoadMainController::viewDidUnload()
{
    
}

void DMDownLoadMainController::viewDidAppear()
{
    m_obDownloadInfos.clear();
    std::vector<std::string> vec = CADownloadManager::getInstance()->getDownloadAllTextTags();
    DMDownloadInfoManager* infoManager = DMDownloadInfoManager::sharedDMDownloadInfoManager();
    
    for (int i=0; i<vec.size(); i++)
    {
        const string& ID = vec.at(i);
        const DMDownloadInfo& info =  infoManager->_select(ID);
        m_obDownloadInfos.push_back(info);
    }
    m_pTableView->reloadData();
}

CATableViewCell* DMDownLoadMainController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    DMDownLoadMainCell *cell = (DMDownLoadMainCell *)table->dequeueReusableCellWithIdentifier("cell");
    
    if(cell==NULL){
        cell = new DMDownLoadMainCell();
        cell->initWithReuseIdentifier("cell", CCSize(this->getView()->getFrame().size.width, _px(224)));
        cell->setAllowsSelected(false);
        cell->autorelease();
        
        CCRect browR = CCRect(cellSize.width-_px(130), _px(80), _px(100), _px(64));
        CAButton* btn = CAButton::createWithFrame(browR, CAButtonTypeRoundedRect);
        btn->setTitleForState(CAControlStateAll, "观看");
        btn->setTitleColorForState(CAControlStateAll, ccc4(255, 117, 33, 255));
        cell->addSubview(btn);
        btn->addTarget(this, CAControl_selector(DMDownLoadMainController::onButton), CAControlEventTouchUpInSide);
    }
    
    cell->setMainUrl(m_obDownloadInfos.at(row).imageUrl);
    cell->setDMName(m_obDownloadInfos.at(row).bookTitle);
    cell->setCacheHeader("已缓存：");
    cell->setCacheNum(m_obDownloadInfos.at(row).alreadyCache);
    cell->setNonCache("未缓存：" + m_obDownloadInfos.at(row).notCache);
    cell->setCache("共：" + m_obDownloadInfos.at(row).sizeExplain);
    
    
    return cell;
}

unsigned int DMDownLoadMainController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_obDownloadInfos.size();
}

unsigned int DMDownLoadMainController::numberOfSections(CATableView *table)
{
    return 1;
}

void DMDownLoadMainController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    DMIntroductionController *controller = DMIntroductionController::create(m_obDownloadInfos.at(row).bookID,
                                                                            m_obDownloadInfos.at(row).bookTitle);
    controller->joinDownload();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(controller, true);
    
    
}

unsigned int DMDownLoadMainController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(224);
}



//====================DMDownLoadMainCell====================

DMDownLoadMainCell::DMDownLoadMainCell()
{
}

DMDownLoadMainCell::~DMDownLoadMainCell()
{
}

bool DMDownLoadMainCell::initWithReuseIdentifier(const char* reuseIdentifier, const CCSize& cellSize)
{
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier)) {
        return false;
    }
    
    //-------已缓存，未缓存，大小的颜色---------
    CAColor4B contGrayColr = ccc4(153, 153, 153, 255);
    //-------已缓存，未缓存，大小的字体大小---------
    float contSize = _px(24);

    //左侧的图片
    CCRect bgRect = CCRect(_px(28), _px(28), _px(124), _px(164));
    CAScale9ImageView* bg = CAScale9ImageView::createWithImage(CAImage::create(book_bg));
    bg->setFrame(bgRect);
    this->addSubview(bg);
    
    CCRect mainImageR = CCRect(_px(32), _px(32), _px(116), _px(156));
    m_pMainImage = CommonUrlImageView::createWithFrame(mainImageR);
    this->addSubview(m_pMainImage);
    
    //标题
    CCRect tNameR = CCRect(_px(180), _px(30), cellSize.width-_px(180), _px(30));
    m_pDMName=CALabel::createWithFrame(tNameR);
    m_pDMName->setColor(ccc4(100, 100, 100, 255));
    m_pDMName->setFontSize(_px(30));
    m_pDMName->setNumberOfLine(1);
    this->addSubview(m_pDMName);
    
    //已存存标题区，相对于标题下面
    CCRect tCacTR = CCRect(tNameR.origin.x, tNameR.origin.y+tNameR.size.height+_px(20), _px(90), _px(24));
    m_pCacheHeader= CALabel::createWithFrame(tCacTR);
    m_pCacheHeader->setFontSize(contSize);
    m_pCacheHeader->setColor(contGrayColr);
    m_pCacheHeader->setNumberOfLine(1);
    this->addSubview(m_pCacheHeader);
    
    //已缓存数量区
    CCRect tCacNR = CCRect(tNameR.origin.x+tCacTR.size.width, tNameR.origin.y+tNameR.size.height+_px(20), _px(90), _px(24));
    m_pCacheNum= CALabel::createWithFrame(tCacNR);
    m_pCacheNum->setFontSize(contSize);
    m_pCacheNum->setColor(ccc4(255, 117, 33, 255));
    m_pCacheNum->setNumberOfLine(1);
    this->addSubview(m_pCacheNum);
    
    //未缓存
    CCRect tUnCR = CCRect(tNameR.origin.x, tCacTR.origin.y+tCacTR.size.height+_px(18), cellSize.width-_px(180), _px(24));
    m_pNonCache = CALabel::createWithFrame(tUnCR);
    m_pNonCache->setFontSize(contSize);
    m_pNonCache->setColor(contGrayColr);
    m_pNonCache->setNumberOfLine(1);
    this->addSubview(m_pNonCache);
    
    //大小
    CCRect tSizeR = CCRect(tNameR.origin.x, tUnCR.origin.y+tUnCR.size.height+_px(18), cellSize.width-_px(180), _px(24));
    m_pSize = CALabel::createWithFrame(tSizeR);
    m_pSize->setFontSize(contSize);
    m_pSize->setColor(contGrayColr);
    m_pSize->setNumberOfLine(1);
    this->addSubview(m_pSize);
    
    //观看
    CCRect browR = CCRect(cellSize.width-_px(130), _px(80), _px(100), _px(64));
    m_pBrowseBtn = CAButton::createWithFrame(browR, CAButtonTypeRoundedRect);
    m_pBrowseBtn->setTitleForState(CAControlStateAll, "观看");
    m_pBrowseBtn->setTitleColorForState(CAControlStateAll, ccc4(255, 117, 33, 255));
    this->addSubview(m_pBrowseBtn);
    
    
    
    return true;
}

void DMDownLoadMainCell::setMainUrl(const std::string &url)
{
    this->m_pMainImage->setUrl(url);
}

void DMDownLoadMainCell::setDMName(const std::string& name)
{
    this->m_pDMName->sizeToFit();
    this->m_pDMName->setText(name);
}

void DMDownLoadMainCell::setCacheHeader(const std::string& header)
{
    this->m_pCacheHeader->sizeToFit();
    this->m_pCacheHeader->setText(header);
}

void DMDownLoadMainCell::setCacheNum (const std::string& num)
{
    this->m_pCacheNum->sizeToFit();
    this->m_pCacheNum->setText(num);
}

void DMDownLoadMainCell::setNonCache (const std::string& nonCache)
{
    this->m_pNonCache->sizeToFit();
    this->m_pNonCache->setText(nonCache);
}

void DMDownLoadMainCell::setCache(const std::string& cache)
{
    this->m_pSize->sizeToFit();
    this->m_pSize->setText(cache);
}

CAButton* DMDownLoadMainCell::getBrowseText()
{
    return m_pBrowseBtn;
}

void DMDownLoadMainCell::normalTableViewCell()
{
    m_pBackgroundView->setColor(CAColor_white);
}

void DMDownLoadMainCell::highlightedTableViewCell()
{
    m_pBackgroundView->setColor(ccc4(200, 200, 200, 255));
}

void DMDownLoadMainCell::selectedTableViewCell()
{
    m_pBackgroundView->setColor(ccc4(200, 200, 200, 255));
}

void DMDownLoadMainCell::disabledTableViewCell()
{
    
}












