using adminWeb.adminClass;
using BBDCL;
using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Linq;
using System.Web;
using System.Web.SessionState;
namespace adminWeb.Handler
{
    /// <summary>
    /// delclassify 的摘要说明
    /// </summary>
    public class delclassify : IHttpHandler
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
                    cn.ExecSQL("delete from T0000 where C00001 = '" + id + "'");
                    cn.ExecSQL("delete from T0001 where C00101 = " + id + ""); 
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