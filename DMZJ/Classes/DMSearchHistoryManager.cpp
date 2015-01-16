//
//  DMSearchHistoryManager.cpp
//  CartoonHouse
//
//  Created by liuguoyan on 14-8-14.
//
//

#include "DMSearchHistoryManager.h"


DMSearchHistoryManager::DMSearchHistoryManager()
{
    int ret = 0;
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "brief.db";
    
    ret = sqlite3_open(fullPath.c_str(), &_sqlite3);

    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS searchhistory(_id INTEGER PRIMARY KEY AUTOINCREMENT, keyword VARCHAR(256) );";
	sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(_sqlite3, sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE searchhistory TABLE\n");
    
    //SAVE  insert into searchhistory (keyword) values ('fdd');
    const char *sql_save = "INSERT INTO searchhistory (keyword) VALUES ( ? );";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_save, -1, &_sqlite_stmt_save, NULL);
    
    //GET RECENT 10 HISTORY
    const char *sql_getrecent = "SELECT _id , keyword FROM searchhistory ORDER BY _id DESC LIMIT ?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_getrecent, -1, &_sqlite_stmt_getrecent, NULL);
    
    //CLEAR
    const char *sql_clear = "DELETE FROM searchhistory ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_clear, -1, &_sqlite_stmt_clear, NULL);
    
    //DELETE
    const char *sql_delete = "DELETE FROM searchhistory where keyword = ? ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_delete, -1, &_sqlite_stmt_delete, NULL);
    
    
}

DMSearchHistoryManager::~DMSearchHistoryManager()
{
    sqlite3_finalize(_sqlite_stmt_save);
    sqlite3_finalize(_sqlite_stmt_getrecent);
    sqlite3_finalize(_sqlite_stmt_clear);
    sqlite3_finalize(_sqlite_stmt_delete);
    sqlite3_close(_sqlite3);
    
}

void DMSearchHistoryManager::save(const DMModelSearchHistory& history)
{
    
    int ok = sqlite3_bind_text(_sqlite_stmt_save, 1, history._keyword.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_step(_sqlite_stmt_save);
    ok |= sqlite3_reset(_sqlite_stmt_save);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
     printf("Error in DMSearchHistoryManager::save");
    
}

vector<DMModelSearchHistory> DMSearchHistoryManager::getRecent(int count)
{
    char val[5];
    sprintf(val, "%d" ,count);
    const char *cou = val;
    int ok = sqlite3_bind_text(_sqlite_stmt_getrecent, 1, cou, -1, SQLITE_TRANSIENT);
    vector<DMModelSearchHistory> historys;
    while (sqlite3_step(_sqlite_stmt_getrecent)== SQLITE_ROW) {
        int _id = sqlite3_column_int(_sqlite_stmt_getrecent, 0);
//        history->_keyword = *sqlite3_column_text(_sqlite_stmt_getrecent, 1);
        
//        const unsigned char *_nickname = sqlite3_column_text(_sqlite_stmt_getrecent, 1);
        const unsigned char *_keyword=sqlite3_column_text(_sqlite_stmt_getrecent, 1);
        historys.push_back(DMModelSearchHistory(_id,(const char*)_keyword));
    }
    
    ok |= sqlite3_reset(_sqlite_stmt_getrecent);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMSearchHistoryManager::getRecent");
    
    return historys;
    
}

void DMSearchHistoryManager::clearHistory()
{
    
    int ok = sqlite3_step(_sqlite_stmt_clear);
    ok |= sqlite3_reset(_sqlite_stmt_clear);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMSearchHistoryManager::clearHistory");
    
}

void DMSearchHistoryManager::delteKey(std::string keyword)
{
    int ok = sqlite3_bind_text(_sqlite_stmt_delete, 1, keyword.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_step(_sqlite_stmt_delete);
    ok |= sqlite3_reset(_sqlite_stmt_delete);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMSearchHistoryManager::delteKey");
    
}


