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
    /// 动漫的基本信息
    /// </summary>
    public class ComicInforMation
    {
        public int result { get; set; }
        public string msg { get; set; }
        public Comicdata data { get; set; }
    }
    /// <summary>
    /// 信息类
    /// </summary>
    public class Comicdata
    {
        public InforMation info { get; set; }
        public wordList[] list { get; set; }
        public aloneList[] alone { get; set; }
        public similarList[] similar { get; set; }
    }

    /// <summary>
    /// alone 参数 不知道干嘛的 空
    /// </summary>
    public class aloneList 
    {
        
    }

    /// <summary>
    /// 估计是关联
    /// </summary>
    public class similarList 
    {
        public string id { get; set; }
        public string title { get; set; }
        public string last_update_chapter_name { get; set; }
        public string cover { get; set; }
    }

    /// <summary>
    /// 基本信息
    /// </summary>
    public class InforMation 
    {
        public string id { get; set; }
        public string title { get; set; }
        public string subtitle { get; set; }
        public string types { get; set; }
        public string zone { get; set; }
        public string status { get; set; }
        public string last_update_chapter_name { get; set; }
        public string last_updatetime { get; set; }
        public string cover { get; set; }
        public string authors { get; set; }
        public string description { get; set; }
        public string first_letter { get; set; }
        public string direction { get; set; }
    }

    /// <summary>
    /// 话的列表
    /// </summary>
    public class wordList 
    {
        public string id { get; set; }
        public string comic_id { get; set; }
        public string chapter_name { get; set; }
        public string chapter_order { get; set; }
        public string filesize { get; set; }
    }
}
