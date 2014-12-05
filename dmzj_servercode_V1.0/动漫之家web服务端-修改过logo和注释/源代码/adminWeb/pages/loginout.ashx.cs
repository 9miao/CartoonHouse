using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using System.Web.SessionState;
namespace adminWeb.pages
{
    /// <summary>
    /// loginout 的摘要说明
    /// </summary>
    public class loginout : IHttpHandler, IRequiresSessionState
    {

        public void ProcessRequest(HttpContext context)
        {
            context.Session.Abandon();
            context.Response.Redirect("../login.aspx");
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