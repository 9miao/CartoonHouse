using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using BBDCL;
using System.Data;
using System.Data.OleDb;
using System.Text;
using CartoonClass;

namespace adminWeb
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    /// <summary>
    /// index 的摘要说明  首页信息
    /// </summary>
    public class index : IHttpHandler {

     
        public void ProcessRequest(HttpContext context)
        {

            using (
                              CDBAccess cn =
                                  new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                CIndexData cdata = new CIndexData();

                //首页大图信息图片
                using (DataTable _dtInfo = cn.ExecQuery(@"SELECT top 1 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    headlinesInfo hinfo = new headlinesInfo();
                    hinfo.id = _dtInfo.Rows[0]["C00016"].ToString();
                    hinfo.subtitle = Coding.ToUnicode(_dtInfo.Rows[0]["C00018"].ToString());
                    hinfo.title = Coding.ToUnicode(_dtInfo.Rows[0]["C00017"].ToString());
                    hinfo.cover = _dtInfo.Rows[0]["C00024"].ToString();
                    cdata.headlines = hinfo;
                }
                //首页信息
                using (DataTable _dtepList = cn.ExecQuery(@"SELECT top 15 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    episodeList[] _dtepStr = new episodeList[_dtepList.Rows.Count];
                    for(int i=0;i<_dtepList.Rows.Count;i++)
                    {
                        episodeList episRow = new episodeList();
                        episRow.id = _dtepList.Rows[i]["C00016"].ToString();
                        episRow.title = Coding.ToUnicode(_dtepList.Rows[i]["C00017"].ToString());
                        episRow.last_update_chapter_name = Coding.ToUnicode(_dtepList.Rows[i]["C00022"].ToString());
                        episRow.cover = _dtepList.Rows[i]["C00024"].ToString();
                        _dtepStr[i] = episRow;
                    }
                    cdata.episode = _dtepStr;
                }

                //连载
                CIndexData cdata1 = new CIndexData();
                using (DataTable _dtInfo = cn.ExecQuery(@"SELECT top 1 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    headlinesInfo hinfo = new headlinesInfo();
                    hinfo.id = _dtInfo.Rows[0]["C00016"].ToString();
                    hinfo.subtitle = Coding.ToUnicode(_dtInfo.Rows[0]["C00018"].ToString());
                    hinfo.title = Coding.ToUnicode(_dtInfo.Rows[0]["C00017"].ToString());
                    hinfo.cover = _dtInfo.Rows[0]["C00024"].ToString();
                    cdata1.headlines = hinfo;
                }
                //连载
                using (DataTable _dtepList = cn.ExecQuery(@"SELECT top 15 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    episodeList[] _dtepStr = new episodeList[_dtepList.Rows.Count];
                    for (int i = 0; i < _dtepList.Rows.Count; i++)
                    {
                        episodeList episRow = new episodeList();
                        episRow.id = _dtepList.Rows[i]["C00016"].ToString();
                        episRow.title = Coding.ToUnicode(_dtepList.Rows[i]["C00017"].ToString());
                        episRow.last_update_chapter_name = Coding.ToUnicode(_dtepList.Rows[i]["C00022"].ToString());
                        episRow.cover = _dtepList.Rows[i]["C00024"].ToString();
                        _dtepStr[i] = episRow;
                    }
                    cdata1.episode = _dtepStr;
                }

                //最新
                CIndexData cdata2 = new CIndexData();
                using (DataTable _dtInfo = cn.ExecQuery(@"SELECT top 1 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    headlinesInfo hinfo = new headlinesInfo();
                    hinfo.id = _dtInfo.Rows[0]["C00016"].ToString();
                    hinfo.subtitle = Coding.ToUnicode(_dtInfo.Rows[0]["C00018"].ToString());
                    hinfo.title = Coding.ToUnicode(_dtInfo.Rows[0]["C00017"].ToString());
                    hinfo.cover = _dtInfo.Rows[0]["C00024"].ToString();
                    cdata2.headlines = hinfo;
                }
                using (DataTable _dtepList = cn.ExecQuery(@"SELECT top 15 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    episodeList[] _dtepStr = new episodeList[_dtepList.Rows.Count];
                    for (int i = 0; i < _dtepList.Rows.Count; i++)
                    {
                        episodeList episRow = new episodeList();
                        episRow.id = _dtepList.Rows[i]["C00016"].ToString();
                        episRow.title = Coding.ToUnicode(_dtepList.Rows[i]["C00017"].ToString());
                        episRow.last_update_chapter_name = Coding.ToUnicode(_dtepList.Rows[i]["C00022"].ToString());
                        episRow.cover = _dtepList.Rows[i]["C00024"].ToString();
                        _dtepStr[i] = episRow;
                    }
                    cdata2.episode = _dtepStr;
                }
                //完结
                CIndexData cdata3 = new CIndexData();
                using (DataTable _dtInfo = cn.ExecQuery(@"SELECT top 1 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    headlinesInfo hinfo = new headlinesInfo();
                    hinfo.id = _dtInfo.Rows[0]["C00016"].ToString();
                    hinfo.subtitle = Coding.ToUnicode(_dtInfo.Rows[0]["C00018"].ToString());
                    hinfo.title = Coding.ToUnicode(_dtInfo.Rows[0]["C00017"].ToString());
                    hinfo.cover = _dtInfo.Rows[0]["C00024"].ToString();
                    cdata3.headlines = hinfo;
                }
                using (DataTable _dtepList = cn.ExecQuery(@"SELECT top 15 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    episodeList[] _dtepStr = new episodeList[_dtepList.Rows.Count];
                    for (int i = 0; i < _dtepList.Rows.Count; i++)
                    {
                        episodeList episRow = new episodeList();
                        episRow.id = _dtepList.Rows[i]["C00016"].ToString();
                        episRow.title = Coding.ToUnicode(_dtepList.Rows[i]["C00017"].ToString());
                        episRow.last_update_chapter_name = Coding.ToUnicode(_dtepList.Rows[i]["C00022"].ToString());
                        episRow.cover = _dtepList.Rows[i]["C00024"].ToString();
                        _dtepStr[i] = episRow;
                    }
                    cdata3.episode = _dtepStr;
                }
                //原创
                CIndexData cdata4 = new CIndexData();
                using (DataTable _dtInfo = cn.ExecQuery(@"SELECT top 1 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    headlinesInfo hinfo = new headlinesInfo();
                    hinfo.id = _dtInfo.Rows[0]["C00016"].ToString();
                    hinfo.subtitle = Coding.ToUnicode(_dtInfo.Rows[0]["C00018"].ToString());
                    hinfo.title = Coding.ToUnicode(_dtInfo.Rows[0]["C00017"].ToString());
                    hinfo.cover = _dtInfo.Rows[0]["C00024"].ToString();
                    cdata4.headlines = hinfo;
                }
                using (DataTable _dtepList = cn.ExecQuery(@"SELECT top 15 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                {
                    episodeList[] _dtepStr = new episodeList[_dtepList.Rows.Count];
                    for (int i = 0; i < _dtepList.Rows.Count; i++)
                    {
                        episodeList episRow = new episodeList();
                        episRow.id = _dtepList.Rows[i]["C00016"].ToString();
                        episRow.title = Coding.ToUnicode(_dtepList.Rows[i]["C00017"].ToString());
                        episRow.last_update_chapter_name = Coding.ToUnicode(_dtepList.Rows[i]["C00022"].ToString());
                        episRow.cover = _dtepList.Rows[i]["C00024"].ToString();
                        _dtepStr[i] = episRow;
                    }
                    cdata4.episode = _dtepStr;
                }


                CIndex cdex = new CIndex();
                cdex.index = cdata;
                cdex.lianzai = cdata1;
                cdex.new_key_huan = cdata2;
                cdex.wanjie = cdata3;
                cdex.yuanchuang = cdata4;
                string json_callback = JsonSerialize.jSerialize(cdex);
                context.Response.Write(json_callback.Replace(@"new_key_huan", "new"));
            }
        }
     
        public bool IsReusable {
            get {
                return false;
            }
        }
    }
}