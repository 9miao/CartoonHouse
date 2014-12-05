//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//
//                            _ooOoo_
//                           o8888888o
//                           88" . "88
//                           (| -_- |)
//                            O\ = /O
//                         ___/`---'\___
//                       .'   \\| |//   `.
//                      /  \\||| : |||//  \
//                     / _||||| -:- |||||- \
//                     |   | \\\ - /// |   |
//                     | \_| ''\---/'' |_/ |
//                     \  .-\__ `-` ___/-. /
//                   ___`. .' /--.--\ `. . __
//                ."" '< `.___\_<|>_/___.' >'"".
//               | | : `- \`.;`\ _ /`;.`/ - ` : | |
//               \ \ `-.   \  9 \ /miao/    .-` / /
//         ======`-.____`-._\Cross_App/_.-`____.-'======
//                            `=---='
//
//         .............................................
//                  佛祖镇楼                  BUG辟易
//          佛曰:
//                  写字楼里写字间，写字间里程序员；
//                  程序人员写程序，又拿程序换酒钱。
//                  酒醒只在网上坐，酒醉还来网下眠；
//                  酒醉酒醒日复日，网上网下年复年。
//                  但愿老死电脑间，不愿鞠躬老板前；
//                  奔驰宝马贵者趣，公交自行程序员。
//                  别人笑我忒疯癫，我笑自己命太贱；
//                  不见满街漂亮妹，哪个归得程序员？


#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CartoonHouse.h"
#include "umSDK.h"

class RootWindow: public CAWindow, public CAKeypadDelegate
{
    
public:

	static RootWindow* getInstance();
    
	RootWindow();
    
	virtual ~RootWindow();
    
    virtual bool init();
    
    CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
    
    CC_SYNTHESIZE_READONLY(CATabBarController*, m_pRootTabBarController, RootTabBarController);
    
    CC_SYNTHESIZE_READONLY(CADrawerController*, m_pRootDrawerController, DrawerController);
    
    CC_SYNTHESIZE(CANavigationController*, m_pSecondNavigationController, SecondNavigationController);
    
    CC_SYNTHESIZE_READONLY(sqlite3*, m_pSqlite3, Sqlite3);
    
    virtual void keyBackClicked();
    
    virtual void keyMenuClicked();
    
protected:
    
    void onAlertView(int index);
    
    bool m_bAlertShow;
};


#endif /* defined(__HelloCpp__ViewController__) */
