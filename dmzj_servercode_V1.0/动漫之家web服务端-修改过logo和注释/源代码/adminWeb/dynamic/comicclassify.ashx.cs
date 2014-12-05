using BBDCL;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Data.OleDb;
using CartoonClass;

namespace adminWeb.dynamic
{
    /// <summary>
    /// comicclassify 的摘要说明
    /// </summary>
    public class comicclassify : IHttpHandler
    {

        /// <summary>
        /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
        /// </summary>
        /// 



        public void ProcessRequest(HttpContext context)
        {
            string classify_type = context.Request["classify_type"];         //字母
            string classify_id = context.Request["classify_id"];             //小分类id
            string page = context.Request["page"];                           //页码
            int pages = 0;
            int.TryParse(page, out pages);
            pages = pages+1;
            LetterComic lc = new LetterComic();
            lc.result = 1;
            lc.msg = "OK";
            try
            {
                using (
                                 CDBAccess cn =
                                     new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                {
                    int check_classify_type = 0;
                    int.TryParse(classify_type, out check_classify_type);
                    string sql_Big = string.Empty;
                    OleDbParameter[] ole_big = new OleDbParameter[] { };
                    if (classify_type != "update")
                    {
                        if (check_classify_type > 0)
                        {
                            //说明是INT型的分类
                            sql_Big = "select * from T0000,T0001 where C00001=C00101 and C00001=" + classify_type + " and C00004='" + classify_id + "'";
                        }
                        else
                        {
                            //字母型
                            sql_Big = "select * from T0000,T0001 where C00001=C00101 and C00002 like '%" + classify_type + "%' and C00004='" + classify_id + "'";
                        }
                    }
                    else { 
                        sql_Big="select * from T0000,T0001 where C00001=C00101 and C00004='" + classify_id + "'";
                    }

                    //先验证字母 跟小分类id  
                    using (DataTable _dt = cn.ExecQuery(sql_Big))
                    {
                        if (_dt.Rows.Count > 0)
                        {
                            string sql = string.Empty;
                            OleDbParameter[] ole = new OleDbParameter[] { };
                            if (pages == 1)
                            {
                                //第一页
                                sql = "select top " + 30 + " * from T0003,T0002 where C00009=C00010 and C00008='" + classify_id + "' order by C00010 desc";
                            }
                            else
                            {
                                //N页
                                sql = "select top " + 30 + " * from T0003,T0002 where C00009=C00010 and C00008='" + classify_id + "' and C00010 not in " +
    @"(select top " + ((pages - 1) * 1) + " C00010 from T0003,T0002 where C00009=C00010 and C00008='" + classify_id + "' order by C00010 desc)"
       + "order by C00010 desc";
                            }
                            //进行分页寻找返回列表
                            using (DataTable _dtList = cn.ExecQuery(sql))
                            {
                                ComicList[] cmlist = new ComicList[_dtList.Rows.Count];
                                for (int i = 0; i < _dtList.Rows.Count; i++)
                                {
                                    ComicList clist = new ComicList();
                                    clist.id = _dtList.Rows[i]["C00010"].ToString();
                                    clist.cover = _dtList.Rows[i]["C00013"].ToString();
                                    clist.hot_hits = _dtList.Rows[i]["C00015"].ToString();
                                    clist.last_update_chapter_name = Coding.ToUnicode(_dtList.Rows[i]["C00012"].ToString());
                                    clist.last_updatetime = _dtList.Rows[i]["C00014"].ToString();
                                    clist.title = Coding.ToUnicode(_dtList.Rows[i]["C00011"].ToString());
                                    cmlist[i] = clist;
                                }
                                lc.data = cmlist;
                            }
                        }
                    }
                }
            }
            catch
            {

            }
            context.Response.ContentType = "text/plain";
            context.Response.Write(JsonSerialize.jSerialize(lc));
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