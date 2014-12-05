using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.Script.Serialization;

namespace adminWeb.adminClass
{
    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    
    public class CallBackJson
    {
        /// <summary>
        /// 反序列化json
        /// </summary>
        /// <param name="UD">传入参数</param>
        /// <returns>返回结果</returns>
        public T Deserialize<T>(string UD)
        {
            JavaScriptSerializer jss_er = new JavaScriptSerializer();
            return jss_er.Deserialize<T>(UD);
        }

        /// <summary>
        /// 序列化json
        /// </summary>
        /// <param name="UD">传入参数</param>
        /// <returns>返回结果</returns>
        public string Serializer(object UD)
        {
            JavaScriptSerializer jss = new JavaScriptSerializer();
            StringBuilder s1 = new StringBuilder();
            jss.Serialize(UD, s1);
            return s1.ToString();
        }
    }
}