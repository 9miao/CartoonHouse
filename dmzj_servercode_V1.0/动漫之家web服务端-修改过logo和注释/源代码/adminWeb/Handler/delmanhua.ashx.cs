using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data.OleDb;
using BBDCL;
namespace adminWeb.Handler
{
    /// <summary>
    /// delmanhua 的摘要说明
    /// </summary>
    public class delmanhua : IHttpHandler
    {


        /// <summary>
        /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
        /// </summary>
        /// 


        public void ProcessRequest(HttpContext context)
        {
            string id = context.Request.Form["thisid"];
           if (!string.IsNullOrEmpty(id))
           {
               using (CDBAccess cn = new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
               {
                   cn.ExecSQL("delete from T0003 where C00010 = "+id+"");
                   cn.ExecSQL("delete from T0004 where C00016 = " + id + "");
                   cn.ExecSQL("delete from T0005 where C00030 = " + id + ""); 
                   context.Response.Write("0");
                   context.Response.End();
               }
           }
           else
           {
               context.Response.Write("参数错误");
               context.Response.End();
           }
        }

        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
}