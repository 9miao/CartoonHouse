//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMSearchHistoryManager__
#define __CartoonHouse__DMSearchHistoryManager__

#include <iostream>
#include "CartoonHouse.h"

class DMModelSearchHistory
{
public:
    
    DMModelSearchHistory(int _id ,const std::string &_keyword):_id(_id),_keyword(_keyword)
    {
    }
    DMModelSearchHistory():_id(-1),_keyword("")
    {
    }
    
    int _id;
    std::string _keyword;

    
};


class DMSearchHistoryManager
{
    
public:
    
    DMSearchHistoryManager();
    
    ~DMSearchHistoryManager();
    
    void save(const DMModelSearchHistory& history);
    
    vector<DMModelSearchHistory> getRecent(int count = 10);
    
    void clearHistory();
    
    void delteKey(std::string keyword);
    
    
private:
    
    sqlite3 *_sqlite3;
    
    sqlite3_stmt *_sqlite_stmt_save;
    sqlite3_stmt *_sqlite_stmt_getrecent;
    sqlite3_stmt *_sqlite_stmt_clear;
    sqlite3_stmt *_sqlite_stmt_delete;
    
};

#endif /* defined(__CartoonHouse__DMSearchHistoryManager__) */
