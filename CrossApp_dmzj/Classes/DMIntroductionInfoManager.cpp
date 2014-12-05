//
//  DMIntroductionInfoManager.cpp
//  CartoonHouse
//
//  Created by liuguoyan on 14-8-25.
//
//

#include "DMIntroductionInfoManager.h"

static DMIntroductionInfoManager* _infoManager = NULL;

DMIntroductionInfoManager* DMIntroductionInfoManager::sharedDMIntroductionInfoManager()
{
    if (_infoManager == NULL)
    {
        _infoManager = new DMIntroductionInfoManager();
    }
    return _infoManager;
}

DMIntroductionInfoManager::DMIntroductionInfoManager()
{
    
    int ret = 0;
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "brief.db";
    
    ret = sqlite3_open(fullPath.c_str(), &_sqlite3);
    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS dm_info (id INTEGER PRIMARY KEY AUTOINCREMENT, dmid varchar(20),title varchar(256), subtitle varchar(256), types varchar(128), zone varchar(256), status varchar(256), last_update_chapter_name varchar(256), last_updatetime varchar(256), cover varchar(256),authors varchar(256), description varchar(256), first_letter varchar(256), direction varchar(256), time varchar(128), readprogress varchar(16), readIsList varchar(16), readChapTitle varchar(64));";
    
	sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(_sqlite3, sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE dm_info TABLE\n");
    
    const char *sql_save = "INSERT INTO dm_info (dmid ,title,subtitle,types,zone,status,last_update_chapter_name,last_updatetime,cover,authors,description,first_letter,direction,time ,readprogress ,readIsList) VALUES ( ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? ,'0','');";
    
    ret |= sqlite3_prepare_v2(_sqlite3, sql_save, -1, &_sqlite_stmt_save, NULL);
    
    //GET RECENT 10 HISTORY
    const char *sql_getrecent = "SELECT dmid, title, subtitle, types, zone, status, last_update_chapter_name, last_updatetime, cover, authors, description, first_letter, direction, time, readprogress, readIsList FROM dm_info ORDER BY id DESC ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_getrecent, -1, &_sqlite_stmt_getrecent, NULL);
    
    //CLEAR
    const char *sql_clear = "DELETE FROM dm_info ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_clear, -1, &_sqlite_stmt_clear, NULL);
    
    //DELETE
    const char *sql_del = "DELETE FROM dm_info WHERE dmid = ? ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_del, -1, &_sqlite_stmt_delete, NULL);

    //UPDATE progress and id
    const char *sql_updateprogressandid = "UPDATE dm_info SET readprogress = ?, readIsList = ?, readChapTitle = ?, WHERE dmid = ? ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_updateprogressandid, -1, &_sqlite_update_readprogressandID, NULL);
    
    //SELECT
    const char *sql_select_byid = "SELECT dmid ,title, subtitle, types, zone, status, last_update_chapter_name, last_updatetime, cover, authors, description, first_letter, direction, time, readprogress, readIsList, readChapTitle, FROM dm_info WHERE dmid = ? limit 1; ";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_select_byid, -1, &_sqlite_select_bookbyid, NULL);
    
}

DMIntroductionInfoManager::~DMIntroductionInfoManager()
{
    sqlite3_finalize(_sqlite_stmt_save);
    sqlite3_finalize(_sqlite_stmt_getrecent);
    sqlite3_finalize(_sqlite_stmt_clear);
    sqlite3_finalize(_sqlite_stmt_delete);
    sqlite3_finalize(_sqlite_update_readprogressandID);
    sqlite3_finalize(_sqlite_select_bookbyid);
    sqlite3_close(_sqlite3);
}

