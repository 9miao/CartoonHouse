//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
#ifndef __HelloCpp__DMCatalogCollectionCell__
#define __HelloCpp__DMCatalogCollectionCell__

#include <iostream>
#include "CartoonHouse.h"

class DMCatalogCollectionCell: public CACollectionViewCell
{
    
public:
    
    DMCatalogCollectionCell();
    
    virtual ~DMCatalogCollectionCell();
    
    virtual bool init(const std::string& reuseIdentifier, const CCSize& cellSize);
    
    void setBookNameText(const std::string& text);
    
    void setInfoText(const std::string& text);
    
    void setBookImageFromUrl(const std::string& url);
    
protected:

    virtual void normalTableViewCell();
    
    virtual void highlightedTableViewCell();
    
    virtual void selectedTableViewCell();
    
    CALabel* m_pBookNameLabel;
    
    CALabel* m_pInfoLabel;
    
    CommonUrlImageView* m_pBookImageView;
    
};


#endif /* defined(__HelloCpp__DMCatalogCollectionCell__) */
