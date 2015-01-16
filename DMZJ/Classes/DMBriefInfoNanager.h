//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMBriefInfoNanager__
#define __HelloCpp__DMBriefInfoNanager__

#include <iostream>
#include "CartoonHouse.h"

struct DMBriefInfoManager
{
    DMBriefInfoManager();
    
    virtual ~DMBriefInfoManager();
    
    void _saved(const DMBriefInfo& model);
    
    void _delete(const string& ID);
    
    DMBriefInfo _select(const string& ID);
    
    void _updateCover(const DMBriefInfo& model);
    
    void _updateTitle(const DMBriefInfo& model);
    
    void _updateSubtitle(const DMBriefInfo& model);
    
protected:
    
    sqlite3 *_sqlite3;
    
    sqlite3_stmt *_sqlite_stmt_insert;
    sqlite3_stmt *_sqlite_stmt_remove;
    sqlite3_stmt *_sqlite_stmt_select;
    sqlite3_stmt *_sqlite_stmt_updateCover;
    sqlite3_stmt *_sqlite_stmt_updateTitle;
    sqlite3_stmt *_sqlite_stmt_updateSubtitle;
};

#endif /* defined(__HelloCpp__DMBriefInfoNanager__) */
