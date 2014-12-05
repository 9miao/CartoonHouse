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


    public class Ctop
    {
        public Topdata[] total { get; set; }
        public Topdata[] month { get; set; }
        public Topdata[] week { get; set; }

    }

    public class Topdata 
    {
        public string id { get; set; }
        public string title { get; set; }
        public string last_update_chapter_name { get; set; }
        public string cover { get; set; }
        public string authors { get; set; }
        public string types { get; set; }
        public string sum_chapters { get; set; }
        public string sum_source { get; set; }
    }
}
