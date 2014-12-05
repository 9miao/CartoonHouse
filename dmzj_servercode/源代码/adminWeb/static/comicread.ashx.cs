using BBDCL;
using CartoonClass;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Data.OleDb;
namespace adminWeb
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    /// <summary>
    /// comicread 的摘要说明  根据 动漫ID 话ID 获取信息
    /// </summary>
    public class comicread : IHttpHandler {


        public void ProcessRequest(HttpContext context)
        {
            string comic_id = context.Request["comic_id"];          //动漫ID
            string chap_id = context.Request["chap_id"];            //话id
            using (
                             CDBAccess cn =
                                 new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                ChapInforMation chapIn = new ChapInforMation();

                //获取话id
                using (DataTable _dt = cn.ExecQuery(@"select * from  T0005
	where C00029=? and C00030=?",
                                new OleDbParameter[]{
                                    new OleDbParameter("@C00029",chap_id),
                                    new OleDbParameter("@C00030",comic_id)
                                }))
                {
                    if (_dt.Rows.Count > 0)
                    {
                        DataRow dr = _dt.Rows[0];
                        chapIn.id = dr["C00029"].ToString();
                        chapIn.chapter_name = Coding.ToUnicode(dr["C00031"].ToString());
                        chapIn.chapter_order = dr["C00032"].ToString();
                        chapIn.comic_id = dr["C00030"].ToString();
                    }
                }

                //获取
                using (DataTable _dt = cn.ExecQuery(@"Select * from T0005,T0006 WHERE C00029=C00035
		AND T0005.C00029=? and T0005.C00030=?
			order by C00034 asc",
                                new OleDbParameter[]{
                                    new OleDbParameter("@C00029",chap_id),
                                    new OleDbParameter("@C00030",comic_id)
                                }))
                {
                    string[] pages = new string[_dt.Rows.Count];
                    for (int i = 0; i < _dt.Rows.Count; i++)
                    {
                        pages[i] = _dt.Rows[i]["C00036"].ToString();
                    }
                    chapIn.page = pages;
                }
                context.Response.ContentType = "text/plain";
                context.Response.Write(JsonSerialize.jSerialize(chapIn));
            }
        }
        public bool IsReusable {
            get {
                return false;
            }
        }
    }
}