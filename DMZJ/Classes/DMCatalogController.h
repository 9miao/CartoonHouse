//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMCatalogController__
#define __HelloCpp__DMCatalogController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMBriefInfoNanager.h"

class DMCatalogController
: public CAViewController
, public CACollectionViewDataSource
, public CACollectionViewDelegate
, public CAScrollViewDelegate
{
    
public:
    
    CREATE_FUNC(DMCatalogController);
    
    DMCatalogController();
    
    virtual ~DMCatalogController();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sTitle, Title);
    
protected:
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onButton(CAControl* control, CCPoint point);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);

    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void initWithCollectionView();
    
protected:
    
    CACollectionView* m_pCollectionView;

    CommonUrlImageView* m_pHeaderImageView;
    
    CALabel* m_pHeaderLabel;
    
    unsigned int m_uiItems;
    
    CAActivityIndicatorView* m_pLoading;
};


#endif /* defined(__HelloCpp__DMCatalogController__) */
