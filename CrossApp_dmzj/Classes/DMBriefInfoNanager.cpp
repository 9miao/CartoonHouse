

#include "DMBriefInfoNanager.h"
#include <stdio.h>
#include <stdlib.h>

DMBriefInfoManager::DMBriefInfoManager()
{
    int ret = 0;
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "brief.db";
    
    ret = sqlite3_open(fullPath.c_str(), &_sqlite3);
    
    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS data(ID VARCHAR(16) PRIMARY KEY, cover VARCHAR(256), title VARCHAR(128), subtitle VARCHAR(128));";
	sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(_sqlite3, sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE TABLE\n");
    
    
    // INSERT
    const char *sql_insert = "INSERT INTO data (ID, cover, title, subtitle) VALUES (?,?,?,?);";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_insert, -1, &_sqlite_stmt_insert, NULL);
    
    // DELETE
    const char *sql_remove = "DELETE FROM data WHERE ID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_remove, -1, &_sqlite_stmt_remove, NULL);
    
    // SELECT
    const char *sql_select = "SELECT cover, title, subtitle FROM data WHERE ID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_select, -1, &_sqlite_stmt_select, NULL);
    
    // REPLACE Cover
    const char *sql_updateCover = "UPDATE data set cover=? where ID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_updateCover, -1, &_sqlite_stmt_updateCover, NULL);
    
    // REPLACE title
    const char *sql_updateTitle = "UPDATE data set title=? where ID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_updateTitle, -1, &_sqlite_stmt_updateTitle, NULL);
    
    // REPLACE subtitle
    const char *sql_updateSubtitle = "UPDATE data set subtitle=? where ID=?;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_updateSubtitle, -1, &_sqlite_stmt_updateSubtitle, NULL);
}

DMBriefInfoManager::~DMBriefInfoManager()
{
    sqlite3_finalize(_sqlite_stmt_insert);
    sqlite3_finalize(_sqlite_stmt_remove);
    sqlite3_finalize(_sqlite_stmt_select);
    sqlite3_finalize(_sqlite_stmt_updateCover);
    sqlite3_finalize(_sqlite_stmt_updateTitle);
    sqlite3_finalize(_sqlite_stmt_updateSubtitle);
    sqlite3_close(_sqlite3);
}

void DMBriefInfoManager::_saved(const DMBriefInfo& model)
{
    int ok  = sqlite3_bind_text(_sqlite_stmt_insert, 1,       model.ID.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 2,    model.cover.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 3,    model.title.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_insert, 4, model.subtitle.c_str(), -1, SQLITE_TRANSIENT);

	ok |= sqlite3_step(_sqlite_stmt_insert);

	ok |= sqlite3_reset(_sqlite_stmt_insert);

	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMBriefInfoManager._saved()\n");
}

void DMBriefInfoManager::_delete(const string& ID)
{
    int ok = sqlite3_bind_text(_sqlite_stmt_remove, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_remove);
	
	ok |= sqlite3_reset(_sqlite_stmt_remove);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMBriefInfoManager._delete()\n");
}

DMBriefInfo DMBriefInfoManager::_select(const string& ID)
{
    int ok = sqlite3_reset(_sqlite_stmt_select);
    
	ok |= sqlite3_bind_text(_sqlite_stmt_select, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
	ok |= sqlite3_step(_sqlite_stmt_select);
	const unsigned char *cover = sqlite3_column_text(_sqlite_stmt_select, 0);
	const unsigned char *title = sqlite3_column_text(_sqlite_stmt_select, 1);
    const unsigned char *subtitle = sqlite3_column_text(_sqlite_stmt_select, 2);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE && ok != SQLITE_ROW)
		printf("Error in DMBriefInfoManager._select()\n");
    
	return DMBriefInfo(ID, (const char*)cover, (const char*)title, (const char*)subtitle);
}

void DMBriefInfoManager::_updateCover(const DMBriefInfo& model)
{
    int ok  = sqlite3_bind_text(_sqlite_stmt_updateCover, 1, model.cover.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_updateCover, 2, model.ID.c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_updateCover);
	
    ok |= sqlite3_reset(_sqlite_stmt_updateCover);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMBriefInfoManager._updateCover()\n");
}

void DMBriefInfoManager::_updateTitle(const DMBriefInfo& model)
{
    int ok  = sqlite3_bind_text(_sqlite_stmt_updateTitle, 1, model.title.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_updateTitle, 2, model.ID.c_str(), -1, SQLITE_TRANSIENT);
    
	ok |= sqlite3_step(_sqlite_stmt_updateTitle);
	
	ok |= sqlite3_reset(_sqlite_stmt_updateTitle);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMBriefInfoManager._updateTitle()\n");
}

void DMBriefInfoManager::_updateSubtitle(const DMBriefInfo& model)
{
    int ok  = sqlite3_bind_text(_sqlite_stmt_updateSubtitle, 1, model.subtitle.c_str(), -1, SQLITE_TRANSIENT);
        ok |= sqlite3_bind_text(_sqlite_stmt_updateSubtitle, 2, model.ID.c_str(), -1, SQLITE_TRANSIENT);
    
	ok |= sqlite3_step(_sqlite_stmt_updateSubtitle);
	
	ok |= sqlite3_reset(_sqlite_stmt_updateSubtitle);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMBriefInfoManager._updateSubtitle()\n");
}