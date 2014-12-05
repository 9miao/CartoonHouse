using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;

namespace adminWeb.adminClass
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class PageBase:Page
    {
        public UserLogin login;
        protected override void OnInit(EventArgs e)
        {
            if (System.Web.HttpContext.Current != null)
            {

                if (Session["UserLogin"] != null)
                {
                    login = (UserLogin)Session["UserLogin"];
                }
                else
                {
                    Response.Redirect("../login.aspx");
                }
                base.OnInit(e);
            }
        }
    }
}