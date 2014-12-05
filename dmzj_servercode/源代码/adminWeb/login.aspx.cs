using BBDCL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.OleDb;
using adminWeb.adminClass;
namespace adminWeb
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public partial class login : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (IsPostBack) { 
                
            }
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            try
            {
                string username = TextBox1.Text.Trim();
                string pwd = TextBox2.Text.Trim();
                if (string.IsNullOrEmpty(username) || string.IsNullOrEmpty(pwd))
                    throw new Exception("请填写用户名密码");
                    using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
                    {
                        if (cn.HasRecord(@"select * from AdminUser where username=? and password=?",
                            new OleDbParameter[]{
                                new OleDbParameter("@usd",username),
                                new OleDbParameter("@pwd",pwd)
                            }))
                        {
                            UserLogin ul = new UserLogin();
                            ul.UserName = username;
                            ul.PassWord = pwd;
                            Session["UserLogin"] = ul;
                            Response.Redirect("pages/classify.aspx");
                        }
                        else {
                            throw new Exception("密码不正确");
                        }
                    }
            }
            catch (Exception ex){
                Page.ClientScript.RegisterClientScriptBlock(this.GetType(), "test", "<script>layer.alert('" + ex.Message + "', 8);</script>");
            }
        }
    }
}