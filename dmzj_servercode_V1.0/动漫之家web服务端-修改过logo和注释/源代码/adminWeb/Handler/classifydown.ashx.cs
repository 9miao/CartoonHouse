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
    /// classifydown 的摘要说明
    /// </summary>
    public class classifydown : IHttpHandler, IRequiresSessionState
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
                string _id = context.Request.Form["id"];            //小分类id
                string _name = context.Request.Form["name"];        //小分类名称
                string _type = context.Request.Form["type"];        //类型 0修改  1添加
                if (string.IsNullOrEmpty(_name.Trim()))
                    throw new Exception("信息参数未空");
                using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                {
                    if (_type=="0")
                    {
                        cn.ExecSQL(@"UPDATE T0001 SET C00006=? WHERE C00004=?",
                            new OleDbParameter[]{
                            new OleDbParameter("@C00003",_name),
                            new OleDbParameter("@C00001",_id)
                        });
                        jclass.Msg = "恭喜你！修改分类成功";
                    }
                    else
                    {
                        string systemid = cn.ExecScalar(@"select max(Val(C00004))+1 from T0001 where C00101 <> 6 ").ToString();
                        if (string.IsNullOrEmpty(systemid))
                            systemid = "1";
                        cn.ExecSQL(@"INSERT INTO T0001 (C00004,C00005,C00006,C00101) VALUES (?,?,?,?)",
                            new OleDbParameter[]{
                            new OleDbParameter("@C00004",systemid),
                            new OleDbParameter("@C00005",""),
                            new OleDbParameter("@C00006",_name),
                            new OleDbParameter("@C00101",_id)
                        });
                        jclass.Msg = "恭喜你！添加分类成功";
                    }
                    jclass.Status = "Success";

                }
            }
            catch (Exception ex)
            {
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