//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __CartoonHouse__DMIntroductionInfoManager__
#define __CartoonHouse__DMIntroductionInfoManager__

#include <iostream>
#include "CartoonHouse.h"

struct DMBrowseInfo
{
    DMBrowseInfo()
    :readIndex(-1)
    ,isList(1)
    {
    
    }
    
    std::string ID;
    
    std::string title;
    
    std::string subtitle;
    
    std::string types;
    
    std::string zone;
    
    std::string status;
    
    std::string last_update_chapter_name;
    
    std::string last_updatetime;
    
    std::string cover;
    
    std::string authors;
    
    std::string description;
    
    std::string first_letter;
    
    std::string direction;
    
    std::string time;
    
    int readIndex;
    
    bool isList;
    
    std::string readChapTitle;
};

class DMIntroductionInfoManager
{
public:
    
    static DMIntroductionInfoManager* sharedDMIntroductionInfoManager();
    
    DMIntroductionInfoManager();
    
    ~DMIntroductionInfoManager();
    
    void save(const DMBrowseInfo& introlinfo);
    
    vector<DMBrowseInfo> getRecent();
    
    void clearHistory();
    
    void deleteDmId(const std::string &id);
    
    void updateProgress(int charIndex, bool isList, std::string readChapTitle, const std::string& comic_id);
    
    DMBrowseInfo selectById(std::string id);
    
private:
    
    sqlite3 *_sqlite3;
    
    sqlite3_stmt *_sqlite_stmt_save;
    sqlite3_stmt *_sqlite_stmt_getrecent;
    sqlite3_stmt *_sqlite_stmt_clear;
    sqlite3_stmt *_sqlite_stmt_delete;
    sqlite3_stmt *_sqlite_update_readprogressandID;
    sqlite3_stmt *_sqlite_select_bookbyid;
    
};

static std::string getCurrentTime(){

    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime (&rawtime);
    char time[32];
    /**
     sprintf(time, "(%04d/%02d/%02d %02d:%02d:%02d)",
     timeinfo->tm_year+1900,
     timeinfo->tm_mon,
     timeinfo->tm_mday,
     timeinfo->tm_hour,
     timeinfo->tm_min,
     timeinfo->tm_sec);
     **/
    
    sprintf(time, "(%02d-%02d %02d:%02d)",
            timeinfo->tm_mon+1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min);
//    printf("c_time = %s",time);
    
    return time;
}


#endif /* defined(__CartoonHouse__DMIntroductionInfoManager__) */
