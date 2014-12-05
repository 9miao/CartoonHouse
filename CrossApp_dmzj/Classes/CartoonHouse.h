//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//
//                       .::::.
//                     .::::::::.
//                    :::::::::::
//                 ..:::::::::::'
//              '::::::::::::'
//                .::::::::::
//           '::::::::::::::..
//                ..::::::::::::.
//              ``::::::::::::::::
//               ::::``:::::::::'        .:::.
//              ::::'   ':::::'       .::::::::.
//            .::::'      ::::     .:::::::'::::.
//           .:::'       :::::  .:::::::::' ':::::.
//          .::'        :::::.:::::::::'      ':::::.
//         .::'         ::::::::::::::'         ``::::.
//     ...:::           ::::::::::::'              ``::.
//    ```` ':.          ':::::::::'                  ::::..
//                       '.:::::'                    ':'````..
//


#ifndef HelloCpp_CartoonHouse_h
#define HelloCpp_CartoonHouse_h

#include <CrossApp.h>
#include "CommonHttpManager.h"
#include "RootWindow.h"
#include "DMBaseModel.h"
USING_NS_CC;

/*********************************************************************/
#pragma ziyuan

static const char * version = "版本:V1.0.0";
static const char * version_code = "1.0.0";


static const char* searchImage = "dm_resource/title_search.png";
static const char* tabBarBackground = "dm_resource/tabbar_background.png";
static const char* navigationBackground = "dm_resource/navigation_background.png";

static const char* loadingBackground = "dm_resource/loading_background.png";
static const char* loadingIcon = "dm_resource/loading_icon.png";

//TabBar

static const char* menuTitle[5] =
{
    "分类", "推荐", "订阅", "下载", "更多"
};

static const char* menuImageNormal[5] =
{
    "dm_resource/tabbar_classification_nromal.png",
    "dm_resource/tabbar_recommend_normal.png",
    "dm_resource/tabbar_subscribe_normal.png",
    "dm_resource/tabbar_download_normal.png",
    "dm_resource/tabbar_more_normal.png"
};

static const char* menuImageSelected[5] =
{
    "dm_resource/tabbar_classification_selected.png",
    "dm_resource/tabbar_recommend_selected.png",
    "dm_resource/tabbar_subscribe_selected.png",
    "dm_resource/tabbar_download_selected.png",
    "dm_resource/tabbar_more_selected.png"
};



//分类

static const char* classifyPath = "dm_resource/classification/";
static const char* classifyItemBg = "dm_resource/classify_itemBg.png";
static const char* dmDefaultHeadImage = "dm_resource/classification/dm_default_head.png";
static const char* dmHeadTopImage = "dm_resource/classification/dm_head_top.png";
//static const char* cellBackgroundImage = "dm_resource/classification/dm_backGround.png";

//推荐

#define SUBSCRIBE_SECTION_COUNT 5
static const char* titleText[5] =
{
    "推荐", "最新", "连载", "完结", "原创"
};

static const char* catalogTitleTag[SUBSCRIBE_SECTION_COUNT] =
{
    "index", "new", "lianzai", "wanjie", "yuanchuang"
};

static const char* titleSelected = "dm_resource/titile_bg_selected.png";
static const char* title_bg_first = "dm_resource/catalog/title_bg_first.png";
static const char* title_bg_other = "dm_resource/catalog/title_bg_other.png";
static const char* book_bg = "dm_resource/catalog/book_bg.png";


//订阅
static const char* loginIcon       = "dm_resource/login_icon.png";
static const char* searchIcon      = "dm_resource/title_search.png";
static const char* segNormal1      = "dm_resource/seg_bg_normal_1.png";
static const char* segSelected1    = "dm_resource/seg_bg_selected_1.png";
static const char* segNormal2      = "dm_resource/seg_bg_normal_2.png";
static const char* segSelected2    = "dm_resource/seg_bg_selected_2.png";
static const char* segNormal3      = "dm_resource/seg_bg_normal_3.png";
static const char* segSelected3    = "dm_resource/seg_bg_selected_3.png";

//排行榜，返回图片
static const char* backBtn    = "dm_resource/btn_left.png";

