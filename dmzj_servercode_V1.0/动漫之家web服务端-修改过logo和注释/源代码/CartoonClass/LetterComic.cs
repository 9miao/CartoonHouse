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


    /// <summary>
    /// 根据字母获取漫画信息
    /// </summary>
    public class LetterComic
    {
        public int result { get; set; }
        public string msg { get; set; }
        public ComicList[] data { get; set; }
    }

    public class ComicList 
    {
        public string id { get; set; }
        public string title { get; set; }
        public string last_update_chapter_name { get; set; }
        public string cover { get; set; }
        public string last_updatetime { get; set; }
        public string hot_hits { get; set; }
    }

}
