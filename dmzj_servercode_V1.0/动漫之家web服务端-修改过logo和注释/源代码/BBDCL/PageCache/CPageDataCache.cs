using System;
using System.Collections.Generic;
using System.Data;
using System.Text;
using System.Threading;

namespace BBDCL.PageCache
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    [Serializable]
    public class CPageDataCache : IPageDataChche, IDisposable
    {
        private DataTable dt;
        public int UpdateSpacing { get; set; }
        private string isUpdatting = "0";
        private DateTime UpdateTime;
        public string SqlString { get; set; }
        public CPageDataCache()
        {
            if (UpdateSpacing == null || UpdateSpacing == 0) this.UpdateSpacing = 10;
        }

        public DataTable getDt()
        {
            int currspacing = (int)DateTime.Now.Subtract(UpdateTime).TotalSeconds;
            if (currspacing > UpdateSpacing)
            {
                //调用线程 更新数据集
                ThreadStart ts = new ThreadStart(UpdateDt);
                Thread t = new Thread(ts);
                t.Start();
            }
            return dt;
        }

        public static CPageDataCache CreatePageDataCache(string sKey, string SqlString, int Spacing)
        {
            CPageDataCache cpdc = new CPageDataCache();
            cpdc.UpdateSpacing = Spacing;
            cpdc.SqlString = SqlString;
            //执行查询
            string Connstr = System.Configuration.ConfigurationManager.AppSettings["DBConnStr"];
            using (CDBAccess cn = new CDBAccess(Connstr))
            {
                cpdc.dt = cn.ExecQuery(cpdc.SqlString);
            }
            cpdc.dt.TableName = "TBN_" + DateTime.Now.ToString("HH:mm:ss fffff");                      cpdc.UpdateTime = DateTime.Now;
            return cpdc;
        }

        void UpdateDt()
        {
            try
            {
                lock (isUpdatting)
                {
                    if (isUpdatting == "1") return;
                    isUpdatting = "1";
                }
                //在此执行查询
                string Connstr = System.Configuration.ConfigurationManager.AppSettings["DBConnStr"];
                DataTable dt2 = null;
                using (CDBAccess cn = new CDBAccess(Connstr))
                {
                    dt2 = cn.ExecQuery(SqlString);
                }
               
                dt2.TableName = "TBN_" + DateTime.Now.ToString("HH:mm:ss fffff");


                DataTable dtTemp = this.dt;
                lock (this.dt)
                {
                    this.UpdateTime = DateTime.Now;
                    this.dt = dt2;
                }
                dtTemp.Dispose();
                lock (isUpdatting)
                {
                    isUpdatting = "0";
                }
            }
            catch (Exception ex)
            {
            }

        }



        public void RefreashPageDataCache(bool ForceRefresh)
        {
            if (!ForceRefresh)
            {
                //非强制刷新
                int currspacing = (int)DateTime.Now.Subtract(UpdateTime).TotalSeconds;
                if (currspacing < UpdateSpacing)
                {
                    return;
                }
            }
            ThreadStart ts = new ThreadStart(UpdateDt);
            Thread t = new Thread(ts);
            t.Start();
        }

        public void Dispose()
        {
            if (dt != null)
                this.dt.Dispose();
        }
    }
}
