using System;
using System.Collections.Generic;
using System.Text;

namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    [Serializable]
    public struct PageInfo
    {
        public int CurrPageIndex;
        public int PageSize;
        public int PageCount;
        public int RecordCount;
    }

}
