using System;
using System.Data;
using System.Reflection;

namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    [Serializable] 
    public class CDataAccessBase
    {
        public string PKFieldName { get; set; }
        public string TableName { get; set; }

        // 从DataRow对象中恢复属性
        public virtual void InitFromDataRow(DataRow dr)
        {
            Type t = GetType();
            foreach (PropertyInfo pi in t.GetProperties())
            {
                foreach (DataColumn dc in dr.Table.Columns)
                {
                    if (dc.ColumnName == pi.Name)
                    {
                        try
                        {
                            pi.SetValue(this, dr[dc], null);
                        }
                        catch (Exception ex)
                        {
                        }
                    }
                }
            }
        }

        //将属性值写会到DataRow中
        public virtual void proptertyToDataTable(DataRow dr)
        {
            Type t = GetType();

            foreach (PropertyInfo pi in t.GetProperties())
            {
                foreach (DataColumn dc in dr.Table.Columns)
                {
                    if (dc.ColumnName.ToUpper() == pi.Name.ToUpper())
                    {
                        dr[dc] = pi.GetValue(this, null);
                    }
                }
            }
        }

        public virtual void beforSave(CDBAccess cda)
        {
            
        }

        public virtual void afterSave(CDBAccess cda)
        {
            
        }

        public object getPkValue()
        {
            Type t = GetType();
            foreach (PropertyInfo pi in t.GetProperties(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public))
            {

                if (pi.Name == this.PKFieldName)
                {
                    return pi.GetValue(this, null);
                    break;
                }
            }
            return "";
        }
    }
}