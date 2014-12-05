//
//  DMMoreController.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-7-28.
//
//

#include "DMMoreController.h"
#include "DMSettingController.h"
#include "DMFeedBackController.h"
#include "DMAboutUsController.h"
#include "DMSettingManager.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


void getAllFilePaths(std::vector<std::string>& filePaths, std::vector<std::string>& dirPaths, const std::string& path)
{
    std::string p = path.at(path.length() - 1) == '/' ? path : path + "/";
    
    struct dirent *ptr;
    DIR* dir;
    dir = opendir(p.c_str());
    
    while ((ptr = readdir(dir)) != NULL)
    {
        std::string file_path = p + ptr->d_name;
        
        if (ptr->d_type == DT_DIR && ptr->d_name[0] != '.')
        {
            dirPaths.push_back(file_path);
            getAllFilePaths(filePaths, dirPaths, file_path);
        }
        else
        {
            filePaths.push_back(file_path);
        }
    }
    
    closedir(dir);
}

unsigned long getFilePathSize(const std::string& path)
{
    unsigned long lCurFileSize = 0;
    
    std::vector<std::string> filePaths;
    std::vector<std::string> dirPaths;
    getAllFilePaths(filePaths, dirPaths, path);
    
    for (int i=0; i<filePaths.size(); i++)
    {
        FILE* fp = fopen(filePaths.at(i).c_str(), "r");
        if (fp != NULL)
        {
            fseek(fp, 0L, SEEK_END);
            lCurFileSize += ftell(fp);
            fclose(fp);
        }
    }
    
    return lCurFileSize;
}

void removeFilePath(const std::string& path)
{
    std::vector<std::string> filePaths;
    std::vector<std::string> dirPaths;
    getAllFilePaths(filePaths, dirPaths, path);
    
    for (std::vector<std::string>::iterator itr=filePaths.begin();
         itr!=filePaths.end();
         itr++)
    {
        remove(itr->c_str());
    }
    
    for (std::vector<std::string>::iterator itr=dirPaths.begin();
         itr!=dirPaths.end();
         itr++)
    {
        remove(itr->c_str());
    }
}

#define ___FILE_PATH___ std::string(CCFileUtils::sharedFileUtils()->getWritablePath() + "image")

DMMoreController::DMMoreController()
:m_pTableView(NULL)
,m_uliTmpSize(0)
{
    
}

DMMoreController::~DMMoreController()
{
    
}

bool DMMoreController::init()
{
    if (!CAViewController::init()) {
        return false;
    }
    
    this->m_Cnames.push_back("设置");
    this->m_Cnames.push_back("版本更新");
    this->m_Cnames.push_back("意见反馈");
    this->m_Cnames.push_back("清除在线缓存(0.0M)");
    this->m_Cnames.push_back("关于我们");
    
    this->m_Cpics.push_back("dm_resource/more/more_setting.png");
    this->m_Cpics.push_back("dm_resource/more/more_verdetect.png");
    this->m_Cpics.push_back("dm_resource/more/more_feedback.png");
    this->m_Cpics.push_back("dm_resource/more/more_clean.png");
    this->m_Cpics.push_back("dm_resource/more/more_about.png");
    
    
    return true;
}

void DMMoreController::viewDidLoad(){
    
    
    CCRect rect = this->getView()->getBounds();
    
    m_pTableView = CATableView::createWithFrame(rect);
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setTableViewDelegate(this);
    this->getView()->addSubview(m_pTableView);
    m_pTableView->setAllowsSelection(true);
    
    this->m_pTableView->setBackGroundColor(ccc4(244, 244, 244, 255));
    

    RootWindow::getInstance()->setSecondNavigationController(this->getNavigationController());

}

void DMMoreController::viewDidUnload()
{
    
}

void DMMoreController::viewDidAppear()
{
    m_uliTmpSize = getFilePathSize(___FILE_PATH___);
    char str[128];
    sprintf(str, "清除在线缓存(%.1fM)", m_uliTmpSize / 1048576.0f);
    this->m_Cnames.at(3) = str;
    m_pTableView->reloadData();
}

