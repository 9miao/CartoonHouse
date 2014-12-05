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
    public partial class downclassify : PageBase
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
            string _id=Request.QueryString["id"];
            int page = getRequestPage();
            using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                int pageCount = 0;
                int RowsCount = 0;
                using (_dt = exPagers.ExecutePager(page, Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["PageSize"]), "C00004", "*", "select * from T0001", " where C00101=" + _id + "", "C00004 asc", out pageCount, out RowsCount))
                {
                    PageInfo pi = new PageInfo();
                    pi.CurrPageIndex = Convert.ToInt32(page);       //当前页码
                    pi.PageCount = Convert.ToInt32(pageCount);   //页数
                    pi.PageSize = Convert.ToInt32(System.Configuration.ConfigurationManager.AppSettings["PageSize"]);   //每页显示
                    pi.RecordCount = RowsCount;   //总数
                    this.PageInfoControl.i = pi;
                    this.PageInfoControl.URL = "../pages/downclassify.aspx?value=" + page + "&id=" + Request.QueryString["id"]+"&";
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
    }
}