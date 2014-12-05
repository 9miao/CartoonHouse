//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMIntroductionController__
#define __HelloCpp__DMIntroductionController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMLoginController.h"

struct DMCommentModel{
    
public:
    
    DMCommentModel():replycount(0)
    {
    }
    
    std::string _id ;
    std::string uid ;
    std::string nickname ;
    std::string avatar_url ;
    std::string pid ;
    std::string comic_id ;
    std::string author_id ;
    std::string author ;
    std::string content ;
    std::string createtime ;
    std::string isused ;
    int replycount;
    vector<DMCommentModel> reply ;
};

const DMCommentModel dMCommentModelDefault = DMCommentModel();

static DMCommentModel converse(const CSJson::Value &data, DMCommentModel &model);

class DMIntroductionHeaderView;


typedef enum {
    LoginForReason_SubmitComment,
    LoginForReason_Collection
}LoginForReason;

class DMIntroductionHeaderView;
class DMIntroductionController
: public CAViewController
, public CACollectionViewDataSource
, public CACollectionViewDelegate
, public CATabBarDelegate
, public DMUserLoginDelegate
, public CATextFieldDelegate
, public CAScrollViewDelegate
{
    
    
public:
    
    DMIntroductionController(const std::string& ID, const std::string& title);
    
    virtual ~DMIntroductionController();
    
    static DMIntroductionController* create(const std::string& ID, const std::string& title);
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
	virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section);
    
	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);
    
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    void initWithCollectionView();
    
    void onReadBook(CAControl* control, CCPoint point);
    
    void onAddCollect(CAControl* control, CCPoint point);
    
    void onReturn(CAControl* control, CCPoint point);
    
    void onDownLoad(CAControl* control, CCPoint point);
    
    void onSort(CAControl* control, CCPoint point);
    
    void onAuthorClick(CAControl* control, CCPoint point);
    
    virtual void tabBarSelectedItem(CATabBar* tabBar, CATabBarItem* item, unsigned int index) ;
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void joinDownload();
    
    CACollectionViewCell* getItem1(const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    CACollectionViewCell* getItem2(const CCSize& cellSize, unsigned int section, unsigned int row);
    
    CACollectionViewCell* getItem3(const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    CACollectionViewCell* getCollectionCell(const CCSize& cellSize, unsigned int row);
    
    void updateCollectData();
    
    void updateCollectionStatus();
    
    virtual void onActionPerformed(LoginAction action);
    
    void addSubScirbeWithRemote();
    
    void delSubScribeWithRemote();
    
    void onAddScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onDelScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void requestCommentInfo();
    
    void onRequestCommonFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onRequestCommonReplyFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    CAButton *generateSubCommentView(DMCommentModel model ,string pstr,int width);
    
    void onFeedBackAction(CAControl *control,CCPoint cp);
    
    int getCommentCellHeight(DMCommentModel &model);
    
    int getReplyItemHeight(DMCommentModel &model);
    
    CCRect getMainCommentRect(DMCommentModel &model,int textsize);
    
    CCRect getFeedBackRect(CCRect &mainCommR);
    
    CCRect getSubFeedBacksRect(CCRect &repR,CCRect &contR,DMCommentModel &model);
    
    int getSubFeedBacksHeight(DMCommentModel &model);
    
    virtual bool getKeyBoardHeight(int height);
    
    virtual bool onTextFieldDetachWithIME(CATextField * sender);
    
    virtual bool onTextFieldAttachWithIME(CATextField * sender);
    
    virtual bool keyBoardCallBack(CATextField *sender);

    void getMoreCommentReplys(CAControl *control,CCPoint cp);
    
    void submitComment(DMCommentModel &model);
    
    void submitCommentFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    const DMCommentModel & getLocalCommentById(std::string id);
    
    void updateWanderRecord(int charIndex, bool isList, const std::string& readChapTitme, const std::string& comic_id);
    
    void startLoginController();
    
    std::string getID();
    
protected:
    
    typedef enum
    {
        DMIntroductionItem1 = 0,
        DMIntroductionItem2,
        DMIntroductionItem3
    }DMIntroductionItemType;
    
    LoginForReason _loginForReason;
    
    DMCommentModel _modelCacheForSubmit;
    
    std::string _cachedTextForSubmit;
    
    DMIntroductionItemType m_eType;
    
    CCSize m_obItemInterval[3];
    
    unsigned int m_uiItems[3];
    
    std::string m_sID;
    
    CACollectionView* m_pCollectionView;

    std::set<unsigned int> m_uNoItemRows;
    
    unsigned int m_uItemsRowCount;
    
    std::map<CAIndexPath3E, DMIntroductionItemInfo*> m_pItemInfos;
    
    std::vector<DMIntroductionItemInfo> m_obListItemInfos;
    
    std::vector<DMIntroductionItemInfo> m_obAloneItemInfos;
    
    std::vector<DMBriefInfo> m_obSimilarItemInfos;
    
    std::set<std::string> m_sHaveDownloadInfoIDs;
    
    CATabBar* m_pTabBarView;
    
    DMIntroductionHeaderView* m_pHeaderView;
    
    CAVector<CACollectionViewCell*> m_pViewCells;
    
    vector<DMCommentModel> m_DMCommentModels;
    
    float __scale;
    
    CAActivityIndicatorView* m_pLoading;
    
    CAView *m_pCommentInputer;
    
    CATextField *m_pCommentFiled;
    
    CAPullToRefreshView* m_pFooterView;
    
    int _iCollectionHeight;
    
    bool m_bReverse[2];
    
    std::string replyId ;
    
    bool isJoinDownload;
    
    friend class DMIntroductionHeaderView;
};

class DMIntroductionHeaderView: public CAView
{
public:
    
    DMIntroductionHeaderView(DMIntroductionController* controller);
    
    virtual ~DMIntroductionHeaderView();
    
    bool init();
    
    CC_SYNTHESIZE_READONLY(CommonUrlImageView* , m_pBookImageView, BookImageView);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pBookAuthorLabel, BookAuthorLabel);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pInfernalLabel, InfernalLabel);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pStatusLabel, StatusLabel);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pTheLatestLabel, TheLatestLabel);
    
    CC_SYNTHESIZE_READONLY(CAButton*, m_pAddCollectionButton, AddCollectionButton);
    
    CC_SYNTHESIZE_READONLY(CAButton*, m_pStartReadButton, StartReadButton);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pAddCollectionLable, AddCollectionLable);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pStartReadLable, StartReadLable);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pDetailsLabel, DetailsLabel);
    
    CC_SYNTHESIZE_READONLY(CAButton*, m_pOpenContractionButton, OpenContractionButton);
    
    CC_PROPERTY_PASS_BY_REF(DMIntroductionInfo, m_obIntroductionInfo, IntroductionInfo)
    
