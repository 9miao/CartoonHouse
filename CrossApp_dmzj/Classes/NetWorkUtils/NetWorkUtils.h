//
//  NetWorkUtils.h
//  project
//
//  Created by 栗元峰 on 13-2-25.
//
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __project__NetWorkUtils__
#define __project__NetWorkUtils__

#include <iostream>

typedef enum
{
    NetWorkTypeNone,
    NetWorkType3G,
    NetWorkTypeWifi
}
NetWorkType;

class NetWorkUtils
{
public:
    
    bool isNetWorkAvailble();
    
    NetWorkType getNewWorkType();
    
    static NetWorkUtils* getInstance();
};

#endif /* defined(__project__NetWorkUtils__) */
