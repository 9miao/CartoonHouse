#include "com_dmzj_manhua_openapi_OpenApiHelper.h"
#include "DMLoginController.h"


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

JNIEXPORT void JNICALL Java_com_dmzj_manhua_openapi_OpenApiHelper_onTokenReturn
  (JNIEnv * env, jclass obj, jint plat, jstring token ,jstring uid)
{
	jclass LogClass = env->FindClass("android/util/Log");
		jmethodID dID = env->GetStaticMethodID(LogClass,"d","(Ljava/lang/String;Ljava/lang/String;)I");
		jstring tag = env->NewStringUTF("Demo");
		jstring infostart = env->NewStringUTF("----- onTokenReturn -----");
		env->CallStaticIntMethod(LogClass,dID,tag,infostart);//
	//
	std::string mtoken = jstring2string(token,env);
	std::string muid = jstring2string(uid,env);
	if(plat == 1){
		DMLoginController::getInstance()->threePartyLand("qq",mtoken,muid);
	}
	else if(plat ==2){
		DMLoginController::getInstance()->threePartyLand("weibo",mtoken,muid);
	}

}
