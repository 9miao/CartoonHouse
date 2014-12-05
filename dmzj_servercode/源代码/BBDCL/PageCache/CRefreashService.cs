using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Web;

namespace BBDCL.PageCache
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class CRefreashService
    {
        public int Spacing = 36000;
        public DateTime LastRefeashTime = DateTime.Now;
        public volatile string isRunning = "0";
        public HttpContext context { get; set; }
        public bool ForceRefeash = false;
        private volatile bool isWork = true;
        public void Run()
        {
            isWork = true;
            ThreadStart ts = new ThreadStart(MainThreadFunction);
            Thread t = new Thread(ts);
            t.Start();
        }

        public void Stop()
        {
            isWork = false;
        }

        void MainThreadFunction()
        {
            while (isWork)
            {
                ThreadStart ts = new ThreadStart(Update);
                Thread t = new Thread(ts);
                t.Start();
                Thread.Sleep(Spacing * 1000);
            }

        }

        void Update()
        {
            try
            {
                lock (isRunning)
                {
                    if (isRunning == "1") return;
                    isRunning = "1";
                }

                //循环变量
                foreach (DictionaryEntry item in context.Cache)
                {
                    if (item.Value is IPageDataChche)
                    {
                        ((IPageDataChche)item.Value).RefreashPageDataCache(ForceRefeash);
                    }
                }
                lock (isRunning)
                {
                    isRunning = "0";
                }
                LastRefeashTime = DateTime.Now;
            }
            catch (Exception e)
            {
                isRunning = "0";
            }
        }
    }
}