void DMIntroductionInfoManager::save(const DMBrowseInfo& introlinfo)
{
    
    deleteDmId(introlinfo.ID);
    
    int ok = sqlite3_bind_text(_sqlite_stmt_save, 1, introlinfo.ID.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 2, introlinfo.title.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 3, introlinfo.subtitle.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 4, introlinfo.types.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 5, introlinfo.zone.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 6, introlinfo.status.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 7, introlinfo.last_update_chapter_name.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 8, introlinfo.last_updatetime.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 9, introlinfo.cover.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 10, introlinfo.authors.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 11, introlinfo.description.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 12, introlinfo.first_letter.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 13, introlinfo.direction.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_save, 14, introlinfo.time.c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_save);
    ok |= sqlite3_reset(_sqlite_stmt_save);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMIntroductionInfoManager::save");
    
}

vector<DMBrowseInfo> DMIntroductionInfoManager::getRecent()
{
    int ok =SQLITE_OK;
    vector<DMBrowseInfo> infos;
    while (sqlite3_step(_sqlite_stmt_getrecent)== SQLITE_ROW) {
        DMBrowseInfo model;
        const unsigned char *ID = sqlite3_column_text(_sqlite_stmt_getrecent, 0);
        const unsigned char *title=sqlite3_column_text(_sqlite_stmt_getrecent, 1);
        const unsigned char *subtitle=sqlite3_column_text(_sqlite_stmt_getrecent, 2);
        const unsigned char *types=sqlite3_column_text(_sqlite_stmt_getrecent, 3);
        const unsigned char *zone=sqlite3_column_text(_sqlite_stmt_getrecent, 4);
        const unsigned char *status=sqlite3_column_text(_sqlite_stmt_getrecent, 5);
        const unsigned char *last_update_chapter_name=sqlite3_column_text(_sqlite_stmt_getrecent, 6);
        const unsigned char *last_updatetime=sqlite3_column_text(_sqlite_stmt_getrecent, 7);
        const unsigned char *cover=sqlite3_column_text(_sqlite_stmt_getrecent, 8);
        const unsigned char *authors=sqlite3_column_text(_sqlite_stmt_getrecent, 9);
        const unsigned char *description=sqlite3_column_text(_sqlite_stmt_getrecent, 10);
        const unsigned char *first_letter=sqlite3_column_text(_sqlite_stmt_getrecent, 11);
        const unsigned char *direction=sqlite3_column_text(_sqlite_stmt_getrecent, 12);
        const unsigned char *time=sqlite3_column_text(_sqlite_stmt_getrecent, 13);
        int readprogress = sqlite3_column_int(_sqlite_stmt_getrecent, 14);
        bool isList = sqlite3_column_int(_sqlite_stmt_getrecent, 15);
        //const unsigned char *chapTitle=sqlite3_column_text(_sqlite_stmt_getrecent, 16);
        model.ID = (const char *)ID;
        model.title =(const char *)title;
        model.subtitle =(const char *)subtitle;
        model.types =(const char *)types;
        model.zone =(const char *)zone;
        model.status =(const char *)status;
        model.last_update_chapter_name =(const char *)last_update_chapter_name;
        model.last_updatetime =(const char *)last_updatetime;
        model.cover =(const char *)cover;
        model.authors =(const char *)authors;
        model.description =(const char *)description;
        model.first_letter =(const char *)first_letter;
        model.direction =(const char *)direction;
        model.time =(const char *)time;
        model.readIndex = readprogress;
        model.isList = isList;
        //model.readChapTitle = (const char *)chapTitle;
        infos.push_back(model);
    }
    
    ok |= sqlite3_reset(_sqlite_stmt_getrecent);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMIntroductionInfoManager::getRecent");
    
    return infos;
}

void DMIntroductionInfoManager::clearHistory()
{
    
    int ok = sqlite3_step(_sqlite_stmt_clear);
    ok |= sqlite3_reset(_sqlite_stmt_clear);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMIntroductionInfoManager::clearHistory");
    
}

