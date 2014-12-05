//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
#ifndef __HelloCpp__DMSubscribeController__
#define __HelloCpp__DMSubscribeController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMIntroductionInfoManager.h"


struct DMModelSubscribe{
    std::string sub_id;
    std::string sub_name;
    std::string sub_img;
    std::string sub_update;
    
};

class DMSubscribeController
: public CAViewController
, public CATableViewDataSource
, public CATableViewDelegate
, public CAPageViewDelegate
{
    
public:
    
    DMSubscribeController();
    
    virtual ~DMSubscribeController();
    
    CREATE_FUNC(DMSubscribeController);
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
protected:
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onSegmented(CAControl* control, CCPoint point);
    
    void onButton1(CAControl* control, CCPoint point);
    
    void onButton2(CAControl* control, CCPoint point);
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    void requestSubscribeData(std::string id1);
    
    void onSubScribeFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    //0 空  ; 1 订阅 ; 2 历史记录 
    void showStatusLable(int flag);
    
    
protected:
    
    CATableView* m_pTableView1;
    
    CATableView* m_pTableView2;
    
    CAPageView* m_pPageView;
    
    CASegmentedControl* m_pSegmentedControl;
    
    //暂无订阅|暂无历史标签
    CALabel * m_pStatusLable;
    
private:
    
    vector<DMModelSubscribe> subscribes;
    
    vector<DMBrowseInfo> introductionInfos;
    
    DMIntroductionInfoManager *m_pIntroInfoManager;
    
};

class DMSubscribeTableCell: public CATableViewCell
{
    
public:
    
    DMSubscribeTableCell();
    
    virtual ~DMSubscribeTableCell();
    
    static DMSubscribeTableCell* create(const std::string& reuseIdentifier, const CADipSize& size);
    
    virtual bool initWithReuseIdentifier(const std::string& reuseIdentifier, const CADipSize& size);

    void setBookIconImage(CAImage* image);
    
    void setBookIconUrl(const std::string& url);
    
    void setBookTitleText(const std::string& text);
    
    void setBookAuthorText(const std::string& text);
    
    void setReadProgressText(const std::string& text);
    
    void setBookUpdateWhereText(const std::string& text);
    
    void setReadRecordText(const std::string& text);
    
    void visibleAuthorBook(bool visible);
    
    
protected:
    
    virtual void normalTableViewCell();
    
    virtual void highlightedTableViewCell();
    
    virtual void selectedTableViewCell();
    
    virtual void disabledTableViewCell();
    
protected:
    
    
    CommonUrlImageView* m_pBookIcon;
    
    CALabel* m_pBookTitle;
    
    CALabel* m_pBookAuthor;
    
    CALabel* m_pReadProgress;
    
    CALabel* m_pBookUpdateWhere;
    
    CALabel* m_pReadRecord;
    
    
    
    
};

#endif /* defined(__HelloCpp__DMSubscribeController__) */
