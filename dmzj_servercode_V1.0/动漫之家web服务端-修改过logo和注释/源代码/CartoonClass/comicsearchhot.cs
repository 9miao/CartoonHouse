using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CartoonClass
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class comicsearchhots
    {
        public int result { get; set; }
        public string msg { get; set; }
        public comicsearchhotData[] data { get; set; }
    }

    public class comicsearchhotData 
    {
        public string id { get; set; }
        public string name { get; set; }
    }
}
