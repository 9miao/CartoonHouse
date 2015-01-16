//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMColllectionManager__
#define __CartoonHouse__DMColllectionManager__

//收藏管理类
#include <iostream>
#include "CartoonHouse.h"

class DMCollection
{
public:
    
    std::string uid;
    
    std::string dmid;
    
};

class DMColllectionManager
{
public:
    
    DMColllectionManager();
    
    virtual ~DMColllectionManager();
    
    static DMColllectionManager* sharedDMColllectionManager();
    
    bool collect(std::string dmid);
    
    bool deCollect(std::string dmid);
    
    bool isCollect(std::string dmid);
    
protected:
    
    sqlite3 *_sqlite3;
    //收藏
    sqlite3_stmt *_sqlite_stmt_collect;
    //取消收藏
    sqlite3_stmt *_sqlite_stmt_decollect;
    //是否收藏
    sqlite3_stmt *_sqlite_stmt_isCollected;

    
    
};

#endif /* defined(__CartoonHouse__DMColllectionManager__) */
