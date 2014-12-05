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
    /// classify 的摘要说明 获取分类数据
    /// </summary>
    public class classify : IHttpHandler {


        public void ProcessRequest(HttpContext context)
        {
            using (
                             CDBAccess cn =
                                 new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                //获取大分类的信息
                using (DataTable _dtBig = cn.ExecQuery(@"select * from T0000 order by C00001 asc"))
                {

                    TreeListing[] tl = new TreeListing[_dtBig.Rows.Count];
                    for (int i = 0; i < _dtBig.Rows.Count; i++)
                    {
                        TreeListing tlb = new TreeListing();
                        tlb.id = _dtBig.Rows[i]["C00001"].ToString();
                        tlb.title = _dtBig.Rows[i]["C00003"].ToString();
                        tlb.icon = _dtBig.Rows[i]["C00002"].ToString();
                        using (DataTable _dtSmall = cn.ExecQuery(@"select * from T0001 where C00101=? order by C00004",
                            new OleDbParameter[]{
                                new OleDbParameter("@a",_dtBig.Rows[i]["C00001"].ToString())
                            }))
                        {
                            TreeListing[] tls = new TreeListing[_dtSmall.Rows.Count];
                            for (int j = 0; j < _dtSmall.Rows.Count; j++)
                            {
                                TreeListing tlsRow = new TreeListing();
                                tlsRow.id = _dtSmall.Rows[j]["C00004"].ToString();
                                tlsRow.icon = _dtSmall.Rows[j]["C00005"].ToString();
                                tlsRow.title = _dtSmall.Rows[j]["C00006"].ToString();
                                tls[j] = tlsRow;
                            }
                            tlb.subnodes = tls;
                        }
                        tl[i] = tlb;
                    }
                    //信息填充后 json 序列化返回
                    context.Response.Write(JsonSerialize.jSerialize(tl));
                }
            }
        }
     
        public bool IsReusable {
            get {
                return false;
            }
        }
    }
}