

#include "DMCatalogCollectionCell.h"

DMCatalogCollectionCell::DMCatalogCollectionCell()
{

}

DMCatalogCollectionCell::~DMCatalogCollectionCell()
{
    
}

bool DMCatalogCollectionCell::init(const std::string& reuseIdentifier, const CCSize& cellSize)
{
    if (!CACollectionViewCell::initWithReuseIdentifier(reuseIdentifier))
    {
        return false;
    }
    
    this->setBackgroundView(NULL);
    
    CCRect rect;
    rect.size.width = cellSize.width;
    rect.size.height = cellSize.width * 21 / 16.0f;
    rect.origin.x = cellSize.width/2;
    rect.origin.y = cellSize.height/2 - _px(13);
    
    CAScale9ImageView* bg = CAScale9ImageView::createWithImage(CAImage::create(book_bg));
    bg->setCenter(rect);
    this->addSubview(bg);
    
    CCRect imageViewRect;
    imageViewRect.origin = rect.origin;
    imageViewRect.size.width = rect.size.width - 4;
    imageViewRect.size.height = rect.size.height - 4;
    m_pBookImageView = CommonUrlImageView::createWithCenter(imageViewRect);
    this->addSubview(m_pBookImageView);
    
    CCRect infoBgRect;
    infoBgRect.size.width = rect.size.width - 4;
    infoBgRect.size.height = _px(38);
    infoBgRect.origin.x = bg->getFrameOrigin().x + 2;
    infoBgRect.origin.y = bg->getFrameOrigin().y + bg->getFrame().size.height - infoBgRect.size.height - 2;
    CAImageView* bookInfoBg = CAImageView::createWithFrame(infoBgRect);
    bookInfoBg->setImage(CAImage::create(title_bg_other));
    this->addSubview(bookInfoBg);
    
    CCRect infoLabelRect;
    infoLabelRect.size.width = infoBgRect.size.width;
    infoLabelRect.size.height = infoBgRect.size.height;
    infoLabelRect.origin.x = 0;
    infoLabelRect.origin.y = infoBgRect.origin.y;
    m_pInfoLabel = CALabel::createWithFrame(infoLabelRect);
    m_pInfoLabel->setFontSize(_px(22));
    m_pInfoLabel->setTextAlignment(CATextAlignmentRight);
    m_pInfoLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_pInfoLabel->setColor(CAColor_white);
    this->addSubview(m_pInfoLabel);;
    
    CCRect labelRect;
    labelRect.size.width = cellSize.width;
    labelRect.size.height = _px(48);
    labelRect.origin.x = 0;
    labelRect.origin.y = cellSize.height - labelRect.size.height;
    m_pBookNameLabel = CALabel::createWithFrame(labelRect);
    m_pBookNameLabel->setFontSize(_px(28));
    //m_pBookNameLabel->setColor(ccc4(100, 100, 100, 255));
    m_pBookNameLabel->setTextAlignment(CATextAlignmentCenter);
    m_pBookNameLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentBottom);
    this->addSubview(m_pBookNameLabel);

    return true;
}

void DMCatalogCollectionCell::setBookNameText(const std::string& text)
{
    m_pBookNameLabel->setText(text);
}

void DMCatalogCollectionCell::setInfoText(const std::string& text)
{
    m_pInfoLabel->setText(text);
}

void DMCatalogCollectionCell::setBookImageFromUrl(const std::string& url)
{
    m_pBookImageView->setImage(CAImage::create("dm_resource/1.png"));
    m_pBookImageView->setUrl(url);
}

void DMCatalogCollectionCell::normalTableViewCell()
{
    this->setScale(1.0f);
}

void DMCatalogCollectionCell::highlightedTableViewCell()
{
    this->setScale(0.98f);
}

void DMCatalogCollectionCell::selectedTableViewCell()
{
    this->setScale(1.0f);
}

