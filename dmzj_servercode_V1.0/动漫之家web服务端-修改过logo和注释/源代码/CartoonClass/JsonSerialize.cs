using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;
namespace CartoonClass
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class JsonSerialize
    {
        /// <summary>
        /// 序列化成json 对象
        /// </summary>
        /// <param name="o">传入的对象 </param>
        /// <returns></returns>
        public static string jSerialize(object o) {
            JavaScriptSerializer jss_er = new JavaScriptSerializer();
            string _str = jss_er.Serialize(o);
            return _str.Replace(@"\\",@"\");
        }
    }
}
