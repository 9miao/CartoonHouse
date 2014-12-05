//
//  PushMessageReceiver.cpp
//  CartoonHouse
//
//  Created by liuguoyan on 14-9-19.
//
//

#include "CartoonHouse.h"
#include "PushMessageReceiver.h"
#include "DMLoginController.h"
#include "DMUserManager.h"

void PushMessageReceiver::handleMessage(std::string title,std::string description,std::string msgstr)
{
    CCLog("title=%s,desc=%s,msg=%s",title.c_str(),description.c_str(),msgstr.c_str());
    
//    DMLoginController *controller = new DMLoginController();
//    controller->init();
//    RootWindow *window = RootWindow::getInstance();
//    window->presentModalViewController(controller, true);
//    controller->autorelease();
    
    CCLog("hasfinished..........push");
    
    
}

void PushMessageReceiver::onBind(std::string appid,std::string userId,std::string channelId,std::string requestId)
{
    PushSetting setting;
    setting.appid = appid;
    setting.userId = userId;
    setting.channelId = channelId;
    setting.requestId = requestId;
    DMPushSettingManager mgr ;
    mgr.saveBind(setting);
    
    std::string url = get_url(HttpUrlTypePushAdduser);
    std::map<string, string> key_value;
    key_value["userid"]=userId;
    CommonHttpManager::getInstance()->send_post(url, key_value, this, CommonHttpJson_selector(PushMessageReceiver::onSumitPushUserIdFinish));
    
}

void PushMessageReceiver::onSumitPushUserIdFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    CCLog("push--绑定手机并提交服务器成功");
}


void PushMessageReceiver::bindUserOnLogin()
{
    DMUser user = DMUserManager::sharedDMUserManager()->onLineUser();
    if (user.uid.length()>0) {
        DMPushSettingManager smgr;
        PushSetting setting = smgr.getBind();
        if (setting.userId.length()==0) {
            return;
        }
        std::string url = get_url(HttpUrlTypePushBindinguser);
        std::map<string, string> key_value;
        key_value["uid"]=user.uid;
        key_value["userid"]=setting.userId;
        CommonHttpManager::getInstance()->send_post(url, key_value, this, CommonHttpJson_selector(PushMessageReceiver::onBindUserFinish));
    }
}

void PushMessageReceiver::onBindUserFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    //添加绑定成功
    CCLog("push--与用户YES绑定成功");
}

void PushMessageReceiver::unBindUserOnLogout()
{
    DMUser user = DMUserManager::sharedDMUserManager()->onLineUser();
    if (user.uid.length()>0) {
        DMPushSettingManager smgr;
        PushSetting setting = smgr.getBind();
        if (setting.userId.length()==0) {
            return;
        }
        std::string url = get_url(HttpUrlTypePushCancelbinding);
        std::map<string, string> key_value;
        key_value["uid"]=user.uid;
        key_value["userid"]=setting.userId;
        CommonHttpManager::getInstance()->send_post(url, key_value, this, CommonHttpJson_selector(PushMessageReceiver::onUnBindUserFinish));
    }
}

void PushMessageReceiver::onUnBindUserFinish(const HttpResponseStatus& status, const CSJson::Value& json)
{
    //解除绑定成功
    CCLog("push--与用户NO解绑成功");
}



DMPushSettingManager::DMPushSettingManager()
{
    
    int ret = 0;
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "brief.db";
    
    ret = sqlite3_open(fullPath.c_str(), &_sqlite3);
    
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS pushsetting(id int auto_increment, appid VARCHAR(50), userid VARCHAR(100), channelid VARCHAR(100) , requestid VARCHAR(100));";
	sqlite3_stmt *stmt;
	int ok=sqlite3_prepare_v2(_sqlite3, sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
    printf("Error in CREATE USER pushsetting\n");
    
    
    //SAVE
    const char *sql_stmt_login = "INSERT INTO pushsetting (appid, userid, channelid,requestid) VALUES (?,?,?,?);";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_stmt_login, -1, &_sqlite_stmt_saveBind, NULL);
    
    //FIND
    const char *sql_stmt_logout = " SELECT appid, userid, channelid ,requestid FROM pushsetting LIMIT 1; ";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_stmt_logout, -1, &_sqlite_stmt_getBind, NULL);
    
    //DELETE
    const char *sql_delete = "DELETE * FROM pushsetting ;";
    ret |= sqlite3_prepare_v2(_sqlite3, sql_delete, -1, &_sqlite_stmt_removeBind, NULL);
    

    
}

DMPushSettingManager:: ~DMPushSettingManager()
{
    sqlite3_finalize(_sqlite_stmt_saveBind);
    sqlite3_finalize(_sqlite_stmt_getBind);
    sqlite3_finalize(_sqlite_stmt_removeBind);
    sqlite3_close(_sqlite3);
}

void DMPushSettingManager::saveBind(PushSetting &pushSetting)
{
    
    int ok = sqlite3_bind_text(_sqlite_stmt_saveBind, 1, pushSetting.appid.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_saveBind, 2, pushSetting.userId.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_saveBind, 3, pushSetting.channelId.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_saveBind, 4, pushSetting.requestId.c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_saveBind);
    ok |= sqlite3_reset(_sqlite_stmt_saveBind);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        printf("Error in DMPushSettingManager::saveBind");
    }
    
}

PushSetting DMPushSettingManager::getBind()
{
    int ok = SQLITE_OK;
    PushSetting set;
    if (sqlite3_step(_sqlite_stmt_getBind)== SQLITE_ROW){
        const unsigned char *appid = sqlite3_column_text(_sqlite_stmt_getBind, 0);
        const unsigned char *userid=sqlite3_column_text(_sqlite_stmt_getBind, 1);
        const unsigned char *channelid=sqlite3_column_text(_sqlite_stmt_getBind, 2);
        const unsigned char *requestid=sqlite3_column_text(_sqlite_stmt_getBind, 3);
        
        set.appid = (const char*)appid;
        set.userId = (const char*)userid;
        set.channelId = (const char*)channelid;
        set.requestId = (const char*)requestid;
    }
    
    ok |= sqlite3_reset(_sqlite_stmt_getBind);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
    printf("Error in DMPushSettingManager::getBind");
    return set;
}

void DMPushSettingManager::removeBind()
{
    int ok = SQLITE_OK;
    
    ok |= sqlite3_step(_sqlite_stmt_removeBind);
	
	ok |= sqlite3_reset(_sqlite_stmt_removeBind);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
    
    printf("Error in DMPushSettingManager::removeBind\n");
}
