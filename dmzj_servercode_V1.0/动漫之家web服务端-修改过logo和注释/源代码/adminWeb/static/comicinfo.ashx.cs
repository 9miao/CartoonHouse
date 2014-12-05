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
    /// comicinfo 的摘要说明
    /// </summary>
    public class comicinfo : IHttpHandler {

      
        public void ProcessRequest (HttpContext context) 
        {
            //获取漫画基本信息
            string comic_id = context.Request.QueryString["comic_id"];  //漫画id
            //数据库的连接字符串
            using (
                             CDBAccess cn =
                                 new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                //返回基本信息类
                ComicInforMation cimn = new ComicInforMation();
                cimn.result = 1;
                cimn.msg = "OK";
                try
                {
                    Comicdata cdata = new Comicdata();      //数据集data
                    InforMation imn = new InforMation();    //基本信息实体类
                    //根据参数查询出基本信息
                    using (DataTable _dtInforMation = cn.ExecQuery("SELECT * FROM T0004 WHERE C00016=?",
                        new OleDbParameter[]{
                        new OleDbParameter("@C00016",comic_id)
                    }))
                    {
                        if (_dtInforMation.Rows.Count > 0)
                        {
                            DataRow _drInforMation = _dtInforMation.Rows[0];
                            imn.id = _drInforMation["C00016"].ToString();
                            imn.last_update_chapter_name = Coding.ToUnicode(_drInforMation["C00022"].ToString());
                            imn.last_updatetime = _drInforMation["C00023"].ToString();
                            imn.status = Coding.ToUnicode(_drInforMation["C00021"].ToString());
                            imn.subtitle = Coding.ToUnicode(_drInforMation["C00018"].ToString());
                            imn.title = Coding.ToUnicode(_drInforMation["C00017"].ToString());
                            imn.types = Coding.ToUnicode(_drInforMation["C00019"].ToString());
                            imn.zone = Coding.ToUnicode(_drInforMation["C00020"].ToString());
                            imn.authors = Coding.ToUnicode(_drInforMation["C00025"].ToString());
                            imn.cover = _drInforMation["C00024"].ToString();
                            imn.description = Coding.ToUnicode(_drInforMation["C00026"].ToString());
                            imn.direction = _drInforMation["C00028"].ToString();
                            imn.first_letter = _drInforMation["C00027"].ToString();
                        }
                        else {
                            throw new Exception();
                        }
                    }
                    cdata.info = imn;       //赋值基本信息
                    //赋值话信息
                    using (DataTable _dtword = cn.ExecQuery("select * from T0005 where C00030=? order by C00029 desc",
                        new OleDbParameter[]{
                        new OleDbParameter("@id",comic_id)
                    }))
                    {
                        wordList[] wlists = new wordList[_dtword.Rows.Count];
                        for (int i = 0; i < _dtword.Rows.Count; i++)
                        {
                            wordList wlistRow = new wordList();
                            wlistRow.id = _dtword.Rows[i]["C00029"].ToString();
                            wlistRow.filesize = _dtword.Rows[i]["C00033"].ToString();
                            wlistRow.comic_id = _dtword.Rows[i]["C00030"].ToString();
                            wlistRow.chapter_order = _dtword.Rows[i]["C00032"].ToString();
                            wlistRow.chapter_name = Coding.ToUnicode(_dtword.Rows[i]["C00031"].ToString());
                            wlists[i] = wlistRow;
                        }
                        cdata.list = wlists;
                    }


                    aloneList[] alist = new aloneList[0];
                    cdata.alone = alist;
                    //赋值similarList关联信息
                    using (DataTable _dtsimilar = cn.ExecQuery("SELECT top 10 * FROM T0004 ORDER BY rnd(C00016) DESC"))
                    {
                        similarList[] similar = new similarList[_dtsimilar.Rows.Count];
                        for (int i = 0; i < _dtsimilar.Rows.Count; i++)
                        {
                            similarList similarRow = new similarList();
                            similarRow.id = _dtsimilar.Rows[i]["C00016"].ToString();
                            similarRow.cover = _dtsimilar.Rows[i]["C00024"].ToString();
                            similarRow.last_update_chapter_name = Coding.ToUnicode(_dtsimilar.Rows[i]["C00022"].ToString());
                            similarRow.title = Coding.ToUnicode(_dtsimilar.Rows[i]["C00017"].ToString());
                            similar[i] = similarRow;
                        }
                        cdata.similar = similar;
                    }
                    cimn.data = cdata;
                }
                catch { 
                    
                }
                context.Response.ContentType = "text/plain";
                context.Response.Write(JsonSerialize.jSerialize(cimn));
            }
        }
     
        public bool IsReusable {
            get {
                return false;
            }
        }
    }
}