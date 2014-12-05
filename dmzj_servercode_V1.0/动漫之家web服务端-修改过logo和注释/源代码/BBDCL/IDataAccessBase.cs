using System;

namespace BBDCL
{


    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    public interface IDataAccessBase
    {
        CClassQuery generateListSql();
        CClassQuery CheckObj();
        CClassQuery generateListSql(int topRecoderCount);
    }
}