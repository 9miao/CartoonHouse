using adminWeb.adminClass;
using BBDCL;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace adminWeb.pages
{
    public partial class chap : PageBase
    {
        public DataTable _dt = null;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                InitData();
            }
        }

        void InitData()
        {
            int page = getRequestPage();
            string id = Request.QueryString["id"];
            if (string.IsNullOrEmpty(id))
            {
                Response.Redirect("comic.aspx");
                return;
            }
            using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                int pageCount = 0;
                int RowsCount = 0;
                string sqlWhere = " where cstr(C00030)='" + id + "'";
                string kw = Request.QueryString["kw"];
                if (!string.IsNullOrEmpty(kw))
                {
                    sqlWhere = " where cstr(C00030)='" + id + "' and C00031 like '%" + kw + "%'";
                    Textserch.Text = kw;
                }
                using (_dt = exPagers.ExecutePager(page, Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["PageSize"]), "cstr(C00029)", "*", "select * from T0005  ", sqlWhere, "C00029 desc", out pageCount, out RowsCount))
                {
                    PageInfo pi = new PageInfo();
                    pi.CurrPageIndex = Convert.ToInt32(page);       //当前页码
                    pi.PageCount = Convert.ToInt32(pageCount);   //页数
                    pi.PageSize = Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["PageSize"]);   //每页显示
                    pi.RecordCount = RowsCount;   //总数
                    this.PageInfoControl.i = pi;
                    this.PageInfoControl.URL = "../pages/chap.aspx?value=" + page + "&";
                    this.PageInfoControl.initPage();
                }
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
            Response.Redirect("chap.aspx?id="+Request.QueryString["id"]+"&kw=" + System.Web.HttpUtility.UrlEncode(Textserch.Text.Trim(), System.Text.Encoding.UTF8));
        }
    }
}