CATableViewCell* DMMoreController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    MoreTableViewCell *cell = dynamic_cast<MoreTableViewCell *>(table->dequeueReusableCellWithIdentifier("moreCell")) ;
    
    if (cell==NULL)
    {
        CADipSize size = CADipSize(_dip(this->getView()->getFrame().size.width), 90);
        cell = new MoreTableViewCell();
        cell->init("moreCell", size);
        cell->setAllowsSelected(false);
        cell->autorelease();
    }
    cell->setBotLineVisible(false);
    
    switch (section) {
        case 0:
            switch (row) {
                case 0:
                    cell->setItemImage(CAImage::create(this->m_Cpics.at(0).c_str()));
                    cell->setItemText(this->m_Cnames.at(0));
                    cell->setAccessToryVisible(true);
                    break;
                    
                default:
                    break;
            }
            break;
        case 1:
            switch (row) {
                case 0:
                    cell->setItemImage(CAImage::create(this->m_Cpics.at(1).c_str()));
                    cell->setItemText(this->m_Cnames.at(1));
                    cell->setAccessToryVisible(false);
                    break;
                case 1:
                    cell->setItemImage(CAImage::create(this->m_Cpics.at(2).c_str()));
                    cell->setItemText(this->m_Cnames.at(2));
                    cell->setAccessToryVisible(true);
                    break;
                case 2:
                    cell->setItemImage(CAImage::create(this->m_Cpics.at(3).c_str()));
                    cell->setItemText(this->m_Cnames.at(3));
                    cell->setAccessToryVisible(false);
                    break;
            }
            break;
        case 2:
            switch (row) {
                case 0:
                    cell->setItemImage(CAImage::create(this->m_Cpics.at(4).c_str()));
                    cell->setItemText(this->m_Cnames.at(4));
                    cell->setAccessToryVisible(true);

                    break;
                 
            }
            break;
    }
    
    
    return cell;
}

unsigned int DMMoreController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    switch (section) {
        case 0:
            return 1;
            break;
        case 1:
            return 3;
            break;
        case 2:
            return 1;
            break;
    }
    return 0;
}

unsigned int DMMoreController::numberOfSections(CATableView *table)
{
    return 3;
}

CAView* DMMoreController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    
    CCRect rect = CCRect(0, 0, viewSize.width, viewSize.height);
    CAView *view = CAView::createWithFrame(rect);
    view->setColor(ccc4(244, 244, 244, 255));
    
    CCRect botR = CCRect(0,viewSize.height-_px(1),viewSize.width ,_px(1));
    CAView *botV = CAView::createWithFrame(botR);
    botV->setColor(ccc4(177, 177, 177, 255));
    
    
    switch (section) {
        case 0:
            view->addSubview(botV);
            break;
        case 1:
            view->addSubview(botV);
            break;
        case 2:
            view->addSubview(botV);
            break;
        default:
            break;
    }
    
    return view;
}


unsigned int DMMoreController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(90);
}



void DMMoreController:: tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

    switch (section) {
        case 0:
            switch (row) {
                //----------------------设置---------------------
                case 0:
                {
                    DMSettingController *setController = new DMSettingController();
                    setController->init();
                    setController->autorelease();
                    setController->setNavigationBarItem(CANavigationBarItem::create("设置"));
                    this->getNavigationController()->pushViewController(setController, true);
                }
                break;
            }
            break;
        case 1:
            switch (row) {
                //----------------------版本更新---------------------
                case 0:
                {
                    string url = "http://api.dmzj.com/dynamic/comicversion/android";
                    std::map<string, string> mm;
                    CommonHttpManager::getInstance()->send_get(url, mm, this, CommonHttpJson_selector(DMMoreController::onVersionDetectFinish));
                }
                    break;
                //----------------------意见反馈---------------------
                case 1:
                {
                    DMFeedBackController *fbController = new DMFeedBackController();
                    fbController->init();
                    fbController->autorelease();
                    fbController->setNavigationBarItem(CANavigationBarItem::create("意见反馈"));
                    this->getNavigationController()->pushViewController(fbController, true);
                }
                    break;
                //----------------------清除在线缓存---------------------
                case 2:
                {
                    CAAlertView* alert = CAAlertView::createWithText("", "确定要清除浏览缓存吗", "取消", "确定", NULL);
                    alert->show();
                    alert->setTarget(this, CAAlertView_selector(DMMoreController::onAlertViewClearTmp));
                }
                    break;
            }
            break;
        case 2:
            switch (row) {
                //----------------------关于我们---------------------
                case 0:
                {
                    DMAboutUsController *aboutController = new DMAboutUsController();
                    aboutController->init();
                    aboutController->autorelease();
                    aboutController->setNavigationBarItem(CANavigationBarItem::create("关于我们"));
                    this->getNavigationController()->pushViewController(aboutController, true);
                }
                    break;
            }
            break;
    }
    
    
    
    
    
    
}