protected:
    
    DMIntroductionController* m_pController;
    
    bool m_bDetailsLabelShowAll;
    
public:
    
    void updateStartReadButton();

    void updateDetailsLabelState();
    
    void changedDetailsLabelState(CAControl* control, CCPoint point);
    
};

class DMIntroductionItemCell: public CACollectionViewCell
{
    
public:
    
    virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier, const CCSize& cellSize);
    
    CC_PROPERTY_IS(bool, m_bDisabled, Disabled);
    
    CC_PROPERTY_IS(bool, m_bHaveDownload, HaveDownload)
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pLabel, Label);
    
protected:

	virtual void normalCollectionViewCell();
    
	virtual void highlightedCollectionViewCell();
    
	virtual void selectedCollectionViewCell();
    
    virtual void disabledCollectionViewCell();
};

class DMIntroductionSimilarItemCell: public CACollectionViewCell
{
    
public:
    
    virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier, const CCSize& cellSize);
    
    CC_SYNTHESIZE_READONLY(CALabel*, m_pLabel, Label);
    
    CC_SYNTHESIZE_READONLY(CommonUrlImageView*, m_pImageView, ImageView);
    
protected:
    
	virtual void normalCollectionViewCell();
    
	virtual void highlightedCollectionViewCell();
    
	virtual void selectedCollectionViewCell();
};

class DMCommentCell: public CACollectionViewCell
{
    
    CC_SYNTHESIZE(CommonUrlImageView*, m_pHeadView, HeadView);
    
    CC_SYNTHESIZE(CALabel*, m_pTopName, TopName);
    
    CC_SYNTHESIZE(CALabel*, m_pDateView, DateView);
    
    CC_SYNTHESIZE(CALabel*, m_pMainComment, MainComment);
    
    CC_SYNTHESIZE(CAButton*, m_pBtnFeedBack, BtnFeedBack);
    
    CC_SYNTHESIZE(CAView*, m_pSubViewLayout, SubViewLayout);
    
    
public:
    
    virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier, const CCSize& cellSize);
    
    
protected:
    
    
    virtual void normalCollectionViewCell();
    
	virtual void highlightedCollectionViewCell();
    
	virtual void selectedCollectionViewCell();
    
    virtual void disabledCollectionViewCell();
};



#endif /* defined(__HelloCpp__DMIntroductionController__) */
