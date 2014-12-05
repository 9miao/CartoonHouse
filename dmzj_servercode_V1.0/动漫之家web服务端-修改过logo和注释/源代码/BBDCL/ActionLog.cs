using System;
using System.Collections.Generic;
using System.Text;
using BBDCL;
using System.Data;
using System.Data.OleDb;
namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


  public class ActionLog
    {
      public static string Adress(string userip)
        {
            string useradress;
            DataTable dt;
            using (CDBAccess cn1 = new CDBAccess(System.Configuration.ConfigurationManager.AppSettings["OledbConStrConfig"]))
            {
                using (dt = cn1.ExecQuery("select Loc1 from IpRegion where IntIp=dbo.IP2Int(?)", new OleDbParameter[] { new OleDbParameter("@p", userip) }))
                {
                    if (dt.Rows.Count > 0)
                    {
                        useradress = dt.Rows[0][0].ToString();
                    }
                    else
                    {
                        useradress = "未知区域";
                    }
                }

            }

            return useradress;
        }

      public static void operationlog(string username, string useraction, string useridpic, string usertype, string user_ip)
      {
          using (CDBAccess cn1 = new CDBAccess(System.Configuration.ConfigurationManager.AppSettings["OledbConStrLog"]))
          {
              //记录用户登录日志
             // string username = dt.Rows[0]["user_id"].ToString();
              //string useraction = "用户登录";
              //string useridpic = dt.Rows[0]["ad_pic"].ToString();
              //string usertype = "True";
              //string userip = "122.0.66.184";
              string address = Adress(user_ip);

              Dictionary<string, string> dc1 = new Dictionary<string, string>();
              dc1.Add("userid", username);
              dc1.Add("user_action", useraction);
              dc1.Add("user_type", usertype);
              dc1.Add("user_adpic", useridpic);
              dc1.Add("user_ip", user_ip);
              dc1.Add("user_area", address);
              dc1.Add("user_datetime", DateTime.Now.ToString());
              cn1.ExecSQL("insert into _user_action_log(user_id,user_action,user_type,user_adpic,user_ip,user_area,user_datetime) values(?,?,?,?,?,?,?)", cn1.sqlparameter_append(dc1));

          }
      }

    }
}
