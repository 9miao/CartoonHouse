using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace adminWeb.adminClass
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    /// <summary>
    /// 通用实体类
    /// </summary>
    public class jsonClass
    {
        /// <summary>
        /// 状态
        /// </summary>
        public string Status { get; set; }

        /// <summary>
        /// 说明
        /// </summary>
        public string Msg { get; set; }
    }

    /// <summary>
    /// 分类实体类
    /// </summary>
    public class Classify 
    {
        /// <summary>
        /// 状态
        /// </summary>
        public string Status { get; set; }

        /// <summary>
        /// 说明
        /// </summary>
        public string Msg { get; set; }

        /// <summary>
        /// 分类名称
        /// </summary>
        public string classifyName { get; set; }
    }
}