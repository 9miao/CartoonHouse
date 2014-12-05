//
//  NetWorkUtils.h
//  project
//
//  Created by 栗元峰 on 14-2-25.
//
//


#include "NetWorkUtils.h"

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
    return true;
}

NetWorkType NetWorkUtils::getNewWorkType()
{
    NetWorkType networkType = NetWorkTypeWifi;

    return networkType;
}

