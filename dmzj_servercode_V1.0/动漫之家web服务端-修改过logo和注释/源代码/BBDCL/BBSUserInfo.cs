using System;
using System.Collections.Generic;
using System.Text;

namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    [Serializable] 
    public  class BBSUserInfo
    {
        public string UserId { get; set; }
        public string UserType { get; set; }
        public string UserName { get; set; }
        public string UserLoginCheckCode { get; set; }
    }
}
