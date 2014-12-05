//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMUserManager__
#define __CartoonHouse__DMUserManager__

#include <iostream>
#include "CartoonHouse.h"

typedef enum {
    offline=0,
    online
}UserStatus;

class DMUser
{
public:
    
    
    std::string uid;
    
    std::string nickname;
    
    std::string photo;
    
    int status;
    
};

class DMUserManager
{
public:
    
    DMUserManager();
    
    virtual ~DMUserManager();
    
    static DMUserManager* sharedDMUserManager();
    
    void _delete(const string& uid);
    
    //登陆
    bool userLogin(const DMUser &user);
    //退出
    bool userLogout();
    //当前登陆的用户
    DMUser onLineUser();
    
    
    
protected:

    sqlite3_stmt *_sqlite_stmt_delete;

    //登陆
    sqlite3_stmt *_sqlite_stmt_login;
    //退出
    sqlite3_stmt *_sqlite_stmt_logout;
    //查找登录状态的用户
    sqlite3_stmt *_sqlite_stmt_select_online;
    
    
    
};




#endif /* defined(__CartoonHouse__DMUserManager__) */
