//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__PushMessageReceiver__
#define __CartoonHouse__PushMessageReceiver__

#include <iostream>
#include "CartoonHouse.h"

struct PushSetting{
    
    std::string appid;
    std::string userId;
    std::string channelId;
    std::string requestId;
    
};


class DMPushSettingManager
{
    public:
    sqlite3 *_sqlite3;
    
    sqlite3_stmt *_sqlite_stmt_saveBind;
    sqlite3_stmt *_sqlite_stmt_getBind;
    sqlite3_stmt *_sqlite_stmt_removeBind;
    
    public:
    DMPushSettingManager();
    virtual ~DMPushSettingManager();
    
    public:
    void saveBind(PushSetting &pushSetting);
    
    PushSetting getBind();
    
    void removeBind();
    
};




class PushMessageReceiver
:public CAObject
{
    
    public:
        void handleMessage(std::string title,std::string description,std::string msgstr);
    
        void onBind(std::string appid,std::string userId,std::string channelId,std::string requestId);
    
    public:
    void onSumitPushUserIdFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void bindUserOnLogin();
    
    void onUnBindUserFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void unBindUserOnLogout();
    
    void onBindUserFinish(const HttpResponseStatus& status, const CSJson::Value& json);
    
    
};




#endif /* defined(__CartoonHouse__PushMessageReceiver__) */
