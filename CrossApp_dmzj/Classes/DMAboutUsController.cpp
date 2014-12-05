

#include "DMAboutUsController.h"
#include "UrlBrowser/OpenBrowser.h"
#define COLOR_BAC ccc4(244,244,244,255)

DMAboutUsController::DMAboutUsController()
{
}


DMAboutUsController::~DMAboutUsController()
{
}

bool DMAboutUsController::init()
{
    if (!CAViewController::init()) {
        return false;
    }

    return true;
}

void DMAboutUsController::DMAboutUsController::viewDidLoad()
{
    
    CCRect conRect = this->getView()->getFrame();
    
    m_pTableView = CATableView::createWithFrame(CCRect(0, 0, conRect.size.width, conRect.size.height));
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setTableViewDelegate(this);
    m_pTableView->setAllowsSelection(true);
    this->getView()->addSubview(m_pTableView);
    
    float width = conRect.size.width- _px(98);
    CCRect fWR = CCRect(_px(30), conRect.size.height-_px(54), width/2, _px(24));
    CCRect sWR = CCRect(_px(30)+width/2, conRect.size.height-_px(54), width/2, _px(24));
    CALabel *fWareLable = CALabel::createWithFrame(fWR);
    CALabel *sWareLabel = CALabel::createWithFrame(sWR);
    fWareLable->setFontSize(_px(24));
    sWareLabel->setFontSize(_px(24));
    CAColor4B color = ccc4(153, 153, 153, 255);
    fWareLable->setColor(color);
    sWareLabel->setColor(color);
    fWareLable->setText("漫画讨论群:260284741");
    sWareLabel->setText("动漫综合群:107765908");
    
    this->getView()->addSubview(fWareLable);
    this->getView()->addSubview(sWareLabel);
    
}

void DMAboutUsController::DMAboutUsController::viewDidUnload()
{
}

CATableViewCell* DMAboutUsController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{
    
    CATableViewCell *cell = table->dequeueReusableCellWithIdentifier("cell");
    if (cell==NULL) {
        
        cell = CATableViewCell::create("cell");
        cell->setAllowsSelected(false);
        CCRect lableR = CCRect(_px(30), _px(29), cellSize.width*0.6, _px(50));
        CALabel *lable = CALabel::createWithFrame(lableR);
        lable->setFontSize(_px(32));
        lable->setColor(ccc4(60, 60, 60, 255));
        lable->setTag(100);
        cell->addSubview(lable);
        
        CCRect accR = CCRect(cellSize.width-_px(45), _px(30), _px(30) , _px(30));
        CAImageView *accImage = CAImageView::createWithFrame(accR);
        accImage->setImage(CAImage::create("dm_resource/comm_arraw.png"));
        accImage->setTag(101);
        cell->addSubview(accImage);

    }
    CALabel *labe = (CALabel*)cell->getSubviewByTag(100);
    switch (row) {
        case 0:
            labe->setText("访问CrossApp官网");
            break;
        case 1:
            labe->setText("访问9秒官网");
            break;
        case 2:
            labe->setText("去评个分呗");
            break;
        case 3:
            labe->setText("分享给好友");
            break;
        default:
            break;
    }
    
    return cell;
}

unsigned int DMAboutUsController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 4;
}

unsigned int DMAboutUsController::numberOfSections(CATableView *table)
{
    return 1;
}

void DMAboutUsController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    switch (row) {
        case 0:
            OpenBrowser::openUrl("http://crossapp.9miao.com");
            break;
        case 1:
            OpenBrowser::openUrl("http://www.9miao.com");
            break;
        case 2:

            break;
        default:
            break;
    }
}
unsigned int DMAboutUsController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return _px(120);
}

CAView* DMAboutUsController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    
    CCRect bacR = CCRect(0, 0, viewSize.width, _px(120));
    CAView *headerBac = CAView::createWithFrame(bacR);
    headerBac->setColor(COLOR_BAC);
    
    CAImageView *imagev = CAImageView::createWithFrame(CCRect(_px(30), _px(30), _px(292), _px(82)));
    imagev->setImage(CAImage::create("dm_resource/more/more_about_slog.png"));
    
    headerBac->addSubview(imagev);
    
    
    CCRect laR = CCRect(viewSize.width-_px(250), _px(80), _px(220), _px(30));
    CALabel *lable = CALabel::createWithFrame(laR);
    lable->setTextAlignment(CATextAlignmentRight);
    lable->setColor(ccc4(60, 60, 60, 255));
    lable->setText(version);
    lable->setFontSize(_px(30));
    
    
    headerBac->addSubview(lable);
    
    return headerBac;
}

unsigned int DMAboutUsController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(90);
}
