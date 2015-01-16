

#include "OpenBrowser.h"
#include "platform/android/jni/JniHelper.h"
USING_NS_CC;

void OpenBrowser::openUrl(const std::string& strUrl)
{
	const char* class_name = "com/component/impl/WebViewCtrl";
	JniMethodInfo info;
	jobject dthero;
	if (JniHelper::getStaticMethodInfo(info, class_name,
			"getInstance", "()Lcom/component/impl/WebViewCtrl;"))
    {
		dthero = info.env->CallStaticObjectMethod(info.classID, info.methodID);
	}
    
	if (dthero && JniHelper::getMethodInfo(info, class_name, "browserOpenURL", "(Ljava/lang/String;)V"))
    {
		info.env->CallVoidMethod(dthero, info.methodID, info.env->NewStringUTF(strUrl.c_str()));
	}
}
