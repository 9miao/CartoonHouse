#include "com_dmzj_manhua_push_MessagePushTool.h"
#include "CrossApp.h"
#include "PushMessageReceiver.h"

static string jstring2string(jstring jstr ,JNIEnv *env)
    {
        if (jstr == NULL)
        {
            return "";
        }
        const char* chars = env->GetStringUTFChars(jstr, NULL);
        string ret(chars);
        env->ReleaseStringUTFChars(jstr, chars);

        return ret;
    }

JNIEXPORT void JNICALL Java_com_dmzj_manhua_push_MessagePushTool_onMessageObtain
  (JNIEnv *env, jclass obj, jobject context, jstring title, jstring description, jstring contentstr)
{
	jclass LogClass = env->FindClass("android/util/Log");
	jmethodID dID = env->GetStaticMethodID(LogClass,"d","(Ljava/lang/String;Ljava/lang/String;)I");
	jstring tag = env->NewStringUTF("Demo");
	jstring infostart = env->NewStringUTF("----- message -----");
	env->CallStaticIntMethod(LogClass,dID,tag,infostart);//


	std::string mTitle = jstring2string(title,env);
	env->CallStaticIntMethod(LogClass,dID,tag,infostart);//
	std::string mDesc = jstring2string(description,env);
	env->CallStaticIntMethod(LogClass,dID,tag,infostart);//
	std::string mMesg = jstring2string(contentstr,env);
	env->CallStaticIntMethod(LogClass,dID,tag,infostart);//
	PushMessageReceiver receiver ;
	env->CallStaticIntMethod(LogClass,dID,tag,infostart);//
	CCLog("title=%s,desc=%s,msg=%s",mTitle.c_str(),mDesc.c_str(),mMesg.c_str());
	receiver.handleMessage(mTitle,mDesc,mMesg);
	env->CallStaticIntMethod(LogClass,dID,tag,infostart);//



}

JNIEXPORT void JNICALL Java_com_dmzj_manhua_push_MessagePushTool_onBaiduBind
  (JNIEnv *env, jobject obj, jstring appid, jstring userId , jstring channelId, jstring requestId)
{
	std::string mAppId = jstring2string(appid,env);
	std::string mUserId = jstring2string(userId,env);
	std::string mChannelId = jstring2string(channelId,env);
	std::string mRequestId = jstring2string(requestId,env);
	PushMessageReceiver *receiver  = new PushMessageReceiver();
	receiver->onBind(mAppId,mUserId,mChannelId,mRequestId);
	receiver->autorelease();

}