//详细介绍
static const char* addCollectionButtonIcon  = "dm_resource/Introduction/btn_addCollection.png";
static const char* startReadButtonIcon      = "dm_resource/Introduction/btn_startRead.png";
static const char* cellNormalIcon           = "dm_resource/Introduction/cell_normal.png";
static const char* cellSelectedIcon         = "dm_resource/Introduction/cell_selected.png";
static const char* cellHaveDowloadIcon      = "dm_resource/Introduction/cell_have_normal.png";
static const char* sortButton  = "dm_resource/Introduction/btn_sort.png";
static const char* contractionLabelButtonIcon  = "dm_resource/Introduction/btn_contractionLabel.png";
static const char* openLabelButtonIcon  = "dm_resource/Introduction/btn_openLabel.png";
static const char* dowloadButtonMinIcon = "dm_resource/Introduction/btn_downloadMin.png";
static const char* divisionIcon = "dm_resource/Introduction/icon_division.png";
static const char* feedbackicon = "dm_resource/Introduction/dm_comment_icon.png";
static const char* getMoreReplyBtn = "dm_resource/Introduction/btn_morereply.png";
static const char* common_tabbarIndicator = "dm_resource/common_tabbar_indicator.png";

static const char* tabBatTitle[3] =
{
    "漫画剧集", "相关评论", "相关推荐"
};

//下载
static const char* downloadButtonIcon = "dm_resource/Introduction/ben_download.png";
static const char* allSelectionButtonIcon = "dm_resource/Introduction/btn_allSelection.png";
static const char* allUnSelectionButtonIcon = "dm_resource/Introduction/btn_allUnSelection.png";

static const char* downloadButtonSee = "dm_resource/download/download_btn_see.png";
static const char* downloadButtonPause = "dm_resource/download/download_btn_pause.png";
static const char* downloadButtonStart = "dm_resource/download/download_btn_start.png";

static const char* downloadTabBatTitle[2] =
{
    "已下载", "正在下载"
};



//漫画观看
static const char* brightnessButtonIcon = "dm_resource/browse/btn_brightness.png";
static const char* collectButtonNormalIcon = "dm_resource/browse/btn_collect.png";
static const char* collectButtonSelectedIcon = "dm_resource/browse/btn_collect_selected.png";
static const char* directionButtonIcon = "dm_resource/browse/btn_direction.png";
static const char* aboutButtonIcon = "dm_resource/browse/btn_about.png";
static const char* brightnessSliderIcon = "dm_resource/browse/slider_brightness.png";
static const char* brightnessSliderBg = "dm_resource/browse/bg_brightness.png";
static const char* pagecontrolSliderIcon = "dm_resource/browse/slider_pagecontrol.png";

static const char* brightnessButtonTitle = "亮度";
static const char* collectButtonNormalTitle = "收藏";
static const char* collectButtonSelectedTitle = "已收藏";
static const char* directionButtonTitle[2] = { "横屏", "竖屏" };
static const char* aboutButtonTitle[2] = { "左手模式", "右手模式" };

static const char* operationRemindH_Icon = "dm_resource/browse/operation_remind_h.png";
static const char* operationRemindV_Icon = "dm_resource/browse/operation_remind_v.png";

/*********************************************************************/


