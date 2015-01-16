//
//  HTTP_URL.h
//  CartoonHouse
//
//  Created by 栗元峰 on 14-12-29.
//
//

#ifndef __CartoonHouse__HTTP_URL__
#define __CartoonHouse__HTTP_URL__

#include <CrossApp.h>
#include <CrossAppExt.h>

USING_NS_CC;
USING_NS_CC_EXT;



namespace HTTP_URL
{

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
        
    }
    HttpUrlType;
    
    struct S_HTTP_URL
    {
        std::string url;
        std::map<std::string, std::string> key_value;
    };
    
    S_HTTP_URL get_HTTP_URL(const HttpUrlType& type,
                        const std::string& ID1 = "",
                        const std::string& ID2 = "",
                        const std::string& ID3 = "",
                        int number = 0);

}
#endif /* defined(__CartoonHouse__HTTP_URL__) */
