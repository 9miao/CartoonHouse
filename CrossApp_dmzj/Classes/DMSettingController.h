//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__SettingController__
#define __HelloCpp__SettingController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMSettingManager.h"
#include "DMLoginController.h"

class DMSettingController :public CAViewController
,public CATableViewDataSource
,public CATableViewDelegate
,public DMUserLoginDelegate
{
    
public:
    DMSettingController();
    
    ~DMSettingController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
protected:
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    
    virtual unsigned int numberOfSections(CATableView *table);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section);
    
    void onUserOnline();
    
    void onUserOffLine();
    
    void userLogin();
    
    void userLogout();
    
    virtual void viewDidAppear();
    
    CATableViewCell * generateCell(const CCSize& cellSize);
    
    //移动网络观看
    void onOnlineWatchSwitch(CAControl*, CCPoint);
    //移动网络下载
    void onDownloadWatch(CAControl*, CCPoint);
    //观看模式
    void onWatchModel(CAControl*, CCPoint);
    
    virtual void onActionPerformed(LoginAction action);
    
    void addWanderOraintationLayer();
    
    void onHorizontoalSet();
    
    void onVeticialSet();
    
    void onNonSetClick();
    
    
protected:
    
    CATableView * m_pTableView;
    
    DMSettingManager * m_pSettingmanger ;
    
    CAButton *m_pLoginOutBtn;
    
    CAView *m_pWanderOraintaionLayer;
};



#endif /* defined(__HelloCpp__SettingController__) */
