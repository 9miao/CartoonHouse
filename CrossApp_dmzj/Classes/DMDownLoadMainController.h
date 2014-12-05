//
//  DMDownLoadMainController.h
//  HelloCpp
//
//  Created by liuguoyan on 14-7-30.
//
//
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
#ifndef __HelloCpp__DMDownLoadMainController__
#define __HelloCpp__DMDownLoadMainController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMDownloadInfoManager.h"

class DMDownLoadMainController :public CAViewController
,public CATableViewDataSource
,public CATableViewDelegate
{
public:
    
    DMDownLoadMainController();
    
    ~DMDownLoadMainController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
protected:
    
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    void onSearch(CAControl* control, CCPoint point);
    
    void onButton(CAControl* control, CCPoint point);
    
private:
    
    CATableView * m_pTableView;
    
    std::vector<DMDownloadInfo> m_obDownloadInfos;
};

class DMDownLoadMainCell : public CATableViewCell
{
public:
    
    DMDownLoadMainCell();
    
    virtual ~DMDownLoadMainCell();
    
    virtual bool initWithReuseIdentifier(const char* reuseIdentifier,const CCSize& cellSize);
     
    void setMainUrl(const std::string& url);
    
    void setDMName(const std::string& name);
    
    void setCacheHeader(const std::string& header);
    
    void setCacheNum (const std::string& num);
    
    void setNonCache (const std::string& nonCache);
    
    void setCache(const std::string& cache);
    
    CAButton* getBrowseText();
    
protected:
    
    virtual void normalTableViewCell();
    
    virtual void highlightedTableViewCell();
    
    virtual void selectedTableViewCell();
    
    virtual void disabledTableViewCell();
    
private:
    //主图片
    CommonUrlImageView *m_pMainImage;
    
    //漫画名称
    CALabel *m_pDMName;
    //缓存的前端标题（因为缓存有两种色，故分两个Lable）
    CALabel *m_pCacheHeader;
    //缓存的数量
    CALabel *m_pCacheNum;
    //未缓存
    CALabel *m_pNonCache;
    //大小
    CALabel *m_pSize;
    
    //右侧观看按钮
    CAButton *m_pBrowseBtn;
    
    
};



#endif /* defined(__HelloCpp__DMDownLoadMainController__) */
