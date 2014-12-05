//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMMoreController__
#define __HelloCpp__DMMoreController__

#include <iostream>
#include "CartoonHouse.h"


class DMMoreController
: public CAViewController
,public CATableViewDataSource
,public CATableViewDelegate
{
    
public:
    
    DMMoreController();
    
    virtual ~DMMoreController();
    
    CREATE_FUNC(DMMoreController);
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    bool init();
    
    void onVersionDetectFinish(HttpResponseStatus status, const CSJson::Value& json);
    
    void onVersionAlertClick(int index);
    
    
protected:
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
    
    void onAlertViewClearTmp(int iButtonIndex);

    
protected:
    
    CATableView * m_pTableView;
    
    std::vector<std::string> m_Cpics;
    
    std::vector<std::string> m_Cnames;
    
    unsigned long m_uliTmpSize;

    
};

class MoreTableViewCell : public CATableViewCell
{
public:
    MoreTableViewCell();
    
    ~MoreTableViewCell();
    
    bool init(const char* reuseIdentifier, const CADipSize& cellSize);
    
    void setItemText(std::string text);
    
    void setItemImage(CAImage *image);
    
    void setAccessToryVisible(bool visible);
    
    void setBotLineVisible(bool visible);
    
protected:
    
    virtual void normalTableViewCell();
    
    virtual void highlightedTableViewCell();
    
    virtual void selectedTableViewCell();
    
    virtual void disabledTableViewCell();
    
protected:
    
    CALabel *m_pItemLable;
    
    CAImageView *m_pIemImage;
    
    CAImageView *m_pAccesstor;
    
    CAView * m_pBotLine;
    
    
    
};




#endif /* defined(__HelloCpp__DMMoreController__) */