void DMMoreController::onAlertViewClearTmp(int iButtonIndex)
{
    if (iButtonIndex == 1)
    {
        removeFilePath(___FILE_PATH___);
        this->m_Cnames.at(3) = "清除在线缓存(0.0M)";
        m_pTableView->reloadData();
    }
}

void DMMoreController::onVersionDetectFinish(HttpResponseStatus status, const CSJson::Value& json)
{
    std::string version = json["version"].asString();
    //无新版本
    if(version.compare(version_code)==0)
    {
        CAAlertView *alert = CAAlertView::createWithText("版本检测", "没有新版本", "确定", NULL);
        alert->setTarget(this, CAAlertView_selector(DMMoreController::onVersionAlertClick));
        alert->show();
    }
    else
    {
        CAAlertView *alert = CAAlertView::createWithText("版本检测", "有新版本，是否下载？", "取消", "确定", NULL);
        alert->setTarget(this, CAAlertView_selector(DMMoreController::onVersionAlertClick));
        alert->show();
    }
    
}

void DMMoreController::onVersionAlertClick(int index)
{
    switch (index) {
        case 0:
            break;
        case 1:
            //这里添加下载的代码
            break;
    }
}


unsigned int DMMoreController:: tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    
    switch (section) {
        case 0:
            return _px(30);
            break;
        case 1:
            return _px(50);
            break;
        case 2:
            return _px(50);
            break;
    }
    return 0;
}


MoreTableViewCell::MoreTableViewCell()
:m_pIemImage(NULL)
,m_pAccesstor(NULL)
,m_pBotLine(NULL)
,m_pItemLable(NULL)
{
    
}

MoreTableViewCell::~MoreTableViewCell()
{
    
}

bool MoreTableViewCell::init(const char* reuseIdentifier, const CADipSize& cellSize)
{
    if (!CATableViewCell::initWithReuseIdentifier(reuseIdentifier)) {
        return false;
    }
    
    CADipRect iconRect = CADipRect(30, 20, 50, 50);
    this->m_pIemImage = CAImageView::createWithFrame(iconRect);
    this->addSubview(m_pIemImage);
    
    CADipRect lableRect = CADipRect(100+(cellSize.width-100)/2, cellSize.height/2, cellSize.width-100, 50);

    this->m_pItemLable = CALabel::createWithCenter(lableRect);
    this->m_pItemLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->m_pItemLable->setFontSize(_px(32));
    this->addSubview(m_pItemLable);
    
    CADipRect accessRect = CADipRect(cellSize.width-30, 30, 10, 10);
    this->m_pAccesstor = CAImageView::createWithFrame(accessRect);
    this->addSubview(m_pAccesstor);
    
    CADipRect botR = CADipRect(0, cellSize.height-1, cellSize.width, 1);
    this->m_pBotLine = CAView::createWithFrame(botR);
    this->m_pBotLine->setColor(ccc4(177, 177, 177, 255));
    this->addSubview(m_pBotLine);
    this->m_pBotLine->setVisible(false);
   
    
    return true;
}

void MoreTableViewCell::setItemText(std::string text)
{
    CC_RETURN_IF(this->m_pItemLable == NULL);
    this->m_pItemLable->setText(text);
}

void MoreTableViewCell::setItemImage(CAImage *image)
{
    CC_RETURN_IF(this->m_pIemImage == NULL);
    this->m_pIemImage->setImage(image);
}

void MoreTableViewCell::setAccessToryVisible(bool visible)
{
    CC_RETURN_IF(this->m_pAccesstor == NULL);
    this->m_pAccesstor->setVisible(visible);
}

void MoreTableViewCell::setBotLineVisible(bool visible)
{
    CC_RETURN_IF(this->m_pBotLine == NULL);
    this->m_pBotLine->setVisible(visible);
}

void MoreTableViewCell::normalTableViewCell()
{
    m_pBackgroundView->setColor(CAColor_white);
}

void MoreTableViewCell::highlightedTableViewCell()
{
    m_pBackgroundView->setColor(ccc4(200, 200, 200, 255));
}

void MoreTableViewCell::selectedTableViewCell()
{
    m_pBackgroundView->setColor(ccc4(200, 200, 200, 255));
}

void MoreTableViewCell::disabledTableViewCell()
{
    
}










