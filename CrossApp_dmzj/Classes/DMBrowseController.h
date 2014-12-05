//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMBrowseController__
#define __CartoonHouse__DMBrowseController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMLoginController.h"


class DMBrowseModel
{
public:
    
    ~DMBrowseModel()
    {
        imageUrls.clear();
    }
    
    std::string comic_id;
    
    std::string chapter_id;
    
    std::string chapter_name;
    
    std::vector<std::string> imageUrls;
    
    bool isDowloaded;
    
};

class DMBrowseVerticalView;
class DMBrowseHorizontalView;
class DMBrowseController:
public CAViewController,
public DMUserLoginDelegate,
public CommonUrlImageViewDelegate
{

public:
    
    DMBrowseController(const std::string& comicID, const std::vector<DMIntroductionItemInfo>& vec, int index);
    
    virtual ~DMBrowseController();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();

    float getTableViewCellHeight(unsigned int section, unsigned int index);
    
    float getTableViewSectionHeight(unsigned int section);
    
    void setCurrPage(int var);
    
    int detectingWhetherTurnThePage();
    
    CC_PROPERTY_IS(bool, m_bBrowseVertical, BrowseVertical);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sComicID, ComicID);
    
    CC_SYNTHESIZE(int, m_iCharIndex, CharIndex);
    
    CC_SYNTHESIZE(DMBrowseModel*, m_pPrevBrowseModel, PrevBrowseModel);
    
    CC_SYNTHESIZE(DMBrowseModel*, m_pCurrBrowseModel, CurrBrowseModel);
    
    CC_SYNTHESIZE(DMBrowseModel*, m_pNextBrowseModel, NextBrowseModel);

    CC_SYNTHESIZE_READONLY(int, m_nCurrPage, CurrPage);

    CC_SYNTHESIZE_READONLY(float, m_fTableViewCellInitialAspectRatio, TableViewCellInitialAspectRatio);
    
protected:
    
    void onButton(CAControl* control, CCPoint point);
    
    void setReversing(CAControl* control, CCPoint point);

    void onPrevRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onCurrRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onNextRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    virtual void imageViewRequestFinished(CommonUrlImageView* imageView);
    
    virtual void onActionPerformed(LoginAction action);
    
    void startLoginController();
    
    void onCollectionClicked();
    
    void addSubScirbeWithRemote();
    
    void onAddScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void delSubScribeWithRemote();
    
    void onDelScribeWithRemoteFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void updateCollectionStatus();
    
    void requestPrevData();
    
    void requestNextData();
    
    void requestFinishOnce();
    
    CommonUrlImageView* createWithUrlImageView(int* index);
    
    void showImage(CommonUrlImageView* imageView, int* index);

    void updateTitle();
    
    const std::string& getKeyAtSection(unsigned int section);
    
    bool isTheEndOfThePage();
    
    bool isTheTopOfThePage();
    
protected:
    
    bool m_bOnLineWatch;
    
    int m_iRequestFinish;
    
    CADeque<CommonUrlImageView*> m_pBrowseImageViews;
    
    CADeque<CALabel*> m_pPageLabels;
    
    std::map<std::string, std::vector<float> > m_fAspectRatioss;
    
    DMBrowseVerticalView* m_pVerticalView;
    
    DMBrowseHorizontalView* m_pHorizontalView;
    
    CAActivityIndicatorView* m_pLoading;

    std::vector<DMIntroductionItemInfo> m_obIntroductionItemInfos;
    
    friend class DMBrowseVerticalView;
    
    friend class DMBrowseHorizontalView;
};

