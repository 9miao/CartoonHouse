//
//  DMSettingManager.cpp
//  HelloCpp
//
//  Created by liuguoyan on 14-8-8.
//
//

#include "DMSettingManager.h"

DMSettingManager::DMSettingManager()
{
    
    //===============================================================
    //系统设置记录
    //===============================================================
    
    const char *sql_createtable = "create table if not exists settings(id integer , set_online_watch int ,set_download_watch int,set_display_model int);";
    
    sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
    insertDefulRecord();
    
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE settings TABLE\n");
    
    //
    const char *sql_setOlinewatch = "UPDATE settings SET set_online_watch = ? WHERE id = 1 ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_setOlinewatch, -1, &_sqlite_stmt_setOlinewatch, NULL);

    
    //
    const char *sql_getSetOlineWatch = "SELECT set_online_watch FROM settings WHERE ID= 1 ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_getSetOlineWatch, -1, &_sqlite_stmt_getSetOlineWatch, NULL);
    
    //
    const char *sql_setSetDownLoadWatch = "UPDATE settings SET set_download_watch = ? WHERE id = 1 ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_setSetDownLoadWatch, -1, &_sqlite_stmt_setSetDownLoadWatch, NULL);
    
    //
    const char *sql_getSetDownLoadWatch = "SELECT set_download_watch FROM settings WHERE ID= 1 ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_getSetDownLoadWatch, -1, &_sqlite_stmt_getSetDownLoadWatch, NULL);
    
    //
    const char *sql_setSetDisplayModel = "UPDATE settings SET set_display_model = ? WHERE id = 1 ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_setSetDisplayModel, -1, &_sqlite_stmt_setSetDisplayModel, NULL);
    
    //
    const char *sql_getSetDisplayModel = "SELECT set_display_model FROM settings WHERE ID= 1 ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_getSetDisplayModel, -1, &_sqlite_stmt_getSetDisplayModel, NULL);
    
    //
    const char *sql_isSettingTableExists = "SELECT  count(*)   FROM settings;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_isSettingTableExists, -1, &_sqlite_stmt_isSettingTableExists, NULL);
    
    //===============================================================
    //下拉刷新记录
    //===============================================================
    
    const char *sql_refreshrecord = "create table if not exists refresh_record (tag integer , time varchar(20));";
    
    sqlite3_stmt *stmt_refresh;
	ok=sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_refreshrecord, -1, &stmt_refresh, NULL);
	ok |= sqlite3_step(stmt_refresh);
	ok |= sqlite3_finalize(stmt_refresh);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE refresh_record TABLE\n");
    
    
    const char *sql_setRefresh = "insert into refresh_record (tag , time ) values ( ? , ? ) ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_setRefresh, -1, &_sqlite_stmt_setRefreshTime, NULL);
    
    const char *sql_getRefresh = "select time from refresh_record where tag = ? ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_getRefresh, -1, &_sqlite_stmt_getRefreshTime, NULL);
    
    const char *sql_delRefresh = "delete from refresh_record where tag = ? ;";
    sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_delRefresh, -1, &_sqlite_stmt_delRefreshTime, NULL);
    
}


void DMSettingManager::insertDefulRecord(){
    
    
    const char *sql_select = "select count(*) from settings;";
    const char *sql_insert = "insert into settings(id, set_online_watch, set_download_watch, set_display_model) values(1,0,0,0);";
    
    sqlite3_stmt *stmt;
    sqlite3_stmt *stmt_insert;
	int ok=sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_select, -1, &stmt, NULL);
    ok |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_insert, -1, &stmt_insert, NULL);
	ok |= sqlite3_step(stmt);
    int val = sqlite3_column_int(stmt , 0);
    if (val==0) {
        ok|=sqlite3_step(stmt_insert);
        ok |= sqlite3_finalize(stmt_insert);
    }
	ok |= sqlite3_finalize(stmt);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE settings TABLE\n");
    
}


DMSettingManager:: ~DMSettingManager()
{
    sqlite3_finalize(_sqlite_stmt_setOlinewatch);
    sqlite3_finalize(_sqlite_stmt_getSetOlineWatch);
    sqlite3_finalize(_sqlite_stmt_setSetDownLoadWatch);
    sqlite3_finalize(_sqlite_stmt_getSetDownLoadWatch);
    sqlite3_finalize(_sqlite_stmt_setSetDisplayModel);
    sqlite3_finalize(_sqlite_stmt_getSetDisplayModel);
    sqlite3_finalize(_sqlite_stmt_isSettingTableExists);
    sqlite3_finalize(_sqlite_stmt_setRefreshTime);
    sqlite3_finalize(_sqlite_stmt_getRefreshTime);
    sqlite3_finalize(_sqlite_stmt_delRefreshTime);
}

//===============================================================
//系统设置记录
//===============================================================

