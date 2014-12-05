//
//  DMColllectionManager.cpp
//  CartoonHouse
//
//  Created by liuguoyan on 14-8-27.
//
//
#include "DMColllectionManager.h"
#include "DMUserManager.h"

static DMColllectionManager* ___collectionManager = NULL;

DMColllectionManager* DMColllectionManager::sharedDMColllectionManager()
{
    if (___collectionManager == NULL)
    {
        ___collectionManager = new DMColllectionManager();
    }
    return ___collectionManager;
}

DMColllectionManager::DMColllectionManager()
{
    _sqlite3 = RootWindow::getInstance()->getSqlite3();
    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS collection(_id int auto_increment primary key, uid VARCHAR(50), dmid VARCHAR(256));";
	sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(_sqlite3, sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE collection TABLE\n");
    
    //COLLECT
    const char *sql_collect = "insert into collection ( uid , dmid ) values ( ? , ? ) ;";
    sqlite3_prepare_v2(_sqlite3, sql_collect, -1, &_sqlite_stmt_collect, NULL);
    
    //DECOLLECT
    const char *sql_decollect = "delete from collection where uid = ? and dmid = ? ;";
    sqlite3_prepare_v2(_sqlite3, sql_decollect, -1, &_sqlite_stmt_decollect, NULL);
    
    //ISCOLLECTED
    const char *sql_iscollected = "select count(*) from collection where uid = ? and dmid = ? ; ";
    sqlite3_prepare_v2(_sqlite3, sql_iscollected, -1, &_sqlite_stmt_isCollected, NULL);

    
}

DMColllectionManager::~DMColllectionManager()
{
    sqlite3_finalize(_sqlite_stmt_collect);
    sqlite3_finalize(_sqlite_stmt_decollect);
    sqlite3_finalize(_sqlite_stmt_isCollected);
    sqlite3_close(_sqlite3);
}

bool DMColllectionManager::collect(std::string dmid)
{
    DMUserManager* mgr = DMUserManager::sharedDMUserManager();
    DMUser user = mgr->onLineUser();
    string uid = user.uid;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_collect, 1, user.uid.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_collect, 2, dmid.c_str(), -1, SQLITE_TRANSIENT);

    ok |= sqlite3_step(_sqlite_stmt_collect);
    ok |= sqlite3_reset(_sqlite_stmt_collect);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        printf("Error in DMColllectionManager::collect");
        return false;
    }
    return true;
}

bool DMColllectionManager::deCollect(std::string dmid)
{
    DMUserManager* mgr = DMUserManager::sharedDMUserManager();
    DMUser user = mgr->onLineUser();
    string uid = user.uid;
    
    int ok = sqlite3_bind_text(_sqlite_stmt_decollect, 1, user.uid.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_decollect, 2, dmid.c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_decollect);
    ok |= sqlite3_reset(_sqlite_stmt_decollect);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        printf("Error in DMColllectionManager::deCollect");
        return false;
    }
    return true;
    
}

bool DMColllectionManager::isCollect(std::string dmid)
{
    DMUserManager* mgr = DMUserManager::sharedDMUserManager();
    DMUser user = mgr->onLineUser();
    string uid = user.uid;
    int ok = sqlite3_bind_text(_sqlite_stmt_isCollected, 1, user.uid.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_isCollected, 2, dmid.c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_isCollected);
    
    int count = sqlite3_column_int(_sqlite_stmt_isCollected, 0);
    
    ok |= sqlite3_reset(_sqlite_stmt_isCollected);
    
    if (count ==0) {
        return false;
    }
    return true;
}

