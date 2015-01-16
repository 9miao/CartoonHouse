//
//  ThreePartyHelper.cpp
//  CartoonHouse
//
//  Created by liuguoyan on 14-9-28.
//
//

#include "../ThreePartyHelper.h"
#include "platform/android/jni/JniHelper.h"
USING_NS_CC;
extern "C"

const char* nativeLogin(int platfrom)
{
	JniMethodInfo jmi;
		if(JniHelper::getStaticMethodInfo(jmi , "com/dmzj/manhua/openapi/OpenApiHelper" , "onLogin" , "(I)V"))
		{
			jmi.env->CallStaticVoidMethod(jmi.classID , jmi.methodID , platfrom);
		}
}

void ThreePartyHelper::onThreePartyLogin(int paltform)
{
	nativeLogin(paltform);
}
