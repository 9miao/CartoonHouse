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
    /// 分类数据
    /// </summary>
    public class TreeList
    {
        public TreeListing[] Lists { get; set; }
    }
    /// <summary>
    /// 数据集
    /// </summary>
    public class TreeListing 
    {
        //id
        public string id { get; set; }
        //图标
        public string icon { get; set; }
        //标题
        public string title { get; set; }

        public TreeListing[] subnodes { get; set; }
    }
}
