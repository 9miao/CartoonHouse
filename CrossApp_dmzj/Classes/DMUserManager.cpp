//
//  DMUserManager.cpp
//  CartoonHouse
//
//  Created by liuguoyan on 14-8-9.
//
//

#include "DMUserManager.h"
#include "PushMessageReceiver.h"

static DMUserManager* ___userManager = NULL;

DMUserManager* DMUserManager::sharedDMUserManager()
{
    if (___userManager == NULL)
    {
        ___userManager = new DMUserManager();
    }
    return ___userManager;
}


DMUserManager::DMUserManager()
{
    
    int ret = 0;
    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS user(id int auto_increment, uid VARCHAR(50), nickname VARCHAR(256), photo VARCHAR(256) , status int);";
	sqlite3_stmt *stmt;
    int ok=sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in CREATE USER TABLE\n");
    
    //DELETE
    const char *sql_delete = "DELETE FROM user WHERE uid = ? ;";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_delete, -1, &_sqlite_stmt_delete, NULL);
    
    const char *sql_stmt_login = "INSERT INTO user (uid, nickname, photo,status) VALUES (?,?,?,?);";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_login, -1, &_sqlite_stmt_login, NULL);
    
    const char *sql_stmt_logout = "UPDATE user SET status = 0 WHERE uid = (SELECT uid FROM user WHERE status = 1 LIMIT 1);";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_logout, -1, &_sqlite_stmt_logout, NULL);
    
    const char* sql_stmt_select_online =" SELECT uid, nickname, photo ,status FROM user WHERE status = 1 LIMIT 1; ";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_select_online, -1, &_sqlite_stmt_select_online, NULL);
}

DMUserManager::~DMUserManager()
{

    sqlite3_finalize(_sqlite_stmt_delete);

    sqlite3_finalize(_sqlite_stmt_login);
    sqlite3_finalize(_sqlite_stmt_logout);
    sqlite3_finalize(_sqlite_stmt_select_online);
}


//登陆
bool DMUserManager::userLogin(const DMUser &user)
{
    
    _delete(user.uid);
    
    char val[10];
    sprintf(val, "%d" ,user.status);
    const char* stu = val;
    int ok = sqlite3_bind_text(_sqlite_stmt_login, 1, user.uid.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_login, 2, user.nickname.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_login, 3, user.photo.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_login, 4, stu, -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_login);
    ok |= sqlite3_reset(_sqlite_stmt_login);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        printf("Error in DMUserManager::userLogin");
        return false;
    }
//    //百度推送绑定用户
//    PushMessageReceiver receiver;
//    receiver.bindUserOnLogin();
    
    return true;
}

//退出
bool DMUserManager::userLogout()
{
    //百度推送解绑用户
//    PushMessageReceiver receiver;
//    receiver.unBindUserOnLogout();
    
    int ok = sqlite3_step(_sqlite_stmt_logout);
    ok |= sqlite3_reset(_sqlite_stmt_logout);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        printf("Error in DMUserManager::userLogout");
        return false;
    }
    return true;
}
//当前登陆的用户
DMUser DMUserManager::onLineUser()
{
    int ok = SQLITE_OK;
    DMUser user;
    if (sqlite3_step(_sqlite_stmt_select_online)== SQLITE_ROW){
        const unsigned char *uid = sqlite3_column_text(_sqlite_stmt_select_online, 0);
        const unsigned char *nickname=sqlite3_column_text(_sqlite_stmt_select_online, 1);
        const unsigned char *photo=sqlite3_column_text(_sqlite_stmt_select_online, 2);
        int status = sqlite3_column_int(_sqlite_stmt_select_online, 3);
        user.uid = (const char*)uid;
        user.nickname = (const char*)nickname;
        user.photo = (const char*)photo;
        user.status = status;
    }
    ok |= sqlite3_reset(_sqlite_stmt_select_online);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        printf("Error in DMUserManager::onLineUser");
    
    return user;
}


void DMUserManager::_delete(const string& uid)
{
    int ok = sqlite3_bind_text(_sqlite_stmt_delete, 1, uid.c_str(), -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_delete);
	
	ok |= sqlite3_reset(_sqlite_stmt_delete);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		printf("Error in DMUserManager::_delete\n");
}


