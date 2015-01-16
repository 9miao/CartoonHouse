//
//  NetWorkUtils.cpp
//  project
//
//  Created by 栗元峰 on 14-2-25.
//
//

#include "../NetWorkUtils.h"
#include "platform/android/jni/JniHelper.h"
USING_NS_CC;

static NetWorkUtils* instance = NULL;

NetWorkUtils* NetWorkUtils::getInstance()
{
	if (instance == NULL)
	{
		instance = new NetWorkUtils();
	}
    
	return instance;
}

bool NetWorkUtils::isNetWorkAvailble()
{

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/jni/PhoneNet","returnType","()I");
    jint _int;
    
    if (isHave)
        _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);

    return (bool)(_int > 0);
}

NetWorkType NetWorkUtils::getNewWorkType()
{
    NetWorkType networkType = NetWorkTypeNone;
    
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/jni/PhoneNet","returnType","()I");
    jint _int;
    
    if (isHave)
        _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
    
    switch (_int)
    {
        case 0:
            networkType = NetWorkTypeNone;
            break;
        case 1:
            networkType = NetWorkTypeWifi;
            break;
        case 2:
        case 3:
            networkType = NetWorkType3G;
            break;
    }  
    return networkType;  
}
