//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMSearchController__
#define __HelloCpp__DMSearchController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMSearchHistoryManager.h"

struct DMSearchResultModel {
    std::string Id;
    std::string title;
    std::string last_update_chapter_name;
    std::string cover;
    std::string authors;
    std::string types;
    std::string sum_chapters;
    std::string sum_source;
};

static DMSearchResultModel parseDMSearchResultModel(const CSJson::Value &json);

struct DMSearchHotModel{
    std::string _id;
    std::string name;
    CAColor4B bacColor;
};

class DMSearchIndexView: public CAView ,public CATableViewDataSource ,public CATableViewDelegate
{
private:
    
    CATableView *m_pIndexTableView;
    
    CAViewController *m_pBeLongedController;
    
    DMSearchHistoryManager *m_pHistoryManager;
    
    vector<DMModelSearchHistory> historys;
    
    vector<vector< DMSearchHotModel > >* m_pHotModels;
    
public:
    
    DMSearchIndexView();
    
    virtual ~DMSearchIndexView();
    
    static DMSearchIndexView* createWithFrame(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect);
    
    void reloadData(vector<vector< DMSearchHotModel > >* var);
    
    void cleanHistory();
    
    CAViewController * getBeLongedController();
    
    void setBeLongedController(CAViewController *viewController);
    
public:
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
    
    
};

class DMSearchResultTabeViewAssist : public CAView , public CATableViewDataSource ,public CATableViewDelegate
{
    
private:
    
    CAViewController *viewController;
    
    
public:
    
    CC_SYNTHESIZE(CATableView *, m_pSearchResultTabeView, SearchResultTabeView);
    
    CC_SYNTHESIZE(std::vector<DMSearchResultModel>, m_pDataModels, DataModels);
    
    DMSearchResultTabeViewAssist();
    
    virtual ~DMSearchResultTabeViewAssist();
    
    static DMSearchResultTabeViewAssist* createWithFrame(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect);
    
    void reloadData(const CSJson::Value& json);
    
    void setViewController(CAViewController *control);
    
    CAViewController *getViewController();
    
public:
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
};



class DMSearchBriefTabeViewAssist : public CAView , public CATableViewDataSource ,public CATableViewDelegate
{
private:
    
    
    CAViewController *viewController;
    
    
public:
    
    CC_SYNTHESIZE(CATableView *, m_pSearchBrifeTabeView, SearchBrifeTabeView);
    
    CC_SYNTHESIZE(std::vector<DMSearchResultModel>, m_pDataModels, DataModels);
    
    DMSearchBriefTabeViewAssist();
    
    virtual ~DMSearchBriefTabeViewAssist();
    
    static DMSearchBriefTabeViewAssist* createWithFrame(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect);
    
    void reloadData(const CSJson::Value& json);
    
    void setViewController(CAViewController *control);
    
    CAViewController *getViewController();
    
public:
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
};

class DMSearchController: public CAViewController , public CATextFieldDelegate
{
    
public:
    
    DMSearchController();
    
    virtual ~DMSearchController();
    
    CREATE_FUNC(DMSearchController);
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    //全部搜索
    void comicsearch(std::string value);
    
    void onComicRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    //模糊搜索
    void comicfuzzysearch(std::string value);
    
    void onComicfuzzyFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    //热门搜索
    void comicsearchhot();
    
    void onComicSearchHotFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    //If the sender doesn't want to insert the text, return true;
    virtual bool onTextFieldInsertText(CATextField * sender, const char * text, int nLen);
    
    //If the sender doesn't want to delete the delText, return true;
    virtual bool onTextFieldDeleteBackward(CATextField * sender, const char * delText, int nLen);
    
    virtual bool onTextFieldDetachWithIME(CATextField * sender);
    
    virtual bool keyBoardCallBack(CATextField *sender);
    
    void onKeyWordConfirmed(CAControl*, CCPoint);
    
    //显示热搜词层
    void showLayerHot();
    
    //输入时，显示概要信息层
    void showLayerBrifeInfo();
    
    //显示基本信息层
    void showLayerInfo();
    
    void initWithTableView();
    
    void saveSearchKeyword(std::string keyword);
    
protected:
    
    void onButton(CAControl* control, CCPoint point);
    
private:
    
    CAActivityIndicatorView* m_pLoading;
    
public:
    
    CC_SYNTHESIZE(DMSearchResultTabeViewAssist * ,m_pSearchResultView, SearchResultView);
    
    CC_SYNTHESIZE(DMSearchBriefTabeViewAssist * ,m_pSearchBriefView, SearchBriefView);
    
    CC_SYNTHESIZE(DMSearchIndexView * ,m_pDMSearchIndexView, DMSearchIndexView);
    
    CC_SYNTHESIZE(vector<vector< DMSearchHotModel > > ,m_pHotModels, HotModels);
    
    CC_SYNTHESIZE(CATextField * ,m_pKeyWordTextFiled, KeyWordTextFiled);
    
    CC_SYNTHESIZE(std::string, m_ExtraKeyword, ExtraKeyWord);
    
};

class DMSearchResultCell :public CATableViewCell
{
    
public:
    
    DMSearchResultCell();
    
    virtual ~DMSearchResultCell();
    
    virtual bool init(const char* reuseIdentifier, const CCSize& cellSize);
    
public:
    //主图片
    CommonUrlImageView * m_pMainImg;
    //动漫名
    CALabel *m_pName;
    //作者
    CALabel *m_pAuthor;
    //题材
    CALabel *m_pClass;
    //最新收录
    CALabel *m_pLasted;
    
    
};

class DMSearchBrifeCell :public CATableViewCell
{
    
public:
    
    DMSearchBrifeCell();
    
    virtual ~DMSearchBrifeCell();
    
    virtual bool init(const char* reuseIdentifier, const CCSize& cellSize);
    
    void setData(DMSearchResultModel &model);
    
    void refreshFrame(float dt = 0);
    
public:
    //动漫名
    CALabel *m_pName;
    //最新收录
    CALabel *m_pLasted;
    
    
};


#endif /* defined(__HelloCpp__DMSearchController__) */
