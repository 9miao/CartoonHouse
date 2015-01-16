//
//  HTTP_URL.cpp
//  CartoonHouse
//
//  Created by 栗元峰 on 14-12-29.
//
//

#include "HTTP_URL.h"
#include "des.h"
namespace HTTP_URL
{

char GUID[] = "A0000000A0000000A0000000A0000000";
    
typedef std::map<std::string, std::string> MAP_JSON;
    
std::string format_json(const MAP_JSON& map)
{
    CSJson::FastWriter writer;
    CSJson::Value root;
    
    std::string appKey = "102743592058620C59595E2BF06D11F0";
    root["Appkey"] = static_cast<const char*>(CrossApp::base64Encode(appKey.c_str(), appKey.length()));
    CCLog("--- %s", CrossApp::base64Encode(appKey.c_str(), appKey.length()));
    std::string userid = "101";
    root["Userid"] = static_cast<const char*>(CrossApp::base64Encode(userid.c_str(), userid.length()));
    CCLog("--- %s", root["Userid"].asString().c_str());
    std::string userKey = "4E0818C74E904E82B214A86B1CF84A77";
    root["UserKey"] = static_cast<const char*>(CrossApp::base64Encode(userKey.c_str(), userKey.length()));
    CCLog("--- %s", root["UserKey"].asString().c_str());
    
    for (MAP_JSON::const_iterator itr = map.begin();
         itr != map.end();
         itr++)
    {
        root[itr->first] = CrossApp::base64Encode(itr->second.c_str(), itr->second.length());
    }
    
    return writer.write(root);
}

S_HTTP_URL get_HTTP_URL(const HttpUrlType& type,
                    const std::string& ID1,
                    const std::string& ID2,
                    const std::string& ID3,
                    int number)
{
    
    S_HTTP_URL s_http_url;
    s_http_url.url = "http://dm.uuu.net/";
    MAP_JSON map;
    
    
    switch (type)
    {
        case HttpUrlTypeRecommend:
        {
            s_http_url.url += std::string("index.ashx");
        }
            break;
        case HttpUrlTypeClassifyify:
        {
            s_http_url.url += std::string("classify.ashx");
        }
            break;
        case HttpUrlTypeRank:
        {
            
        }
            break;
        case HttpUrlTypeClassifCatalog:
        {
            s_http_url.url += std::string("Comicclassify.ashx");
            map["letter"] = ID1;
            map["id"] = ID2;
            char str[8];
            sprintf(str, "%d", number);
            map["page"] = str;
        }
            break;
        case HttpUrlTypeBookDetail:
        {
            s_http_url.url += std::string("Comicinfo.ashx");
            map["Comic_id"] = ID1;
        }
            break;
        case HttpUrlTypeChapPages:
        {
            s_http_url.url += std::string("Comicread.ashx");
            map["Comic_id"] = ID1;
            map["chap_id"] = ID2;
        }
            break;
        case HttpUrlTypeNextChapPages:
        {
            s_http_url.url += std::string("Comicnext.ashx");
            map["Comic_id"] = ID1;
            map["chap_id"] = ID2;
        }
            break;
        case HttpUrlTypePrevChapPages:
        {
            s_http_url.url += std::string("Comiclast.ashx");
            map["Comic_id"] = ID1;
            map["chap_id"] = ID2;
        }
            break;
        case HttpUrlTypeBookDownload:
        {
            s_http_url.url = "http://imgzip.dmzj.com/" + ID1 + "/" + ID2 + "/" + ID3;
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
            s_http_url.url += std::string("Comicsearchhot.ashx");
        }
            break;
        case HttpUrlTypeTop:
        {
            s_http_url.url += std::string("Top.ashx");
        }
            break;
        case HttpUrlTypeRegistVerify:
        {
            map["type"] = ID1;
            map["content"] = ID2;
        }
            break;
        case HttpUrlTypeRegist:
        {
            map["nickname"] = ID1;
            map["email"] = ID2;
            map["passwd"] = ID3;
        }
            break;
        case HttpUrlTypeSubScribComic:
        {
            map["uid"] = ID1;
        }
            break;
        case HttpUrlTypeSubScribAdd:
        {
            map["uid"] = ID1;
            map["comicid"] = ID2;
        }
            break;
        case HttpUrlTypeSubScribDel:
        {
            map["uid"] = ID1;
            map["comicid"] = ID2;
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
            map["nickname"] = ID1;
            map["passwd"] = ID2;
        }
            break;
        case HttpUrlTypePushAdduser:
        {
            map["userid"] = ID1;
        }
            break;
            
        case HttpUrlTypePushBindinguser:
        {
            map["uid"] = ID1;
            map["userid"] = ID2;
        }
            break;
        case HttpUrlTypePushCancelbinding:
        {
            
        }
            break;
        case HttpUrlTypeThreePartyLand:
        {
            map["channel"] = ID1;
            map["token"] = ID2;
            map["uid"] = ID3;
        }
            break;
            
        default:
            break;
    }

    std::string json = format_json(map);

    int len = 0;
    char *postvalue = NULL;
    char *pvalue;
    
    postvalue = new char[json.length() * 4 / 3 + 10 + 32 * 2];
    pvalue = postvalue;
    
    int i = 0;
    len = 16;
    for (i = 0; i < len; i ++)
    {
        pvalue[i*2] = "0123456789abcdef"[(GUID[i]&0xF0)>>4];
        pvalue[i*2+1] = "0123456789abcdef"[GUID[i]&0x0F];
    }
    pvalue += i*2;
    
    len = json.length();
    Base64Encode((unsigned char*)pvalue, (unsigned char*)json.c_str(), len);
    pvalue += len;
    len = 16;
    for (i = 0; i < len; i ++)
    {
        pvalue[i*2] = "0123456789abcdef"[(GUID[i]&0xF0)>>4];
        pvalue[i*2+1] = "0123456789abcdef"[GUID[i]&0x0F];
    }
    pvalue += i*2;
    *pvalue = '\0';
    
    len = strlen(postvalue);
    
    char *desValue = new char[(len + 8) * 2];
    
    char key[] = "JKASJHKJDAH$";
    len = des::DESCrypto(postvalue, desValue, len, key, NULL, 0, false);
    
    delete []postvalue;
    postvalue = NULL;
    
    std::string tmp = CrossApp::base64Encode(desValue, len);
    postvalue = CrossApp::base64Encode(tmp.c_str(), tmp.length());
    s_http_url.key_value["ud"] = postvalue;
    
    delete []desValue;

    CCLog("---->%s %s", s_http_url.url.c_str(), s_http_url.key_value["ud"].c_str());
    
//    char* json_in = const_cast<char*>(json.c_str());
//    
//    char* json_out = CrossApp::base64Encode(json_in, json.length());
//    
//    std::string new_json_out = GUID + json_out + GUID;
//
//    int len = new_json_out.length();
//    
//    char* postVlaue = const_cast<char*>(new_json_out.c_str());
//    
//    char *desValue = new char[(len + 8) * 2];
//    
//    char key[] = "JKASJHKJDAH$";
//    
//    len = des::DESCrypto(postVlaue, desValue, len, key, NULL, 0, false);
//    
//    std::string ud;
//    ud.resize((len + 8) * 2, sizeof(char));
//    for (int i=0; i<ud.length(); i++)
//    {
//        ud[i] = desValue[i];
//    }
//
//    delete []desValue;
//
//    url += ud;
//    

    
    return s_http_url;
}

}