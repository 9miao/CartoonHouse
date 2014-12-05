using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Web;

namespace BBDCL.PageCache
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


     public static  class  CConvert
     {
         public  static int ObjectToInt(object obj, int defaultValue = 0)
         {
             int result = defaultValue;
             try
             {
                 result = int.Parse(obj.ToString());
             }catch
             {
                 
             }
             return result;
         }

         public static int objAdd(object o,int value)
         {
             int io = CConvert.ObjectToInt(o, 0);
             io += value;
             return io;
         }

         public static int objSub(object o,int value)
         {
             int io = CConvert.ObjectToInt(o, 0);
             io -= value;
             return io;
         }

         public static int objSubWithMix(object o,int value,int mixValue)
         {
             int i = CConvert.objSub(o, value);
             if (i < mixValue) return mixValue; else return i;
         }


         public static float obj2Float(object obj,float defaultValue = 0f)
         {
             float result = 0f;
             try
             {
                 result = float.Parse(obj.ToString());
             }catch
             {
                 result = defaultValue;
             }
             return result;
         }

         public static string obj2str(object obj,string defaultValue = "")
         {
             string result = "";
             try
             {
                 result = obj.ToString();
             }
             catch
             {
                 result = defaultValue;
             }
             return result;
         }

         public static bool obj2bool(object obj,bool defaultValue=false)
         {
             bool result=defaultValue;
             try
             {
                 result = bool.Parse(obj.ToString());
             }catch
             {
                 
             }
             return result;
         }

         public static string NoHtml(string text)
         {
             //删除脚本
             text = Regex.Replace(text, @"<script[^>]*?>.*?</script>", "", RegexOptions.IgnoreCase);
             //删除HTML
             text = Regex.Replace(text, @"<(.[^>]*)>", "", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"([\r\n])[\s]+", "", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"-->", "", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"<!--.*", "", RegexOptions.IgnoreCase);


             text = Regex.Replace(text, @"&(quot|#34);", "\"", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(amp|#38);", "&", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(lt|#60);", "<", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(gt|#62);", ">", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(nbsp|#160);", " ", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(iexcl|#161);", "\xa1", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(cent|#162);", "\xa2", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(pound|#163);", "\xa3", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&(copy|#169);", "\xa9", RegexOptions.IgnoreCase);
             text = Regex.Replace(text, @"&#(\d+);", "", RegexOptions.IgnoreCase);

             text.Replace("<", "");
             text.Replace(">", "");
             text.Replace("\r\n", "");

             text = HttpContext.Current.Server.HtmlEncode(text).Trim();
             return text;
         }
     }
}
