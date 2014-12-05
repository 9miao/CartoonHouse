using System;
using System.Collections.Generic;


namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


   public class Fenye
    {
        public string ProcPage(string url, int maxPage, int Currpage)
        {
            string result = "";
            //处理上一页
            if (Currpage > 1)
            {
                result += "<a href=\'" + url + "Page=" + (Currpage - 1) + "' class='fenye'>上一页</a>";
            }
            else
            {
                result += "<a href='javascript:void(0)' class='fenye'>上一页</a>";
            }

            if (maxPage > 12)
            {
                #region 作废
                //if (maxPage == 13)
                //{
                //    //两头用省略号
                //    if (maxPage > 13)
                //    {

                //    }
                //    else
                //    {

                //    }

                //}
                //else
                //{
                //    for (int i = 1; i <= 10; i++)
                //    {
                //        if (i == Currpage)
                //        {
                //            result += " <a href=\"javascript:void(0)\" class=\"d2\">1</a> ";
                //        }
                //        else
                //        {
                //            result += " <a href=\""+url+"?id=id&Page=" + "" + "_" + "" + "_" + "" + "_" + i + " class=\"d2\">1</a> ";
                //        }
                //    }
                //    result += " <a href=\""+url+"?id=id&Page=" + "" + "_" + "" + "_" + "" + "_" + 11 + " class=\"d2\">...</a> ";
                //    result += " <a href=\""+url+"?id=id&Page=" + "" + "_" + "" + "_" + "" + "_" + maxPage + " class=\"d2\">...</a> ";
                //}
                #endregion
                if (maxPage < 14)
                {
                    //单向 填充一个 省略号
                    if (Currpage > 3)
                    {
                        //左填充省略号
                        result += " <a href=\"" + url + "Page=" + 1 + "\" class=\"d2\">" + 1 + "</a> ";
                        result += " <a href=\"" + url + "Page=" + (Currpage - 1) + "\" class=\"d2\">...</a> ";
                        for (int i = Currpage; i < maxPage; i++)
                        {
                            if (i == Currpage)
                            {
                                result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + i + "</a> ";
                            }
                            else
                            {
                                result += " <a href=\"" + url + "Page=" + i + "\" class=\"d2\">" + i + "</a> ";
                            }
                        }
                        if (Currpage == maxPage)
                        {
                            result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + maxPage + "</a> ";
                        }
                        else
                        {
                            result += " <a href=\"" + url + "Page=" + (maxPage) +
                                      "\" class=\"d2\">" + maxPage + "</a> ";
                        }
                    }
                    else
                    {
                        //右填充省略号
                        for (int i = 1; i <= 10; i++)
                        {
                            if (i == Currpage)
                            {
                                result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + i + "</a> ";
                            }
                            else
                            {
                                result += " <a href=\"" + url + "Page=" + i + "\" class=\"d2\">" + i + "</a> ";
                            }
                        }
                        result += " <a href=\"" + url + "Page=" + 11 + "\" class=\"d2\">...</a> ";
                        result += " <a href=\"" + url + "Page=" + maxPage + "\" class=\"d2\">" + maxPage + "</a> ";
                    }
                }
                else
                {
                    //双向填充
                    if (Currpage > 3 && Currpage < (maxPage - 9))
                    {
                        //双向填充
                        result += " <a href=\"" + url + "Page=" + 1 + "\" class=\"d2\">" + 1 + "</a> ";
                        result += " <a href=\"" + url + "Page=" + (Currpage - 1) + "\" class=\"d2\">...</a> ";
                        for (int i = Currpage; i < Currpage + 8; i++)
                        {
                            if (i == Currpage)
                            {
                                result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + i + "</a> ";
                            }
                            else
                            {
                                result += " <a href=\"" + url + "Page=" + i + "\" class=\"d2\">" + i + "</a> ";
                            }
                        }

                        result += " <a href=\"" + url + "Page=" + 11 + "\" class=\"d2\">...</a> ";
                        result += " <a href=\"" + url + "Page=" + maxPage +
                                  "\" class=\"d2\">" + maxPage + "</a> ";
                    }
                    else
                    {
                        //单向填充
                        if (Currpage > (maxPage - 9))
                        {
                            //向左填充
                            result += " <a href=\"" + url + "Page=" + 1 + "\" class=\"d2\">" + 1 + "</a> ";
                            result += " <a href=\"" + url + "Page=" + (Currpage - 1) + "\" class=\"d2\">...</a> ";
                            for (int i = Currpage; i < maxPage; i++)
                            {
                                if (i == Currpage)
                                {
                                    result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + i + "</a> ";
                                }
                                else
                                {
                                    result += " <a href=\"" + url + "Page=" + i + "\" class=\"d2\">" + i + "</a> ";
                                }
                            }
                            if (Currpage == maxPage)
                            {
                                result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + maxPage + "</a> ";
                            }
                            else
                            {
                                result += " <a href=\"" + url + "Page=" + (maxPage) +
                                          "\" class=\"d2\">" + maxPage + "</a> ";
                            }
                        }
                        else
                        {
                            //右填充
                            for (int i = 1; i <= 10; i++)
                            {
                                if (i == Currpage)
                                {
                                    result += " <a href=\"javascript:void(0)\" class=\"fanyechangeed\">" + i + "</a> ";
                                }
                                else
                                {
                                    result += " <a href=\"" + url + "Page=" + i + "\" class=\"d2\">" + i + "</a> ";
                                }
                            }
                            result += " <a href=\"" + url + "Page=" + 11 + "\" class=\"d2\">...</a> ";
                            result += " <a href=\"" + url + "Page=" + maxPage + "\" class=\"d2\">" + maxPage + "</a> ";
                        }
                    }
                }
            }
            else
            {
                for (int i = 1; i <= maxPage; i++)
                {
                    if (i == Currpage)
                    {
                        result += " <span class=\"fanyechangeed\">" + i + "</span> ";
                    }
                    else
                    {
                        result += " <a href=\"" + url + "Page=" + i + "\" class=\"d2\">" + i + "</a> ";
                    }
                }
            }


            //处理最后一页
            if (Currpage == maxPage)
            {
                result += "<a href='javascript:void(0)' class='d4'>下一页</a>";
            }
            else if (Currpage < maxPage)
            {
                result += "<a href='javascript:void(0)' class='d4'>下一页</a>";
            }
            else
            {
                result += "<a href='" + url + "Page=" + (Currpage + 1) +
                          "' class='d4'>下一页</a>";

            }
            return result;
        }
    }
}
