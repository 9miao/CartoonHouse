//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMClassifCatalogViewController__
#define __CartoonHouse__DMClassifCatalogViewController__

#include <iostream>
#include "CartoonHouse.h"

class DMClassifCatalogViewController
: public CAViewController
, public CACollectionViewDataSource
, public CACollectionViewDelegate
, public CAScrollViewDelegate
{
    
public:
    
    DMClassifCatalogViewController(const std::string& ID);
    
    virtual ~DMClassifCatalogViewController();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onButton(CAControl* control, CCPoint point);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    void initWithCollectionView();
    
protected:
    
    CACollectionView* m_pCollectionView;

    std::vector<DMBriefInfo> m_vDatas;
    
    std::string m_sID;
    
    unsigned int m_uiItems;
    
    int m_nDataPage;
    
    CAActivityIndicatorView* m_pLoading;
    
    bool m_bFooterRefreshing;
};


#endif /* defined(__CartoonHouse__DMClassifCatalogViewController__) */
