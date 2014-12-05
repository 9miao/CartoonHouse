using adminWeb.adminClass;
using BBDCL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.SessionState;
using System.Data.OleDb;
namespace adminWeb.Handler
{
    /// <summary>
    /// classify 的摘要说明
    /// </summary>
    public class classify : IHttpHandler, IRequiresSessionState
    {


        /// <summary>
        /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
        /// </summary>
        /// 


        public void ProcessRequest(HttpContext context)
        {
            CallBackJson json = new CallBackJson();
            jsonClass jclass = new jsonClass();
            jclass.Status = "Error";
            jclass.Msg = "服务器异常";
            try
            {
                string _id = context.Request.Form["id"];
                string _name = context.Request.Form["name"];
                if (string.IsNullOrEmpty(_name.Trim()))
                    throw new Exception("信息参数未空");
                using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                {
                    if (!string.IsNullOrEmpty(_id))
                    {
                        cn.ExecSQL(@"UPDATE T0000 SET C00003=? WHERE C00001=?",
                            new OleDbParameter[]{
                            new OleDbParameter("@C00003",_name),
                            new OleDbParameter("@C00001",_id)
                        });
                        jclass.Msg = "恭喜你！修改大分类成功";
                    }
                    else {
                        string systemid = cn.ExecScalar(@"select max(C00001)+1 from T0000").ToString();
                        if (string.IsNullOrEmpty(systemid))
                            systemid = "1";
                        cn.ExecSQL(@"INSERT INTO T0000 (C00001,C00002,C00003) VALUES (?,?,?)",
                            new OleDbParameter[]{
                            new OleDbParameter("@C00001",systemid),
                            new OleDbParameter("@C00002",""),
                            new OleDbParameter("@C00003",_name)
                        });
                        jclass.Msg = "恭喜你！添加大分类成功";
                    }
                    jclass.Status = "Success";
                    
                }
            }
            catch(Exception ex) {
                jclass.Msg = ex.Message;
            }
            context.Response.Write(json.Serializer(jclass));
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