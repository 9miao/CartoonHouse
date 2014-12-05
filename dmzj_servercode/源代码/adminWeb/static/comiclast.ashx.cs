using BBDCL;
using CartoonClass;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Linq;
using System.Web;

namespace adminWeb
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    /// <summary>
    /// comiclast 的摘要说明  根据ID 获取上一话的信息
    /// </summary>
    public class comiclast : IHttpHandler {

        public void ProcessRequest(HttpContext context)
        {
            string comic_id = context.Request["comic_id"];      //漫画id
            string chap_id = context.Request["chap_id"];        //章节
            using (
                             CDBAccess cn =
                                 new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                ChapInforMation chapIn = new ChapInforMation();

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
                        chapIn.chapter_name = Coding.ToUnicode(dr["C00031"].ToString());
                        chapIn.chapter_order = dr["C00032"].ToString();
                        chapIn.comic_id = dr["C00030"].ToString();
                    }
                }



                string h_id = chap_id;         //上一张
                using (DataTable _dt = cn.ExecQuery(@"select top 1 C00029 from T0005 where C00030=" + comic_id + " and C00029 < " + chap_id + " order by C00029 desc"))
                {
                    if (_dt.Rows.Count > 0)
                    {
                        h_id = _dt.Rows[0]["C00029"].ToString();
                        chapIn.id = h_id;
                        //根据话id 查找
                        using (DataTable _dts = cn.ExecQuery(@"Select * from T0006 WHERE C00035=?
			order by C00034 asc",
                                        new OleDbParameter[]{
                                    new OleDbParameter("@C00035",h_id)
                                }))
                        {
                            string[] pages = new string[_dts.Rows.Count];
                            for (int i = 0; i < _dts.Rows.Count; i++)
                            {
                                pages[i] = _dts.Rows[i]["C00036"].ToString();
                            }
                            chapIn.page = pages;
                        }
                    }
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