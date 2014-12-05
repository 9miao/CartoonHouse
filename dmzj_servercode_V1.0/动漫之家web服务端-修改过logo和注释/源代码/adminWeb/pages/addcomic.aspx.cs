using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;
using System.Data.OleDb;
using BBDCL;
using adminWeb.adminClass;
namespace adminWeb.pages
{
    public partial class addcomic : PageBase
    {
        public DataTable dttype = null, thisdttypes,dt01;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
                BindData();
        }

        void BindData()
        {
            using (CDBAccess cn = new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                dt01 = cn.ExecQuery("select * from T0000");
                dttype = cn.ExecQuery("select * from T0001");
                string id = Request.QueryString["id"];
                if (!string.IsNullOrEmpty(id))
                {

                    //当前漫画对应的多个分类
                    using (DataTable dt3 = cn.ExecQuery("select * from T0003 where C00010 = " + id + ""))
                    {

                        thisdttypes = cn.ExecQuery("select * from T0002  where C00009 = " + id + "");
                        if (dt3.Rows.Count > 0)
                            TextBox1.Text = dt3.Rows[0]["C00011"].ToString();

                        using (DataTable dt = cn.ExecQuery("select * from T0004 where C00016 = " + id + ""))
                        {
                            if (dt.Rows.Count > 0)
                            {
                                TextBox2.Text = dt.Rows[0]["C00018"].ToString();
                                TextBox4.Text = dt.Rows[0]["C00024"].ToString();
                                TextBox5.Text = dt.Rows[0]["C00025"].ToString();
                                TextBox6.Text = dt.Rows[0]["C00026"].ToString();
                            }
                        }
                    }
                }
                else
                    //当前漫画对应的多个分类
                    thisdttypes = cn.ExecQuery("select * from T0002  where C00009 = -1 ");

            }
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            string id = Request.QueryString["id"];
            string name = TextBox1.Text;
            string jianjie = TextBox2.Text;
            string leixing = Request.Form["checkname"];
            string tpdizhi = TextBox4.Text;
            string zuozhe = TextBox5.Text;
            string jieshao = TextBox6.Text;
            using (CDBAccess cn = new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {

                string Tleixing = "";
                string[] T=null;
                if(!string.IsNullOrEmpty(leixing))
                {
                 T = leixing.Split(',');
                string Tsql = "";
                if (T.Length > 0)
                {
                    foreach (string s in T)
                    {
                        Tsql += " C00004 = '" + s + "' or";
                    }
                    Tsql = Tsql.Substring(0, Tsql.LastIndexOf("or"));
                    using (DataTable dt = cn.ExecQuery("select * from T0001 where "+Tsql))
                    {
                        foreach (DataRow dr in dt.Rows)
                        {
                            Tleixing += dr[2] + "/";
                        }
                        if (dt.Rows.Count > 0)
                            Tleixing = Tleixing.Substring(0, Tleixing.LastIndexOf("/"));
                    }
                }
                }

                if (string.IsNullOrEmpty(id))
                {
                    Dictionary<string, string> dc = new Dictionary<string, string>();
                    int iid = int.Parse(cn.ExecQuery("select top 1 C00010 from T0003 order by C00010 desc").Rows[0][0].ToString()) + 1;
                    dc.Add("id", iid.ToString());
                    dc.Add("name", name);
                    dc.Add("jianjie", jianjie);
                    dc.Add("tpdizhi", tpdizhi);
                    dc.Add("zuozhe", zuozhe);
                    dc.Add("jieshao", jieshao);
                    dc.Add("tm", exPagers.getSecondEnd(DateTime.Now).ToString());
                    
                    //添加
                    string sql = "insert into T0004(C00016,C00017,C00018,C00024,C00025,C00026,C00028,C00023) values (@id,@name,@jianjie,@tpdizhi,@zuozhe,@jieshao,1,@tm)";
                    cn.ExecSQL("insert into T0003 (C00010,C00011,C00013,C00012,C00014) values (@p1,@p2,@p3,@p4,@p5)", new OleDbParameter[] { new OleDbParameter("@p1", iid), new OleDbParameter("@p2", name), new OleDbParameter("@p3", tpdizhi), new OleDbParameter("@p4", exPagers.getSecondEnd(DateTime.Now)), new OleDbParameter("@p5", exPagers.getSecondEnd(DateTime.Now)) });
                    cn.ExecQuery(sql, cn.sqlparameter_append(dc));
                    
                //    string Tid = cn.ExecQuery("select C00016 from T0004 where C00017 = @name", new OleDbParameter[] { new OleDbParameter("@name", name) }).Rows[0][0].ToString();
                    string insertsql = "insert into T0002(C00008,C00009) values (@p2,@p3)";
                    if(T!=null)
                    foreach (string s in T)
                    {
                        cn.ExecSQL(insertsql, new OleDbParameter[] { new OleDbParameter("@p2", s), new OleDbParameter("@p3", iid)});
                    }
                    Response.Write("<script>alert('添加成功');window.location.href='addcomic.aspx'</script>");
                }
                else
                {
                    //修改
                    Dictionary<string, string> dc = new Dictionary<string, string>();
                    dc.Add("name", name);
                    dc.Add("jianjie", jianjie);
                    dc.Add("tpdizhi", tpdizhi);
                    dc.Add("zuozhe", zuozhe);
                    dc.Add("jieshao", jieshao);
                    dc.Add("id", id);
                    cn.ExecQuery("update T0003 set C00011 = @name,C00013=@tpdizhi where C00010=@id", new OleDbParameter[] { new OleDbParameter("@name", name), new OleDbParameter("@tpdizhi", tpdizhi), new OleDbParameter("@id", id) });
                    cn.ExecSQL("update T0004 set C00017 = @name,C00018 = @jianjie,C00024 = @tpdizhi,C00025 = @zuozhe,C00026 = @jieshao where C00016 = @id",cn.sqlparameter_append(dc));
                    //循环插入分类
                    string insertsql = "insert into T0002(C00008,C00009) values (@p2,@p3)";
                    cn.ExecSQL("delete from T0002 where C00009 = @id", new OleDbParameter[] { new OleDbParameter("@p", id) });
                    if (T != null)
                    foreach (string s in T)
                    {
                        cn.ExecSQL(insertsql, new OleDbParameter[] { new OleDbParameter("@p2", s), new OleDbParameter("@p3", id) });
                    }
                    Response.Write("<script>alert('修改成功');window.location.href='addcomic.aspx?id=" + id + "'</script>");
                }
            }
        }

        private string Chinese(string str)
        {
            if (str.CompareTo("吖") < 0)
            {
                string s = str.Substring(0, 1).ToUpper();
                if (char.IsNumber(s, 0))
                {
                    return "0";
                }
                else
                {
                    return s;
                }
            }
            else if (str.CompareTo("八") < 0)
            {
                return "A";
            }
            else if (str.CompareTo("嚓") < 0)
            {
                return "B";
            }
            else if (str.CompareTo("咑") < 0)
            {
                return "C";
            }
            else if (str.CompareTo("妸") < 0)
            {
                return "D";
            }
            else if (str.CompareTo("发") < 0)
            {
                return "E";
            }
            else if (str.CompareTo("旮") < 0)
            {
                return "F";
            }
            else if (str.CompareTo("铪") < 0)
            {
                return "G";
            }
            else if (str.CompareTo("讥") < 0)
            {
                return "H";
            }
            else if (str.CompareTo("咔") < 0)
            {
                return "J";
            }
            else if (str.CompareTo("垃") < 0)
            {
                return "K";
            }
            else if (str.CompareTo("嘸") < 0)
            {
                return "L";
            }
            else if (str.CompareTo("拏") < 0)
            {
                return "M";
            }
            else if (str.CompareTo("噢") < 0)
            {
                return "N";
            }
            else if (str.CompareTo("妑") < 0)
            {
                return "O";
            }
            else if (str.CompareTo("七") < 0)
            {
                return "P";
            }
            else if (str.CompareTo("亽") < 0)
            {
                return "Q";
            }
            else if (str.CompareTo("仨") < 0)
            {
                return "R";
            }
            else if (str.CompareTo("他") < 0)
            {
                return "S";
            }
            else if (str.CompareTo("哇") < 0)
            {
                return "T";
            }
            else if (str.CompareTo("夕") < 0)
            {
                return "W";
            }
            else if (str.CompareTo("丫") < 0)
            {
                return "X";
            }
            else if (str.CompareTo("帀") < 0)
            {
                return "Y";
            }
            else if (str.CompareTo("咗") < 0)
            {
                return "Z";
            }
            else
            {
                return "0";
            }
        }
    }
}