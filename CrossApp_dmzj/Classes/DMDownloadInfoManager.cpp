

#include "DMDownloadInfoManager.h"
#include <stdio.h>
#include <stdlib.h>


static DMDownloadInfoManager* _____manager = NULL;

DMDownloadInfoManager* DMDownloadInfoManager::sharedDMDownloadInfoManager()
{
    if (!_____manager)
    {
        _____manager = new DMDownloadInfoManager();
    }
    return _____manager;
}

DMDownloadInfoManager::DMDownloadInfoManager()
{
    int ret = 0;
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "brief.db";
    
    ret = sqlite3_open(fullPath.c_str(), &_sqlite3);
    
    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS downloadInfo(bookID VARCHAR(64) PRIMARY KEY, bookTitle VARCHAR(256), imageUrl VARCHAR(256), alreadyCache VARCHAR(128), notCache VARCHAR(128), sizeExplain VARCHAR(128));";
	sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(_sqlite3, sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
    {
		CCLog("Error in CREATE downloadInfo\n");
    }
    
    
    // INSERT
    const char *sql_insert = "INSERT INTO downloadInfo (bookID, bookTitle, imageUrl, alreadyCache, notCache, sizeExplain) VALUES (?,?,?,?,?,?);";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_insert, -1, &_sqlite_stmt_insert, NULL);
    
    // DELETE
    const char *sql_remove = "DELETE FROM downloadInfo WHERE bookID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_remove, -1, &_sqlite_stmt_remove, NULL);
    
    // SELECT
    const char *sql_select = "SELECT bookTitle, imageUrl, alreadyCache, notCache, sizeExplain FROM downloadInfo WHERE bookID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_select, -1, &_sqlite_stmt_select, NULL);
    
    // UPDATE
    const char *sql_update = "UPDATE downloadInfo set alreadyCache=?, notCache=?, sizeExplain=? where bookID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_update, -1, &_sqlite_stmt_update, NULL);
}

DMDownloadInfoManager::~DMDownloadInfoManager()
{
    sqlite3_finalize(_sqlite_stmt_insert);
    sqlite3_finalize(_sqlite_stmt_remove);
    sqlite3_finalize(_sqlite_stmt_select);
    sqlite3_finalize(_sqlite_stmt_update);
    sqlite3_close(_sqlite3);
}

void DMDownloadInfoManager::_saved(const DMDownloadInfo& model)
{
    _delete(model.bookID);
    
    int ok  = sqlite3_bind_text(_sqlite_stmt_insert, 1,       model.bookID.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 2,    model.bookTitle.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 3,     model.imageUrl.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 4, model.alreadyCache.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 5,     model.notCache.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 6,  model.sizeExplain.c_str(), -1, SQLITE_TRANSIENT);
    
	ok |= sqlite3_step(_sqlite_stmt_insert);

	ok |= sqlite3_reset(_sqlite_stmt_insert);

	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMDownloadInfoManager._saved()\n");
    else
        CCLog("SAVED DOWNLOADINFO OK");
}

void DMDownloadInfoManager::_delete(const string& ID)
{
    int ok = sqlite3_bind_text(_sqlite_stmt_remove, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_remove);
	
	ok |= sqlite3_reset(_sqlite_stmt_remove);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMDownloadInfoManager._delete()\n");
}

DMDownloadInfo DMDownloadInfoManager::_select(const string& ID)
{
    
    int ok = sqlite3_reset(_sqlite_stmt_select);
    
	ok |= sqlite3_bind_text(_sqlite_stmt_select, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
	ok |= sqlite3_step(_sqlite_stmt_select);
	const unsigned char *bookTitle = sqlite3_column_text(_sqlite_stmt_select, 0);
	const unsigned char *imageUrl = sqlite3_column_text(_sqlite_stmt_select, 1);
    const unsigned char *alreadyCache = sqlite3_column_text(_sqlite_stmt_select, 2);
    const unsigned char *notCache = sqlite3_column_text(_sqlite_stmt_select, 3);
    const unsigned char *sizeExplain = sqlite3_column_text(_sqlite_stmt_select, 4);
    
    DMDownloadInfo info;
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
    {
		printf("Error in DMDownloadInfoManager._select()\n");
        return info;
    }
    
    
    info.bookID = ID;
    info.bookTitle = (const char*)bookTitle;
    info.imageUrl = (const char*)imageUrl;
    info.alreadyCache = (const char*)alreadyCache;
    info.notCache = (const char*)notCache;
    info.sizeExplain = (const char*)sizeExplain;
    
	return info;
}

void DMDownloadInfoManager::_update(const DMDownloadInfo& model)
{
    int ok  = sqlite3_bind_text(_sqlite_stmt_update, 1, model.alreadyCache.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_update, 2, model.notCache.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_update, 3, model.sizeExplain.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_update, 4, model.bookID.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_step(_sqlite_stmt_update);
	
    ok |= sqlite3_reset(_sqlite_stmt_update);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMDownloadInfoManager._update_alreadyCache()\n");
}
