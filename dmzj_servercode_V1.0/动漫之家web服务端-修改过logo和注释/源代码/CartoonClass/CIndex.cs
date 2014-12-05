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


    public class CIndex
    {
        public CIndexData index { get; set; }
        public CIndexData new_key_huan { get; set; }
        public CIndexData lianzai { get; set; }
        public CIndexData wanjie { get; set; }
        public CIndexData yuanchuang { get; set; }
    }

    public class CIndexData 
    {
        public headlinesInfo headlines { get; set; }
        public episodeList[] episode { get; set; }
    }

    public class headlinesInfo 
    {
        public string id { get; set; }
        public string cover { get; set; }
        public string title { get; set; }
        public string subtitle { get; set; }
    }

    public class episodeList 
    {
        public string id { get; set; }
        public string cover { get; set; }
        public string title { get; set; }
        public string last_update_chapter_name { get; set; }
    }
}
