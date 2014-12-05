using BBDCL;
using CartoonClass;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace adminWeb
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    /// <summary>
    /// top 的摘要说明
    /// </summary>
    public class top : IHttpHandler {


        public void ProcessRequest(HttpContext context)
        {
            using (
                             CDBAccess cn =
                                 new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                Ctop cp = new Ctop();
                using (DataTable _dt = cn.ExecQuery("select top 10 * from T0004 ORDER BY C00016 DESC"))
                {
                    Topdata[] TD = new Topdata[_dt.Rows.Count];
                    for (int i = 0; i < _dt.Rows.Count; i++)
                    {
                        Topdata tdrow = new Topdata();
                        tdrow.authors = Coding.ToUnicode(_dt.Rows[i]["C00025"].ToString());
                        tdrow.cover = _dt.Rows[i]["C00024"].ToString();
                        tdrow.id = _dt.Rows[i]["C00016"].ToString();
                        tdrow.last_update_chapter_name = Coding.ToUnicode(_dt.Rows[i]["C00022"].ToString());
                        tdrow.sum_chapters = "100";
                        tdrow.sum_source = "101";
                        tdrow.title = Coding.ToUnicode(_dt.Rows[i]["C00017"].ToString());
                        tdrow.types = Coding.ToUnicode(_dt.Rows[i]["C00019"].ToString());
                        TD[i] = tdrow;
                    }
                    cp.month = TD;
                    cp.total = TD;
                    cp.week = TD;
                }
                context.Response.Write(JsonSerialize.jSerialize(cp));
            }
        }
     
        public bool IsReusable {
            get {
                return false;
            }
        }
    }
}