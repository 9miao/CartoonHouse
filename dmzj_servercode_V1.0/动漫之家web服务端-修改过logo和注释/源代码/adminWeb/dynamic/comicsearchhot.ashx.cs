using BBDCL;
using CartoonClass;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace adminWeb.dynamic
{
    /// <summary>
    /// comicsearchhot 的摘要说明  热门数据
    /// </summary>
    public class comicsearchhot : IHttpHandler
    {


        /// <summary>
        /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
        /// </summary>
        /// 




        public void ProcessRequest(HttpContext context)
        {

            comicsearchhots chots = new comicsearchhots();
            chots.result = 1;
            chots.msg = "OK";
            try
            {

                using (
                                  CDBAccess cn =
                                      new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                {
                    using (DataTable _dt = cn.ExecQuery(@"SELECT top 20 * FROM T0003 ORDER BY C00010 DESC"))
                    {
                        comicsearchhotData[] comDataList = new comicsearchhotData[_dt.Rows.Count];
                        for (int i = 0; i < _dt.Rows.Count; i++)
                        {
                            comicsearchhotData cData = new comicsearchhotData();
                            cData.id = _dt.Rows[i]["C00010"].ToString();
                            cData.name = Coding.ToUnicode(_dt.Rows[i]["C00011"].ToString());
                            comDataList[i] = cData;
                        }
                        chots.data = comDataList;
                    }
                }
            }
            catch
            {

            }
            context.Response.ContentType = "text/plain";
            context.Response.Write(JsonSerialize.jSerialize(chots));
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