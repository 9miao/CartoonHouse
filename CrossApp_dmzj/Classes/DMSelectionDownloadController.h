//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMSelectionDownloadController__
#define __CartoonHouse__DMSelectionDownloadController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMDownloadBookInfo.h"

class DMSelectionDownloadController
: public CAViewController
, public CACollectionViewDataSource
, public CACollectionViewDelegate
{
public:
    
    DMSelectionDownloadController(const DMIntroductionInfo& info,
                                  const std::vector<DMIntroductionItemInfo>& listItemInfos,
                                  const std::vector<DMIntroductionItemInfo>& aloneItemInfos);
    
    virtual ~DMSelectionDownloadController();

    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    void onReturn(CAControl* control, CCPoint point);
    
    void onSearch(CAControl* control, CCPoint point);
    
    void onAllSelection(CAControl* control, CCPoint point);
    
    void onDownload(CAControl* control, CCPoint point);
    
    void onSort(CAControl* control, CCPoint point);
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
	virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);
    
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    bool isDisabledFromUrl(const std::string& url);
    
    CC_SYNTHESIZE_RETAIN(DMDownloadBookInfo*, m_pDownloadInfo, DownloadInfo);
    
protected:
    
    CACollectionView* m_pCollectionView;
    
    std::vector<DMIntroductionItemInfo> m_obListItemInfos;
    
    std::vector<DMIntroductionItemInfo> m_obAloneItemInfos;
    
    DMIntroductionInfo m_obInfo;
    
    CAView* m_pSectionView[2];
    
    unsigned int m_uiItems;
};


#endif /* defined(__CartoonHouse__DMSelectionDownloadController__) */
