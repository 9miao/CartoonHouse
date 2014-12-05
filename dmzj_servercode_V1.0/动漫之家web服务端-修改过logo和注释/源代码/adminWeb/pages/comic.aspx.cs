using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.OleDb;
using BBDCL;
using adminWeb.adminClass;
namespace adminWeb.pages
{
    public partial class comic : PageBase
    {
        public DataTable _dt = null;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack) {
                InitData();
            }
        }

        void InitData() {
            try
            {
                int page = getRequestPage();
                using (
                                         CDBAccess cn =
                                             new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                {
                    string classifyid = Request.QueryString["id"];

                    string sql = string.Empty;
                    string sqlWhere = string.Empty;
                    if (!string.IsNullOrEmpty(classifyid))
                    {
                        sql = @"select * from T0002,T0003 where C00010=C00009 and cstr(C00008)='" + classifyid + "'";
                    }
                    else
                    {
                        sql = @"select * from T0003";
                    }
                    int pageCount = 0;
                    int RowsCount = 0;
                    string kw = Request.QueryString["kw"];
                    if (!string.IsNullOrEmpty(kw))
                    {
                        sqlWhere = " where C00011 like '%" + kw + "%'";
                        Textserch.Text = kw;
                    }
                    using (_dt = exPagers.ExecutePager(page, Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["PageSize"]), "cstr(C00010)", "*", sql, sqlWhere, "C00014 desc", out pageCount, out RowsCount))
                    {
                        PageInfo pi = new PageInfo();
                        pi.CurrPageIndex = Convert.ToInt32(page);       //当前页码
                        pi.PageCount = Convert.ToInt32(pageCount);   //页数
                        pi.PageSize = Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["PageSize"]);   //每页显示
                        pi.RecordCount = RowsCount;   //总数
                        this.PageInfoControl.i = pi;
                        this.PageInfoControl.URL = "../pages/comic.aspx?value=" + page + "&kw=" + kw + "&id=" + classifyid+"&";
                        this.PageInfoControl.initPage();
                    }
                }
            }
            catch {
                Response.Redirect("comic.aspx");
            }
        }
        /// <summary>
        /// 获取当前页码
        /// </summary>
        /// <returns>返回url页码</returns>
        int getRequestPage()
        {
            string pages = Request.QueryString["page"];     //url 页码
            if (string.IsNullOrEmpty(pages))
                pages = "1";
            int page = 1;
            int.TryParse(pages, out page);
            return page;
        }

        protected void BtnSerch_Click(object sender, EventArgs e)
        {
            Response.Redirect("comic.aspx?kw=" + System.Web.HttpUtility.UrlEncode(Textserch.Text.Trim(), System.Text.Encoding.UTF8));
        }
    }
}