void DMIntroductionInfoManager::deleteDmId(const std::string &id)
{
    
    int ok = sqlite3_bind_text(_sqlite_stmt_delete, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    ok = sqlite3_step(_sqlite_stmt_delete);
    ok |= sqlite3_reset(_sqlite_stmt_delete);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMIntroductionInfoManager::deleteDmId");
    
}

void DMIntroductionInfoManager::updateProgress(int charIndex, bool isList, std::string readChapTitle, const std::string& comic_id)
{
    int ok = sqlite3_bind_text(_sqlite_update_readprogressandID, 4, comic_id.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_update_readprogressandID, 3, readChapTitle.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_int(_sqlite_update_readprogressandID, 2, (int)isList);
    ok |= sqlite3_bind_int(_sqlite_update_readprogressandID, 1, charIndex);
    
    ok |= sqlite3_step(_sqlite_update_readprogressandID);
    ok |= sqlite3_reset(_sqlite_update_readprogressandID);
    
    CCLog("DMIntroductionInfoManager::updateProgress");
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMIntroductionInfoManager::updateProgress");
    
}

DMBrowseInfo DMIntroductionInfoManager::selectById(std::string id)
{
    int ok =SQLITE_OK;
    
    ok = sqlite3_bind_text(_sqlite_select_bookbyid, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    
    DMBrowseInfo model;
    if (sqlite3_step(_sqlite_select_bookbyid)== SQLITE_ROW) {
        const unsigned char *ID = sqlite3_column_text(_sqlite_select_bookbyid, 0);
        const unsigned char *title=sqlite3_column_text(_sqlite_select_bookbyid, 1);
        const unsigned char *subtitle=sqlite3_column_text(_sqlite_select_bookbyid, 2);
        const unsigned char *types=sqlite3_column_text(_sqlite_select_bookbyid, 3);
        const unsigned char *zone=sqlite3_column_text(_sqlite_select_bookbyid, 4);
        const unsigned char *status=sqlite3_column_text(_sqlite_select_bookbyid, 5);
        const unsigned char *last_update_chapter_name=sqlite3_column_text(_sqlite_select_bookbyid, 6);
        const unsigned char *last_updatetime=sqlite3_column_text(_sqlite_select_bookbyid, 7);
        const unsigned char *cover=sqlite3_column_text(_sqlite_select_bookbyid, 8);
        const unsigned char *authors=sqlite3_column_text(_sqlite_select_bookbyid, 9);
        const unsigned char *description=sqlite3_column_text(_sqlite_select_bookbyid, 10);
        const unsigned char *first_letter=sqlite3_column_text(_sqlite_select_bookbyid, 11);
        const unsigned char *direction=sqlite3_column_text(_sqlite_select_bookbyid, 12);
        const unsigned char *time=sqlite3_column_text(_sqlite_select_bookbyid, 13);
        int readprogress = sqlite3_column_int(_sqlite_select_bookbyid, 14);
        bool isList = sqlite3_column_int(_sqlite_select_bookbyid, 15);
        const unsigned char *chapTitle=sqlite3_column_text(_sqlite_stmt_getrecent, 16);
        model.ID = (const char *)ID;
        model.title =(const char *)title;
        model.subtitle =(const char *)subtitle;
        model.types =(const char *)types;
        model.zone =(const char *)zone;
        model.status =(const char *)status;
        model.last_update_chapter_name =(const char *)last_update_chapter_name;
        model.last_updatetime =(const char *)last_updatetime;
        model.cover =(const char *)cover;
        model.authors =(const char *)authors;
        model.description =(const char *)description;
        model.first_letter =(const char *)first_letter;
        model.direction =(const char *)direction;
        model.time =(const char *)time;
        model.readIndex = readprogress;
        model.isList = isList;
        model.readChapTitle = (const char*)chapTitle;
    }
    
    ok |= sqlite3_reset(_sqlite_select_bookbyid);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMIntroductionInfoManager::selectById");
    
    return model;
}