void DMSettingManager::setSetOnLineWatch(int value)
{
    char val [10];
    
    sprintf(val, "%d" ,value);
    
    const char* v = val;
   
    int ok = sqlite3_bind_text(_sqlite_stmt_setOlinewatch, 1, v, -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_setOlinewatch);
	
	ok |= sqlite3_reset(_sqlite_stmt_setOlinewatch);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMSettingManager.setSetOnLineWatch()\n");
    
}
int DMSettingManager::getSetOnLineWatch()
{
    
	int ok = sqlite3_step(_sqlite_stmt_getSetOlineWatch);
    
    int val = sqlite3_column_int(_sqlite_stmt_getSetOlineWatch , 0);
    
    ok |= sqlite3_reset(_sqlite_stmt_getSetOlineWatch);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		printf("Error in DMSettingManager.getSetOnLineWatch()\n");
    
    return val;
    
}

void DMSettingManager::setSetDownLoadWatch(int value)
{
    char val [10];
    
    sprintf(val, "%d" ,value);
    
    const char* v = val;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_setSetDownLoadWatch, 1, v, -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_setSetDownLoadWatch);
	
	ok |= sqlite3_reset(_sqlite_stmt_setSetDownLoadWatch);
    
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMSettingManager.setSetDownLoadWatch()\n");
}
int DMSettingManager::getSetDownLoadWatch()
{
    int ok = sqlite3_reset(_sqlite_stmt_getSetDownLoadWatch);
    
	ok = sqlite3_step(_sqlite_stmt_getSetDownLoadWatch);
    
    int val = sqlite3_column_int(_sqlite_stmt_getSetDownLoadWatch , 0);
    
    ok |= sqlite3_reset(_sqlite_stmt_getSetDownLoadWatch);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		printf("Error in DMSettingManager.getSetDownLoadWatch()\n");
    
    return val;
}

void DMSettingManager::setSetDisplayModel(int value)
{
    
    char val [10];
    
    sprintf(val, "%d" ,value);
    
    const char* v = val;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_setSetDisplayModel, 1, v, -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_setSetDisplayModel);
	
	ok |= sqlite3_reset(_sqlite_stmt_setSetDisplayModel);
    
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMSettingManager.setSetDisplayModel()\n");
    
}
int DMSettingManager::getSetDisplayModel()
{
    
	int ok = sqlite3_step(_sqlite_stmt_getSetDisplayModel);
    
    int val = sqlite3_column_int(_sqlite_stmt_getSetDisplayModel , 0);
    
    ok |= sqlite3_reset(_sqlite_stmt_getSetDisplayModel);
    
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		printf("Error in DMSettingManager.getSetDisplayModel()\n");
    
    return val;
}


int DMSettingManager::isSettingTableExists()
{
    
	int ok = sqlite3_step(_sqlite_stmt_isSettingTableExists);
    
    int val = sqlite3_column_int(_sqlite_stmt_isSettingTableExists , 0);
    
    ok |= sqlite3_reset(_sqlite_stmt_isSettingTableExists);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		printf("Error in DMSettingManager.isSettingTableExists()\n");
    
    return val;
}


//===============================================================
//下拉刷新记录
//===============================================================

static std::string getCurrentTime(){
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime (&rawtime);
    char time[32];
    
    sprintf(time, "%02d-%02d %02d:%02d",
            timeinfo->tm_mon+1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min);
    return time;
}


void DMSettingManager::setRefreshTime(int tag)
{
    
    delRefreshTime(tag);
    
    std::string timestr= getCurrentTime();
    
    char val [10];
    
    sprintf(val, "%d" ,tag);
    
    const char* v = val;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_setRefreshTime, 1, v, -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_bind_text(_sqlite_stmt_setRefreshTime, 2, timestr.c_str(), -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_setRefreshTime);
	
	ok |= sqlite3_reset(_sqlite_stmt_setRefreshTime);
    
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMSettingManager.setRefreshTime()\n");
}

std::string DMSettingManager::getRefreshTime(int tag)
{
    char val [10];
    
    sprintf(val, "%d" ,tag);
    
    const char* v = val;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_getRefreshTime, 1, v, -1, SQLITE_TRANSIENT);
    
    ok|= sqlite3_step(_sqlite_stmt_getRefreshTime);
    
    std::string time = (const char *)sqlite3_column_text(_sqlite_stmt_getRefreshTime, 0);
    
    ok |= sqlite3_reset(_sqlite_stmt_getRefreshTime);
    
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		printf("Error in DMSettingManager.getRefreshTime()\n");
    
    time = time.length()==0 ? getCurrentTime(): time;
    
    return time;
}

void DMSettingManager::delRefreshTime(int tag)
{
    
    char val [10];
    
    sprintf(val, "%d" ,tag);
    
    const char* v = val;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_delRefreshTime, 1, v, -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_delRefreshTime);
	
	ok |= sqlite3_reset(_sqlite_stmt_delRefreshTime);
    
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMSettingManager.delRefreshTime()\n");
}




