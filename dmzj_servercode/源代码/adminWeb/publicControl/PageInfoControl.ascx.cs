using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using BBDCL;
namespace Manager.Class
{
    public partial class PageInfoControl : System.Web.UI.UserControl
    {
        public string PageString = "";
        public PageInfo i;
        public bool NoRecordHidden = false;
        public string URL;
        private int MaxDispPage = 5;
        public HyperLink TopPage, PrePage, NextPage, lastPage;
        protected void Page_Load(object sender, EventArgs e)
        {



        }

        public void initPage()
        {
            if (NoRecordHidden && i.RecordCount == 0)
            {
                return;
            }
            int countpage = i.PageCount;
            int currpage = i.CurrPageIndex;
            // 处理固定部分
            TopPage = new HyperLink();
            TopPage.Text = "第一页";
            PrePage = new HyperLink();
            PrePage.Text = "上一页";
            NextPage = new HyperLink();
            NextPage.Text = "下一页";
            lastPage = new HyperLink();
            lastPage.Text = "最后页";
            this.TopPage.NavigateUrl = "";
            this.PrePage.NavigateUrl = "";
            this.NextPage.NavigateUrl = "";
            this.lastPage.NavigateUrl = "";

            this.TopPage.NavigateUrl = URL + "Page=1";
            this.PrePage.NavigateUrl = URL + "Page=" + (currpage - 1).ToString();
            this.NextPage.NavigateUrl = URL + "Page=" + (currpage + 1).ToString();
            this.lastPage.NavigateUrl = URL + "Page=" + countpage.ToString(); ;
            this.div1.Controls.Add(TopPage);
            this.div1.Controls.Add(PrePage);
            if (currpage == 1)
            {
                this.TopPage.NavigateUrl = "";
                this.PrePage.NavigateUrl = "";
            }
            if (currpage == countpage)
            {
                this.NextPage.NavigateUrl = "";
                this.lastPage.NavigateUrl = "";
            }

            if (countpage <= MaxDispPage) //页面数小于5 则只列有限的页
            {
                for (int ii = 1; ii <= countpage; ii++)
                {
                    HyperLink pageindex = new HyperLink();
                    pageindex.Text = ii.ToString();
                    if (currpage != ii)
                    {
                        pageindex.NavigateUrl = URL + "Page=" + ii.ToString();
                    }
                    else
                    {
                        pageindex.CssClass = "isthis";
                    }
                    this.div1.Controls.Add(pageindex);
                }
            }
            else
            {

                if (countpage - currpage > 2)
                {
                    if (currpage > 2)
                    {
                        for (int ii = currpage - 2; ii <= currpage + 2; ii++)
                        {
                            addpage(ii, currpage);
                        }
                    }
                    else
                    {
                        for (int ii = 1; ii <= MaxDispPage; ii++)
                        {
                            addpage(ii, currpage);
                        }
                    }

                }
                else
                {
                    for (int ii = countpage - 4; ii <= countpage; ii++)
                    {
                        HyperLink pageindex = new HyperLink();

                        pageindex.Text = ii.ToString();
                        if (currpage != ii)
                        {
                            pageindex.NavigateUrl = URL + "&Page=" + ii.ToString(); ;
                        }
                        else
                        {
                            pageindex.CssClass = "isthis";
                        }
                        this.div1.Controls.Add(pageindex);
                    }
                }
            }

            this.div1.Controls.Add(NextPage);
            this.div1.Controls.Add(lastPage);
            if (div1.Controls.Count < 7)
                div1.Visible = false;
        }

        void addpage(int ii, int currpage)
        {
            HyperLink pageindex = new HyperLink();
            pageindex.Text = ii.ToString();
            if (currpage != ii)
            {
                pageindex.NavigateUrl = URL + "&Page=" + ii.ToString();
            }
            else
                pageindex.CssClass = "isthis";
            this.div1.Controls.Add(pageindex);
        }
    }
}