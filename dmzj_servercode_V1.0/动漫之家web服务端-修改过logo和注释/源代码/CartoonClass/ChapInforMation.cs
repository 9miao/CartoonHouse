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


    public class ChapInforMation
    {
        public string id { get; set; }
        public string comic_id { get; set; }
        public string chapter_name { get; set; }
        public string chapter_order { get; set; }
        public string[] page { get; set; }
    }

}