class DMBrowseVerticalView
: public CAView
, public CAPageViewDelegate
, public CAScrollViewDelegate
{
    
public:
    
    DMBrowseVerticalView(DMBrowseController* controller);
    
    virtual ~DMBrowseVerticalView();
    
    bool init();
    
    void showBar();
    
    void hideBar();
    
    virtual void scrollViewWillBeginDragging(CAScrollView* view);
    
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const CCPoint& point);
    
    void updatePageView();
    
    void onSliderValueChanged(CAControl* control, CCPoint point);
    
    void onSliderTouchUpSide(CAControl* control, CCPoint point);
    
    void onOperationRemind(CAControl* control, CCPoint point);
    
    void onExchangedAboutModel(CAControl* control, CCPoint point);
    
    void onBrightnessView(CAControl* control, CCPoint point);
    
    void updateUI();
    
    void updateTitle();
    
protected:

    CALabel* m_pBrowseInfoLabel;
    
    CAButton* m_pbrightnessButton;
    
    CAButton* m_pCollectButton;
    
    CAButton* m_pDirectionButton;
    
    CAButton* m_pAboutButton;
    
    CASlider* m_pSlider;
    
    CASlider* m_pBrightnessSlider;
    
    CAImageView* m_pBrightnessBg;
    
    CALabel* m_pPageInfoLabel;
    
    CAPageView* m_pPageView;
    
    int m_iLastPage;
    
    bool m_bBarShow;
    
    CAView* m_pNavigationBar;
    
    CAView* m_pSetupBar;
    
    DMBrowseController* m_pController;
    
    bool m_bLeftModel;
};

class DMBrowseVerticalImageView
: public CAView
, public CommonUrlImageViewDelegate
{
public:
    
    DMBrowseVerticalImageView(const CADipSize& winSize);
    
    virtual ~DMBrowseVerticalImageView();
    
    bool init();

    void setImageView(CommonUrlImageView* var);
    
    void setLabel(CALabel* label);
    
    void reset();
    
    virtual void imageViewRequestFinished(CommonUrlImageView* imageView);
    
protected:
    
    CAScrollView* m_pScrollView;
    
    CommonUrlImageView* m_pImageView;
    
    CALabel* m_pLabel;
    
    CADipSize m_obWinSize;
};

class DMBrowseHorizontalView
: public CAView
, public CATableViewDataSource
, public CATableViewDelegate
, public CAScrollViewDelegate
{
    
public:
    
    DMBrowseHorizontalView(DMBrowseController* controller);
    
    virtual ~DMBrowseHorizontalView();
    
    bool init();
    
    void showBar();
    
    void hideBar();

public:
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);

    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void scrollViewWillBeginDragging(CAScrollView* view);
    
    virtual void scrollViewDidMoved(CAScrollView* view);

    void updateTableView();
    
    void updateOffset(int row);
    
    void updateReadInfo();
    
    float getNewTableViewContentOffset();
    
    float getNewTableViewContentOffsetAtIndex(int index);
    
    float getOldTableViewContentOffset();
    
    float getOldTableViewContentOffsetAtIndex(int index);
    
    int getIndexAtTableViewCurrContentOffset();

    void onSliderValueChanged(CAControl* control, CCPoint point);
    
    void onSliderTouchUpSide(CAControl* control, CCPoint point);

    void onOperationRemind(CAControl* control, CCPoint point);
    
    void onBrightnessView(CAControl* control, CCPoint point);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    void updateUI();
    
    void updateTitle();
    
protected:
    
    bool m_bUpdateTableView;
    
    CAVector<CAImage*> m_pImages;
    
    CALabel* m_pBrowseInfoLabel;
    
    CADeque<CAView*> m_pImageViews;
    
    CAButton* m_pbrightnessButton;
    
    CAButton* m_pCollectButton;
    
    CAButton* m_pDirectionButton;
    
    CASlider* m_pSlider;
    
    CASlider* m_pBrightnessSlider;
    
    CAImageView* m_pBrightnessBg;
    
    CALabel* m_pPageInfoLabel;

    bool m_bBarShow;

    CATableView* m_pTableView;
    
    CAView* m_pNavigationBar;
    
    CAView* m_pSetupBar;
    
    DMBrowseController* m_pController;
    
    bool m_bMoved;
};

#endif /* defined(__CartoonHouse__DMBrowseController__) */
