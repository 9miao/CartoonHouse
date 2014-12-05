using adminWeb.adminClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace adminWeb.pages
{
    public partial class Site1 : System.Web.UI.MasterPage
    {
        public UserLogin login;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack) {
                if (Session["UserLogin"] != null)
                {
                    login = (UserLogin)Session["UserLogin"];
                }
                else
                {
                    Response.Redirect("../login.aspx");
                }
            }
        }
    }
}