typedef enum
{
    HttpUrlTypeUserLogin,           //用户登录
    HttpUrlTypeRecommend,           //推荐 首页数据
    HttpUrlTypeClassifyify,         //分类 tree 数据
    HttpUrlTypeRank,                //分类 排行榜数据
    HttpUrlTypeClassifCatalog,      //分类节点数据 排序类型，分类的id，当前显示的数量
    HttpUrlTypeBookDetail,          //漫画介绍
    HttpUrlTypeChapPages,           //某一话对应的所有图片url: bookId chapId
    HttpUrlTypeNextChapPages,       //某一话的下一话对应的所有图片url: bookId chapId
    HttpUrlTypePrevChapPages,       //某一话的上一话对应的所有图片url: bookId chapId
    HttpUrlTypeBookDownload,        //漫画下载 imgzip.dmzj.com/漫画首字母/漫画id/话的id
    HttpUrlTypeComicFuzzySearch,    //模糊搜索
    HttpUrlTypeComicSearch ,        //全部搜索
    HttpUrlTypeComicSearchHot,      //热门搜索
    HttpUrlTypeTop,                 //排行榜
    HttpUrlTypeRegistVerify,        //注册验证用户名
    HttpUrlTypeRegist,              //注册
    HttpUrlTypeSubScribComic,       //订阅查询
    HttpUrlTypeSubScribAdd,         //订阅添加
    HttpUrlTypeSubScribDel,         //订阅删除
    HttpUrlTypeGetComment,           //得到评论信息
    HttpUrlTypeGetCommentReply,      //得到评论回复
    HttpUrlTypeAddComment,           //添加评论
    HttpUrlTypeComicreport,          //反馈
    HttpUrlTypePushAdduser,          //给服务器提交百度帐号
    HttpUrlTypePushBindinguser,      //百度用户绑定
    HttpUrlTypePushCancelbinding,     //解除百度用户绑定
    HttpUrlTypeThreePartyLand        //三方登陆
    
}HttpUrlType;


static std::string get_url(const HttpUrlType& type, const std::string& ID1 = "", const std::string& ID2 = "", const std::string& ID3 = "", int number = 0)
{
    std::string url;
    switch (type)
    {
        case HttpUrlTypeRecommend:
        {
            url = "http://dm.uuu.net/static/index.ashx";
        }
            break;
        case HttpUrlTypeClassifyify:
        {
            url = "http://dm.uuu.net/static/classify.ashx";
        }
            break;
        case HttpUrlTypeRank:
        {

        }
            break;
        case HttpUrlTypeClassifCatalog:
        {
            char str[8];
            sprintf(str, "%d", number);
            url = "http://dm.uuu.net/dynamic/comicclassify.ashx?classify_type=" + ID1 + "&classify_id=" + ID2 + "&page=" + str;
        }
            break;
        case HttpUrlTypeBookDetail:
        {
            url = "http://dm.uuu.net/static/comicinfo.ashx?comic_id=" + ID1;
        }
            break;
        case HttpUrlTypeChapPages:
        {
            url = "http://dm.uuu.net/static/comicread.ashx?comic_id=" + ID1 + "&chap_id=" + ID2;
        }
            break;
        case HttpUrlTypeNextChapPages:
        {

        }
            break;
        case HttpUrlTypePrevChapPages:
        {

        }
            break;
        case HttpUrlTypeBookDownload:
        {
            url = "http://imgzip.dmzj.com/" + ID1 + "/" + ID2 + "/" + ID3;
        }
            break;
        case HttpUrlTypeComicFuzzySearch:
        {
            

        }
            break;
        case HttpUrlTypeComicSearch:
        {

        }
            break;
        case HttpUrlTypeComicSearchHot:
        {
            url = "http://dm.uuu.net/dynamic/comicsearchhot.ashx";
        }
            break;
        case HttpUrlTypeTop:
        {
            url = "http://dm.uuu.net/static/top.ashx";
        }
            break;
        case HttpUrlTypeRegistVerify:
        {

        }
            break;
        case HttpUrlTypeRegist:
        {

        }
            break;
        case HttpUrlTypeSubScribComic:
        {

        }
            break;
        case HttpUrlTypeSubScribAdd:
        {

        }
            break;
        case HttpUrlTypeSubScribDel:
        {

        }
            break;
        case HttpUrlTypeGetComment:
        {

        }
            break;
        case HttpUrlTypeGetCommentReply:
        {

        }
            break;
        case HttpUrlTypeAddComment:
        {

        }
            break;
        case HttpUrlTypeComicreport:
        {

        }
            break;
        case HttpUrlTypeUserLogin:
        {

        }
            break;
        case HttpUrlTypePushAdduser:
        {

        }
        break;
        
        case HttpUrlTypePushBindinguser:
        {

        }
        break;
        case HttpUrlTypePushCancelbinding:
        {

        }
        break;
        case HttpUrlTypeThreePartyLand:
        {

        }
        break;
        
        default:
            break;
    }
    
    return url;
}





#endif
