//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
#ifndef __HelloCpp__DMAboutUsController__
#define __HelloCpp__DMAboutUsController__

#include <iostream>
#include "CartoonHouse.h"


class DMAboutUsController
:public CAViewController
,public CATableViewDataSource
,public CATableViewDelegate
{
public:
    DMAboutUsController();
    
    ~DMAboutUsController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
protected:
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
private:
    CATableView *m_pTableView;
    
};


#endif /* defined(__HelloCpp__DMAboutUsController__) */
