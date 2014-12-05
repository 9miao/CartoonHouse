using System;
using System.Collections.Generic;
using System.Text;
using System.Web;

namespace BBDCL.PageCache
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public class CCache
    {
        
        public HttpContext context { get; set; }
        public string PageDataKey { get; set; }
        public string Sqlstr { get; set; }
        public int Spacing = 20;
        public CCache(HttpContext c, string PageKey, string SqlString, int Spacing)
        {
            this.Sqlstr = SqlString;
            this.context = c;
            this.PageDataKey = PageKey;
            this.Spacing = Spacing;
        }
        public CPageDataCache getPageDataCache()
        {
            if (context.Cache.Get(PageDataKey) != null)
            {
                CPageDataCache c = (CPageDataCache)context.Cache[PageDataKey];
                c.UpdateSpacing = Spacing;
                return c;
            }
            else
            {
                CPageDataCache c = CPageDataCache.CreatePageDataCache(PageDataKey, Sqlstr, Spacing);
                lock (context.Cache)
                {
                    if (context.Cache[PageDataKey] == null)
                        context.Cache[PageDataKey] = c;
                    return c;
                }
            }
        }

        public static CPageDataCache FindPageData(HttpContext context, String PageDataKey)
        {
            if (context.Cache.Get(PageDataKey) != null)
            {
                CPageDataCache c = (CPageDataCache)context.Cache[PageDataKey];
                return c;
            }else
            {
                return null;
            }
        }
    }
}
