//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMClassificationViewController__
#define __HelloCpp__DMClassificationViewController__

#include <iostream>
#include "CartoonHouse.h"
#include "DMTabBarController.h"
#include "DMLoginController.h"



struct DMModelClassify
{
    std::string _id;
    std::string icon;
    std::string title;
    vector<DMModelClassify> subnodes;
    
    
};

class DMClassificationViewController
: public CAViewController
, public CACollectionViewDataSource
, public CACollectionViewDelegate
, public DMUserLoginDelegate
{
    
    
public:
    
    DMClassificationViewController();
    
    virtual ~DMClassificationViewController();
    
    static DMClassificationViewController* create();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
public:

    //Necessary
	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);
    
	//Necessary
    virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    
    //Necessary
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    
    virtual unsigned int numberOfSections(CACollectionView *collectionView);
    
	virtual unsigned int collectionViewHeightForHeaderInSection(CACollectionView *collectionView, unsigned int section);

	virtual CAView* collectionViewSectionViewForHeaderInSection(CACollectionView *collectionView, const CCSize& viewSize, unsigned int section);

    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    
    void onButton(CAControl* control, CCPoint point);
    
    void onHeadeClicke(CAControl* control, CCPoint point);
    
    virtual void onActionPerformed(LoginAction action);
    
    void checkUserStatus();
    
private:
    
    vector<DMModelClassify> m_vClassifyModeles;
    
    DMUserLoginDelegate *m_pUserLoginDelegate;
    
    void searchClassfy();
    
    void onSearchClassifyFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void addHeadForegrand();
    
protected:
    
    CALabel* m_pMyInfoLabel;
    
    std::string m_sMyInfoString;
    
    CommonUrlImageView *m_pHeadImgStatus;
    
    CAButton *m_pHeadButton;
    
    int m_iItems;
    
protected:
    
    CACollectionView* m_pCollectionView;
    
    friend class DMTabBarController;
};


#endif /* defined(__HelloCpp__DMClassificationViewController__) */
