using BBDCL;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace adminWeb.adminClass
{


    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class exPagers
    {
        /// <summary>
        /// ACCESS高效分页
        /// </summary>
        /// <param name="pageIndex">当前页码</param>
        /// <param name="pageSize">分页容量</param>
        /// <param name="strKey">主键</param>
        /// <param name="showString">显示的字段</param>
        /// <param name="queryString">查询字符串，支持联合查询</param>
        /// <param name="whereString">查询条件，若有条件限制则必须以where 开头</param>
        /// <param name="orderString">排序规则</param>
        /// <param name="pageCount">传出参数：总页数统计</param>
        /// <param name="recordCount">传出参数：总记录统计</param>
        /// <returns>装载记录的DataTable</returns>
        public static DataTable ExecutePager(int pageIndex, int pageSize, string strKey, string showString, string queryString, string whereString, string orderString, out int pageCount, out int recordCount)
        {
            if (pageIndex < 1) pageIndex = 1;
            if (pageSize < 1) pageSize = 10;
            if (string.IsNullOrEmpty(showString)) showString = "*";
            if (string.IsNullOrEmpty(orderString)) orderString = strKey + " asc ";
            using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                string myVw = string.Format(" ( {0} ) tempVw ", queryString);

                string sql111 = string.Format(" select count(*) as recordCount from {0} {1}", myVw, whereString);

                recordCount = Convert.ToInt32(cn.ExecScalar(sql111));

                if ((recordCount % pageSize) > 0)
                    pageCount = recordCount / pageSize + 1;
                else
                    pageCount = recordCount / pageSize;
                string sql;
                if (pageIndex == 1)//第一页
                {
                    sql = string.Format("select top {0} {1} from {2} {3} order by {4} ", pageSize, showString, myVw, whereString, orderString);
                }
                else if (pageIndex > pageCount)//超出总页数
                {
                    sql = string.Format("select top {0} {1} from {2} {3} order by {4} ", pageSize, showString, myVw, "where 1=2", orderString);
                }
                else
                {
                    int pageLowerBound = pageSize * pageIndex;
                    int pageUpperBound = pageLowerBound - pageSize;
                    string recordIDs = recordID(string.Format("select top {0} {1} from {2} {3} order by {4} ", pageLowerBound, strKey, myVw, whereString, orderString), pageUpperBound);
                    sql = string.Format("select {0} from {1} where {2} in ({3}) order by {4} ", showString, myVw, strKey, recordIDs, orderString);

                }
                using (DataTable dt = cn.ExecQuery(sql))
                {
                    return dt;
                }
            }
        }
        /// <summary>
        /// 分页使用
        /// </summary>
        /// <param name="query"></param>
        /// <param name="passCount"></param>
        /// <returns></returns>
        private static string recordID(string query, int passCount)
        {
            string result = string.Empty;
            using (
                                     CDBAccess cn =
                                         new CDBAccess(System.Configuration.ConfigurationSettings.AppSettings["OledbStr"]))
            {
                using (DataTable _dt = cn.ExecQuery(query))
                {
                    foreach (DataRow dr in _dt.Rows)
                    {
                        if (passCount < 1)
                        {
                            result += ",'" + dr[0] + "'";
                        }
                        passCount--;
                    }
                }
                return result.Substring(1);
            }
        }


        /// <summary>
        /// int转换时间
        /// </summary>
        /// <param name="timeInt"></param>
        /// <returns></returns>
        public static DateTime IntToDatetime(string timeInt) 
        {
            Int64 begtime = Convert.ToInt64(timeInt) * 10000000;//100毫微秒为单位,textBox1.text需要转化的int日期
            DateTime dt_1970 = new DateTime(1970, 1, 1, 8, 0, 0);
            long tricks_1970 = dt_1970.Ticks;//1970年1月1日刻度
            long time_tricks = tricks_1970 + begtime;//日志日期刻度
            DateTime dt = new DateTime(time_tricks);//转化为DateTime
            return dt;
        }

        /// <summary>
        /// 时间转换int
        /// </summary>
        /// <param name="end">结束时间</param>
        /// <returns>int值</returns>
        public static int getSecondEnd(DateTime end)
        {
            int result = 0;
            DateTime startdate = new DateTime(1970, 1, 1, 8, 0, 0);
            TimeSpan seconds = end.AddDays(1) - startdate;
            result = Convert.ToInt32(seconds.TotalSeconds);
            return result;
        }
    }
}