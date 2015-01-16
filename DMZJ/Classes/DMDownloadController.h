//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMDownloadController__
#define __CartoonHouse__DMDownloadController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMDownloadBookInfo.h"
#include "DMDownloadInfoManager.h"
class DMDownloadListCell;
class DMDownloadController
: public CAViewController
, public CACollectionViewDataSource
, public CACollectionViewDelegate
, public CAListViewDataSource
, public CAListViewDelegate
, public CAPageViewDelegate
, public CATabBarDelegate
, public CADownloadManagerDelegate
{
public:
    
    DMDownloadController(const DMIntroductionInfo& info, DMDownloadBookInfo* downloadInfo);
    
    virtual ~DMDownloadController();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    void onReturn(CAControl* control, CCPoint point);
    
    void onEdit(CAControl* control, CCPoint point);
    
    void onAllSelection(CAControl* control, CCPoint point);
    
    void onAllSelectionFromListView(CAControl* control, CCPoint point);
    
    void onDownload(CAControl* control, CCPoint point);
    
    void deleteDownloaded(CAControl* control, CCPoint point);
    
    void deleteDownloading(CAControl* control, CCPoint point);
    
    void onSort(CAControl* control, CCPoint point);
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index);
    
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
	virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);
    
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    
    virtual unsigned int numberOfIndex(CAListView *listView);
    
	virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index);
    
	virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const CCSize& cellSize, unsigned int index);
    
    virtual void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index);
    
    virtual void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index);
    
    virtual void onError(unsigned long download_id, CADownloadManager::ErrorCode errorCode);
    
    virtual void onProgress(unsigned long download_id, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded);
    
    virtual void onPauseDownload(unsigned long download_id);
    
    virtual void onResumeDownload(unsigned long download_id);
    
    virtual void onSuccess(unsigned long download_id);
    
    void initWithDownload();
    
    CC_SYNTHESIZE_RETAIN(DMDownloadBookInfo*, m_pDownloadInfo, DownloadInfo);
    
    CC_PROPERTY_IS(bool, m_bEdit, Edit);
    
protected:
    
    friend class DMDownloadListCell;
    
    CAActivityIndicatorView* m_pLoading;
    
    CATabBar* m_pTabBarView;
    
    CAPageView* m_pPageView;
    
    CACollectionView* m_pCollectionView;
    
    CAListView* m_pListView;
    
    CAButton* m_pDownloadControlbutton;

    CAButton* m_pEditButton;
    
    DMIntroductionInfo m_obInfo;
    
    std::set<std::string> m_obSelectedList;
    
    std::set<std::string> m_obEditDownloading;
    
    std::map<std::string, unsigned long> m_mDownloadProgresss;
    
    CAView* m_pSectionView[2];
    
    CAView* m_pEditButtonView0;
    
    CAView* m_pEditButtonView[2];
    
    int m_iSortRules;
    
    bool m_bDownLoadWatch;
    
    unsigned int m_uiItems;
};

class DMDownloadListCell: public CAListViewCell
{
public:
    
    static DMDownloadListCell* create(const std::string& reuseIdentifier, const CADipSize& size);
    
	virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier, const CADipSize& size);
    
    void setDownloadUrl(const std::string& url);
    
    void setDownloadController(DMDownloadController* controller);
    
    void update(float dt);
    
    void setPauseDownload(bool isPause, bool isCallback = false);
    
    void onButton(CAControl* control, CCPoint point);
    
    CC_PROPERTY_IS(bool, m_bEdit, Edit);
    
protected:
    
    virtual void normalListViewCell();
    
    virtual void highlightedListViewCell();
    
    virtual void selectedListViewCell();
    
    virtual void disabledListViewCell();
    
    void setFileSize(float size);
    
    void setTitle(const std::string& title);
    
    void setNowDownloadSize(unsigned long size);
    
    std::string m_sDownloadUrl;
    
    DMDownloadController* m_pController;
    
    CALabel* m_pChapterName;
    
    CALabel* m_pFileSize;
    
    CALabel* m_pProgressLabel;
    
    CALabel* m_pProgress2Label;
    
    CAProgress* m_pProgressView;
    
    CAButton* m_pButton;
    
    CAImageView* m_pIconImageView;
    
    CADipSize m_cSize;
    
    float m_fProgress;
    
    float m_fFileSize;
};

#endif /* defined(__CartoonHouse__DMDownloadController__) */
