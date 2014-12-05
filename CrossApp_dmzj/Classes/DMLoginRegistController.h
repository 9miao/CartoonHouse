//  Copyright (c) 2014 http://www.9miao.com All rights reserved.

#ifndef __HelloCpp__DMLoginRegistController__
#define __HelloCpp__DMLoginRegistController__

#include <iostream>
#include "CartoonHouse.h"
#include <regex.h>

class DMLoginRegistController:
public CAViewController,
public CAMediaDelegate ,
public CATextFieldDelegate
{
public:
    DMLoginRegistController();
    
    ~DMLoginRegistController();
    
    bool init();
    
    virtual void viewDidLoad();
    
    virtual void viewDidUnload();
    
    virtual void getSelectedImage(CAImage *image);
    
    void addPicChoserLayer();
    
    void onHeadClick();
    
    void onCameraBtnClick();
    
    void onAlbumBtnClick();
    
    void onCancelBtnClick();
    
    void onSumbitBtnClick();
    
    virtual bool onTextFieldDetachWithIME(CATextField * sender);
    
    void verifyUser(const std::string &name ,const std::string type);
    
    void onVerifyUserFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void submitUserInfo();
    
    void onSubmitUserInfoComplete(const HttpResponseStatus& status, const CSJson::Value& json);
    
    
private:
    
    CAScrollView *m_pLayoutView;
    
    CAButton *m_pUserHead;
    
    CATextField *m_pNickName;
    
    CATextField *m_pTeleEmailNum;
    
    CATextField *m_pPassword;
    
    CATextField *m_pConfirmPwd;
    
    CAView *m_pPicChoserLayer;
    
    
};

#define SUBSLEN 10              /* 匹配子串的数量 */
#define EBUFLEN 128             /* 错误消息buffer长度 */
#define BUFLEN 1024             /* 匹配到的字符串buffer长度 */

bool static regexEmail(const std::string &str)
{
    
    size_t          len;
    regex_t         re;                     /* 存储编译好的正则表达式，正则表达式在使用之前要经过编译 */
    regmatch_t      subs [SUBSLEN];         /* 存储匹配到的字符串位置 */
    char            errbuf    [EBUFLEN];    /* 存储错误消息 */
    int             err;
    char            pattern   [] = "^([a-zA-Z0-9_\\.\\-\\+])+\\@(([a-zA-Z0-9\\-])+\\.)+([a-zA-Z0-9]{2,4})+$";
    
    /* 编译正则表达式 */
    err = regcomp(&re, pattern, REG_EXTENDED);
    
    if (err) {
        len = regerror(err, &re, errbuf, sizeof(errbuf));
        printf("error: regcomp: %s\n", errbuf);
        return 1;
    }
    /* 执行模式匹配 */
    err = regexec(&re, str.c_str(), (size_t) SUBSLEN, subs, 0);
    
    if (err == REG_NOMATCH) { /* 没有匹配成功 */
        printf("Sorry, no match ...\n");
        regfree(&re);
        return false;
    } else if (err) {  /* 其它错误 */
        len = regerror(err, &re, errbuf, sizeof(errbuf));
        printf("error: regexec: %s\n", errbuf);
        return false;
    }
    else
    {
        printf("\n match successful ");
        
        regfree(&re);   /* 用完了别忘了释放 */
        return true;
    }
    
    
}


#endif /* defined(__HelloCpp__DMLoginRegistController__) */
