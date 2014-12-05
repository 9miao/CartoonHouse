using adminWeb.adminClass;
using BBDCL;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Linq;
using System.Web;
using System.Web.SessionState;

namespace adminWeb.Handler
{
    /// <summary>
    /// getclassiifydown 的摘要说明
    /// </summary>
    public class getclassiifydown : IHttpHandler, IRequiresSessionState
    {


        /// <summary>
        /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
        /// </summary>
        /// 


        public void ProcessRequest(HttpContext context)
        {
            CallBackJson json = new CallBackJson();
            Classify classify = new Classify();
            classify.Status = "Error";
            classify.Msg = "服务器异常";
            try
            {
                string id = context.Request.Form["id"];
                if (string.IsNullOrEmpty(id))
                    throw new Exception("信息不完整");
                using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                {
                    using (DataTable _dt = cn.ExecQuery("select * from T0001 WHERE C00004=?",
                        new OleDbParameter[]{
                            new OleDbParameter("@id",id)
                        }))
                    {
                        if (_dt.Rows.Count > 0)
                        {
                            classify.Status = "Success";
                            classify.classifyName = _dt.Rows[0]["C00006"].ToString();
                            classify.Msg = "获取信息成功";
                        }
                        else
                        {
                            throw new Exception("未找到分类信息");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                classify.Msg = ex.Message;
            }
            context.Response.Write(json.Serializer(classify));
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