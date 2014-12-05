//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMBriefInfoNanager__
#define __HelloCpp__DMBriefInfoNanager__

#include <iostream>
#include "CartoonHouse.h"


struct DMDownloadInfoManager
{
    DMDownloadInfoManager();
    
    virtual ~DMDownloadInfoManager();
    
    static DMDownloadInfoManager* sharedDMDownloadInfoManager();
    
    void _saved(const DMDownloadInfo& model);
    
    void _delete(const string& ID);
    
    DMDownloadInfo _select(const string& ID);
    
    void _update(const DMDownloadInfo& model);

protected:
    
    sqlite3 *_sqlite3;
    
    sqlite3_stmt *_sqlite_stmt_insert;
    sqlite3_stmt *_sqlite_stmt_remove;
    sqlite3_stmt *_sqlite_stmt_select;
    sqlite3_stmt *_sqlite_stmt_update;
};

#endif /* defined(__HelloCpp__DMBriefInfoNanager__) */
