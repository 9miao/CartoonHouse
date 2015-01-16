//
//  NetWorkUtils.cpp
//  project
//
//  Created by 栗元峰 on 14-2-25.
//
//

#include "../NetWorkUtils.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import <Foundation/Foundation.h>
#import <netinet/in.h>
#include "Reachability.h"

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
    Reachability *r = [Reachability reachabilityWithHostname:@"www.apple.com"];
    return [r isReachable];
}

NetWorkType NetWorkUtils::getNewWorkType()
{
    NetWorkType networkType = NetWorkTypeNone;
    Reachability *r = [Reachability reachabilityWithHostname:@"www.apple.com"];
    switch ([r currentReachabilityStatus])
    {
        case NotReachable:
            networkType=NetWorkTypeNone;
            break;
        case ReachableViaWWAN:
            networkType=NetWorkType3G;
            break;
        case ReachableViaWiFi:
            networkType=NetWorkTypeWifi;
            break;  
    }  
    return networkType;  
}