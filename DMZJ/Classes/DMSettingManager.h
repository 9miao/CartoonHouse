//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMSettingManager__
#define __HelloCpp__DMSettingManager__

#include <iostream>
#include "CartoonHouse.h"

#define SETTINGS_COUNT 3

typedef enum{
    Off=0,
    On=1
}SwitchStatus;

typedef enum {
    Horizontal=0,
    Vertical=1
}DisplayModel;


typedef enum
{
    //推荐页的时间
    RefreshTagRecommandPage,
    //分类的时间
    RefreshTagClassify,
    //详情的时间
    RefreshTagIntroduction
    
}RefreshTag;

static const char* settings[SETTINGS_COUNT] =
{
    //移动网络观看（SwitchStatus）
    "set_online_watch",
    //移动网络下载（SwitchStatus）
    "set_download_watch",
    //显示模式（DisplayModel）
    "set_display_model"
};

class DMSettingManager
{
public:
    sqlite3_stmt *_sqlite_stmt_setOlinewatch;
    sqlite3_stmt *_sqlite_stmt_getSetOlineWatch;
    sqlite3_stmt *_sqlite_stmt_setSetDownLoadWatch;
    sqlite3_stmt *_sqlite_stmt_getSetDownLoadWatch;
    sqlite3_stmt *_sqlite_stmt_setSetDisplayModel;
    sqlite3_stmt *_sqlite_stmt_getSetDisplayModel;
    sqlite3_stmt *_sqlite_stmt_isSettingTableExists;
    
    sqlite3_stmt *_sqlite_stmt_setRefreshTime;
    sqlite3_stmt *_sqlite_stmt_getRefreshTime;
    sqlite3_stmt *_sqlite_stmt_delRefreshTime;
    
public:
    DMSettingManager();
    virtual ~DMSettingManager();
    
    void insertDefulRecord();
    
    void setSetOnLineWatch(int value);
    int getSetOnLineWatch();
    
    void setSetDownLoadWatch(int value);
    int getSetDownLoadWatch();
    
    void setSetDisplayModel(int value);
    int getSetDisplayModel();
    
    int isSettingTableExists();
    
    void setRefreshTime(int tag);
    
    std::string getRefreshTime(int tag);
    
    void delRefreshTime(int tag);
    
    
    
};



#endif /* defined(__HelloCpp__DMSettingManager__) */
