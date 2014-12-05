//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMChartsController__
#define __HelloCpp__DMChartsController__

#include <iostream>
#include "CartoonHouse.h"

struct TopModel{
    std::string _id;
    std::string title;
    std::string last_update_chapter_name;
    std::string cover;
    std::string authors;
    std::string types;
    std::string sum_chapters;
    std::string sum_source;
};

class DMChartsController :public CAViewController
,public CATableViewDataSource
,public CATableViewDelegate
{
public:
    
    DMChartsController();
    
    ~DMChartsController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    void initWithTableView();
    
private:
    
    void onBackClick();
    
    void onSegmented(CAControl* control, CCPoint point);
    
    void searchTopItems();
    
    void onSearchFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    TopModel convertTopModel(const CSJson::Value &value);
    
    void onReturn(CAControl* control, CCPoint point);
    
private:
    
    CATableView *m_pTableView;
    CATableView *m_pMonthView;
    CATableView *m_pmWeekView;
    
    vector<TopModel> all_Set;
    vector<TopModel> month_Set;
    vector<TopModel> week_Set;
    vector<TopModel> current_Set;
    
    vector<CATableView*> m_pCurrTableViews;
    
    CAActivityIndicatorView* m_pLoading;
    
};

class DMChartsCell :public CATableViewCell
{
    
public:
    
    DMChartsCell();
    
    virtual ~DMChartsCell();
    
    virtual bool init(const char* reuseIdentifier, const CCSize& cellSize);
    
private:

    
    //排名
    CAImageView *m_pPosition;
    //漫画主图
    CommonUrlImageView *m_pMainImg;
    //漫画名称
    CALabel *m_pName;
    //漫画作者
    CALabel *m_pAuthor;
    //题材
    CALabel *m_pClass;
    //最新收录
    CALabel *m_pLastWorks;
    
public:
    
    CAImageView* getPosition();
    CommonUrlImageView* getMainImg();
    CALabel* getName();
    CALabel* getAuthor();
    CALabel* getClass();
    CALabel* getLastWorks();
    
    
};


#endif /* defined(__HelloCpp__DMChartsController__) */









