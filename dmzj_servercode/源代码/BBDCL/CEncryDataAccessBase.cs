using System;
using System.Data;
using System.IO;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Web.Security;

namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    [Serializable]
    public class CEncryDataAccessBase : CDataAccessBase
    {
        public bool isEncrypt { get; set; }

        /// <summary>
        /// 加密
        /// </summary>
        /// <param name="Text"></param>
        /// <param name="sKey"></param>
        /// <returns></returns>
        protected string Encrypt(string Text, string sKey)
        {
            if (!isEncrypt) return Text;
            var des = new DESCryptoServiceProvider();
            byte[] inputByteArray;
            inputByteArray = Encoding.Default.GetBytes(Text);

            des.Key =
                Encoding.ASCII.GetBytes(FormsAuthentication.HashPasswordForStoringInConfigFile(sKey, "md5").Substring(
                    0, 8));
            des.IV =
                Encoding.ASCII.GetBytes(FormsAuthentication.HashPasswordForStoringInConfigFile(sKey, "md5").Substring(
                    0, 8));
            var ms = new MemoryStream();
            var cs = new CryptoStream(ms, des.CreateEncryptor(), CryptoStreamMode.Write);
            cs.Write(inputByteArray, 0, inputByteArray.Length);
            cs.FlushFinalBlock();
            var ret = new StringBuilder();
            foreach (byte b in ms.ToArray())
            {
                ret.AppendFormat("{0:X2}", b);
            }
            return ret.ToString();
        }


        /// <summary>
        /// 解密
        /// </summary>
        /// <param name="Text"></param>
        /// <param name="sKey"></param>
        /// <returns></returns>
        protected string Decrypt(string Text, string sKey)
        {
            if (!isEncrypt) return Text;
            var des = new DESCryptoServiceProvider();
            int len;
            len = Text.Length/2;
            var inputByteArray = new byte[len];
            int x, i;
            for (x = 0; x < len; x++)
            {
                i = Convert.ToInt32(Text.Substring(x*2, 2), 16);
                inputByteArray[x] = (byte) i;
            }
            des.Key =
                Encoding.ASCII.GetBytes(FormsAuthentication.HashPasswordForStoringInConfigFile(sKey, "md5").Substring(
                    0, 8));
            des.IV =
                Encoding.ASCII.GetBytes(FormsAuthentication.HashPasswordForStoringInConfigFile(sKey, "md5").Substring(
                    0, 8));
            var ms = new MemoryStream();
            var cs = new CryptoStream(ms, des.CreateDecryptor(), CryptoStreamMode.Write);
            cs.Write(inputByteArray, 0, inputByteArray.Length);
            cs.FlushFinalBlock();
            return Encoding.Default.GetString(ms.ToArray());
        }


        public override void InitFromDataRow(DataRow dr)
        {
            Type t = GetType();
            foreach (PropertyInfo pi in t.GetProperties(BindingFlags.Instance | BindingFlags.NonPublic))
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
        public override void proptertyToDataTable(DataRow dr)
        {
            Type t = GetType();

            foreach (PropertyInfo pi in t.GetProperties(BindingFlags.Instance | BindingFlags.NonPublic))
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
    }
}