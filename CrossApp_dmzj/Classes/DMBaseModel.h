//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef CartoonHouse_DMBaseModel_h
#define CartoonHouse_DMBaseModel_h

#include <iostream>
#include "CrossApp.h"
USING_NS_CC;

struct DMBriefInfo
{
    DMBriefInfo(const string& ID, const string& cover, const string& title, const string& subtitle)
    :ID(ID), cover(cover), title(title), subtitle(subtitle)
    {
    }
    
    std::string ID;
    
    std::string cover;
    
    std::string title;
    
    std::string subtitle;
    
};

struct DMIntroductionInfo
{
    std::string ID;
    
    std::string title;
    
    std::string subtitle;
    
    std::string types;
    
    std::string zone;
    
    std::string status;
    
    std::string last_update_chapter_name;
    
    std::string last_updatetime;
    
    std::string cover;
    
    std::string authors;
    
    std::string description;
    
    std::string first_letter;
    
    std::string direction;
    
    
};

struct DMIntroductionItemInfo
{
    std::string ID;
    
    std::string comic_id;
    
    std::string chapter_name;
    
    std::string chapter_order;
    
    std::string filesize;
    
    bool operator== (const DMIntroductionItemInfo& other) const
    {
        return this->ID.compare(other.ID) == 0 && this->comic_id.compare(other.comic_id) == 0;
    }
};

struct DMDownloadInfo
{
    std::string bookID;
    
    std::string bookTitle;
    
    std::string imageUrl;
    
    std::string alreadyCache;
    
    std::string notCache;
    
    std::string sizeExplain;
};


#endif
