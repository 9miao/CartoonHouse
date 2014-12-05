using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;

namespace BBDCL
{

    /// <summary>
    /// 动漫app由9miao社团研发,如有问题请登陆http://www.9miao.com/官网
    /// </summary>
    /// 


    [Serializable]
    public class CDBAccess : IDisposable
    {
        private readonly string ConnString = "";
        private OleDbDataAdapter adapter;
        private OleDbCommand command;
        private OleDbCommandBuilder commandBuilder;
        public OleDbConnection connection;
        private bool isdisposeConn = true;
        public OleDbTransaction odt;
        private Dictionary<int, string> sqlList;


        /// <summary>
        /// 从Ini文件初始化连接对象
        /// </summary>
        /// <param name="iniFilePath"></param>
        /// <param name="section"></param>
        public CDBAccess(string iniFilePath, string section)
        {
            ConnString = GetConnStr(iniFilePath, section);
            initClass(true);
        }

        /// <summary>
        /// 根据连接字符串初始化数据库对象
        /// </summary>
        /// <param name="pConnString">外部连接字符串</param>
        public CDBAccess(string pConnString)
        {
            ConnString = pConnString;
            initClass(true);
            if (connection.State == ConnectionState.Closed)
                connection.Open();
        }


        /// <summary>
        /// 从前台调用初获取Conn 初始化对象 
        /// </summary>
        /// <param name="oleConn"></param>
        public CDBAccess(ref OleDbConnection oleConn)
        {
            connection = oleConn;
            initClass(false);
        }

        /// <summary>
        /// 释放对象所占用的资源
        /// </summary>
        public void Dispose()
        {
            if (connection.State != ConnectionState.Closed && isdisposeConn)
            {
                connection.Close();
            }
        }

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal,
                                                          int size, string filePath);

        /// <summary>
        /// 出示化对象数据
        /// </summary>
        /// <param name="isInitConn">是否根据连接字符串初始化对象 </param>
        private void initClass(bool isInitConn)
        {
            if (isInitConn)
            {
                connection = new OleDbConnection(ConnString);
                isdisposeConn = true;
            }
            else
            {
                isdisposeConn = false;
            }
            command = new OleDbCommand();
            command.Connection = connection;
            adapter = new OleDbDataAdapter();
            commandBuilder = new OleDbCommandBuilder(adapter);
            sqlList = new Dictionary<int, string>();
        }

        /// <summary>
        /// 从Ini文件中读取连接字符串信息
        /// </summary>
        /// <param name="iniFilePath"></param>
        /// <param name="section"></param>
        /// <returns></returns>
        private static string GetConnStr(string iniFilePath, string section)
        {
            var DataSource = new StringBuilder(100);
            var InitialCatalog = new StringBuilder(100);
            var UserID = new StringBuilder(100);
            var PersistSecurityInfo = new StringBuilder(100);
            var Password = new StringBuilder(100);
            var Provider = new StringBuilder(100);

            GetPrivateProfileString(section, "Data Source", "", DataSource, 100, iniFilePath);
            GetPrivateProfileString(section, "Initial Catalog", "", InitialCatalog, 100, iniFilePath);
            GetPrivateProfileString(section, "User ID", "", UserID, 100, iniFilePath);
            GetPrivateProfileString(section, "Persist Security Info", "", PersistSecurityInfo, 100, iniFilePath);
            GetPrivateProfileString(section, "Password", "", Password, 100, iniFilePath);
            GetPrivateProfileString(section, "Provider", "", Provider, 100, iniFilePath);

            return
                string.Format(
                    "Provider={0};Password={1};Persist Security Info={2};User ID={3};Initial Catalog={4};Data Source={5}",
                    Provider, Password, PersistSecurityInfo, UserID, InitialCatalog, DataSource);
        }

        /// <summary>
        /// 执行SQL语句
        /// </summary>
        /// <param name="sql">SQL语句</param>
        /// <param parameters="sql">SQL语句所带的参数</param>
        /// <param name="parameters"></param>
        /// <returns>受影响的行数</returns>
        public int ExecSQL(string sql, OleDbParameter[] parameters)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
           
            command.CommandText = sql;
            command.Parameters.Clear();
            command.Parameters.AddRange(parameters);
            int result = command.ExecuteNonQuery();
            command.Parameters.Clear();
            if (odt == null && isdisposeConn) connection.Close();
            return result;
        }

        /// <summary>
        /// 执行SQL语句
        /// </summary>
        /// <param name="sql">SQL语句</param>
        /// <returns>受影响的行数</returns>
        public int ExecSQL(string sql)
        {
            return ExecSQL(sql, new OleDbParameter[0]);
        }

        /// <summary>
        /// 执行查询
        /// </summary>
        /// <param name="sql">查询语句</param>
        /// <returns>查询结果表</returns>
        public DataTable ExecQuery(string sql)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var dt = new DataTable();
            command.CommandText = sql;
            adapter.SelectCommand = command;
            adapter.Fill(dt);
            sqlList.Add(dt.GetHashCode(), sql);
            dt.Disposed += DataTable_Disposed;
            if (odt == null && isdisposeConn) connection.Close();
            return dt;
        }

      

        // <summary>
        /// 执行查询
        /// </summary>
        /// <param name="sql">查询语句</param>
        /// <returns>查询结果表</returns>
        public DataTable ExecQuery(string sql, OleDbParameter[] parameters)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var dt = new DataTable();
            command.Parameters.Clear();
            command.Parameters.AddRange(parameters);
            command.CommandText = sql;

            adapter.SelectCommand = command;
            adapter.Fill(dt);
            sqlList.Add(dt.GetHashCode(), sql);
            dt.Disposed += DataTable_Disposed;
            command.Parameters.Clear();
            if (odt == null && isdisposeConn) connection.Close();
            return dt;
        }


        /// <summary>
        /// 执行一个返回结果集的存储过程
        /// </summary>
        /// <param name="ProcedureName"></param>
        /// <returns></returns>
        public DataSet ExecProcedure(string ProcedureName, OleDbParameter[] parameters)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            command.Parameters.Clear();
            command.Parameters.AddRange(parameters);
            command.CommandText = ProcedureName;
            command.CommandType = CommandType.StoredProcedure;
            adapter.SelectCommand = command;
            var ds = new DataSet();
            adapter.Fill(ds);
            if (odt == null && isdisposeConn) connection.Close();
            command.CommandType = CommandType.Text;
            return ds;
        }


        /// <summary>
        /// 执行一个返回结果集的存储过程
        /// </summary>
        /// <param name="ProcedureName"></param>
        /// <returns></returns>
        public DataSet ExecProcedure(string ProcedureName,ref OleDbParameter[] parameters)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            command.Parameters.Clear();
            command.Parameters.AddRange(parameters);
            command.CommandText = ProcedureName;
            command.CommandType = CommandType.StoredProcedure;
            adapter.SelectCommand = command;
            var ds = new DataSet();
            adapter.Fill(ds);
            if (odt == null && isdisposeConn) connection.Close();
            command.CommandType = CommandType.Text;
            return ds;
        }


        /// <summary>
        ///  返回查询语句 结果的第一行 第一列的内容
        /// </summary>
        /// <param name="sql">查询语句</param>
        /// <returns></returns>
        public object ExecScalar(string sql)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            command.CommandText = sql;
            //command.Connection = connection;
            object result = command.ExecuteScalar();
            command.Cancel();
            if (odt == null && isdisposeConn) connection.Close();
            return result;
        }

        public object ExecScalar(string sql, OleDbParameter[] parameters)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            command.Parameters.Clear();
            command.Parameters.AddRange(parameters);
            command.CommandText = sql;
            object result = command.ExecuteScalar();
            command.Parameters.Clear();
            if (odt == null && isdisposeConn) connection.Close();
            return result;

        }


        /// <summary>
        /// 执行查询（多语句）
        /// </summary>
        /// <param name="sql">查询语句</param>
        /// <returns>数据集</returns>
        public DataSet ExecQuery(string[] sql)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var ds = new DataSet();
            foreach (string s in sql)
            {
                var dt = new DataTable();
                command.CommandText = s;
                adapter.SelectCommand = command;
                adapter.Fill(dt);
                ds.Tables.Add(dt);
                sqlList.Add(dt.GetHashCode(), s);
                dt.Disposed += DataTable_Disposed;
            }
            if (odt == null && isdisposeConn) connection.Close();
            return ds;
        }

        /// <summary>
        /// 判断查询是否存在记录
        /// </summary>
        /// <param name="sql">查询语句</param>
        /// <returns>如果该查询有返回记录则为true，否则为false</returns>
        public bool HasRecord(string sql)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var dt = new DataTable();
            command.CommandText = sql;
            adapter.SelectCommand = command;
            if (adapter != null) adapter.Fill(dt);
            if (odt == null && isdisposeConn) connection.Close();
            return dt.Rows.Count != 0;
        }



        /// <summary>
        ///  判断是否搜索到数据
        /// </summary>
        /// <param name="sql">sql字符串</param>
        /// <param name="parameters">参数</param>
        /// <returns></returns>
        public bool HasRecord(string sql, OleDbParameter[] parameters)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var dt = new DataTable();
            command.CommandText = sql;
            command.Parameters.Clear();
            command.Parameters.AddRange(parameters);
            int result = command.ExecuteNonQuery();
            // command.Parameters.Clear();;
            adapter.SelectCommand = command;
            if (adapter != null) adapter.Fill(dt);
            if (odt == null && isdisposeConn) connection.Close();
            return dt.Rows.Count != 0;
        }


        /// <summary>
        /// 获取数据表
        /// </summary>
        /// <param name="tableName">数据表名称</param>
        /// <returns>数据表</returns>
        public DataTable GetTable(string tableName)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var dt = new DataTable();
            command.CommandText = "Select * From " + tableName;
            adapter.SelectCommand = command;
            adapter.Fill(dt);
            sqlList.Add(dt.GetHashCode(), command.CommandText);
            dt.Disposed += DataTable_Disposed;
            if (odt == null && isdisposeConn) connection.Close();
            return dt;
        }

        /// <summary>
        /// 使用特定的查询语句填充数据表
        /// </summary>
        /// <param name="dataTable">要填充的数据表</param>
        /// <param name="sql">查询语句</param>
        public void Fill(DataTable dataTable, string sql)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            int key = dataTable.GetHashCode();
            if (!sqlList.ContainsKey(key))
            {
                sqlList.Add(key, sql);
                dataTable.Disposed += DataTable_Disposed;
            }
            command.CommandText = sql;
            adapter.SelectCommand = command;
            adapter.Fill(dataTable);
            if (odt == null && isdisposeConn) connection.Close();
        }

        /// <summary>
        /// 使用特定的查询语句填充数据集
        /// </summary>
        /// <param name="dataset">要填充的数据集</param>
        /// <param name="sql">查询语句</param>
        public void Fill(DataSet dataset, string[] sql)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            dataset.Tables.Clear();
            foreach (string s in sql)
            {
                var dt = new DataTable();
                command.CommandText = s;
                adapter.SelectCommand = command;
                adapter.Fill(dt);
                dataset.Tables.Add(dt);
                sqlList.Add(dt.GetHashCode(), s);
                dt.Disposed += DataTable_Disposed;
            }
            if (odt == null && isdisposeConn) connection.Close();
        }

        /// <summary>
        /// 更新表中的数据至数据库，仅能更新由该实例获取的DataTable对象
        /// </summary>
        /// <param name="dataTable">要执行更新的表</param>
        public void Update(DataTable dataTable)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            int key = dataTable.GetHashCode();
            if (!sqlList.ContainsKey(key))
                throw new Exception("该DataTable不是由本" + ToString() + "创建，无法更新！");
            string sql = sqlList[key];
            command.CommandText = sql;
            adapter.SelectCommand = command;
            commandBuilder.RefreshSchema();
            adapter.Update(dataTable);
            if (odt == null && isdisposeConn) connection.Close();
        }


        /// <summary>
        /// 更新表中的数据至数据库，仅能更新由该实例获取的DataTable对象
        /// </summary>
        /// <param name="dataTable">要执行更新的表</param>
        public void Update(DataTable dataTable, string sqlstring)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();

            ;
            string sql = sqlstring;
            command.CommandText = sql;
            adapter.SelectCommand = command;
            commandBuilder.RefreshSchema();
            adapter.Update(dataTable);
            if (odt == null && isdisposeConn) connection.Close();
        }

        /// <summary>
        /// 根据Dictionary生成sqlparameter数组      蒋成龙 
        /// </summary>
        /// <param name="ht">Dictionary</param>
        /// <returns>sqlparameter数组</returns>
        public OleDbParameter[] sqlparameter_append(Dictionary<string, string> dc)
        {
            int count = dc.Count;
            OleDbParameter[] paralist;
            if (count == 0)//空sqlparameter
            {
                paralist = new OleDbParameter[0];
            }
            else
            {

                paralist = new OleDbParameter[dc.Count];
                int i = 0;
                foreach (KeyValuePair<string, string> kvp in dc)
                {
                    paralist[i] = new OleDbParameter("@" + kvp.Key, kvp.Value);
                    i++;
                }
            }
            return paralist;
        }


        /// <summary>
        /// 执行存储过程,返回执行是否成功,ok代表成功,否则直接返回错误信息
        /// </summary>
        /// <param name="ProcedureName">存储过程名字</param>
        /// <param name="parameters">Dictionary</param>
        /// <returns>是否执行成功</returns>
        public string ExecProcedureBool(string ProcedureName, OleDbParameter[] oledb)
        {
            string flag;
                if (connection.State == ConnectionState.Closed)
                    connection.Open();
                using (OleDbCommand command = new OleDbCommand(null, connection))
                {
                    //command.Parameters.Clear();
                    command.Parameters.AddRange(oledb);
                    OleDbParameter[] ol = new OleDbParameter[1];
                    ol[0] = new OleDbParameter("@flag", OleDbType.VarChar, 50);
                    ol[0].Direction = ParameterDirection.Output;
                    command.Parameters.AddRange(ol);
                    command.CommandText = ProcedureName;
                    command.CommandType = CommandType.StoredProcedure;
                    OleDbDataAdapter adapter = new OleDbDataAdapter();
                    adapter.SelectCommand = command;
                    adapter.SelectCommand.ExecuteNonQuery();
                    flag = command.Parameters["@flag"].Value.ToString();
                    command.Parameters.Clear();
                }
            return flag;
        }



        /// <summary>
        /// 更新数据集中的数据至数据库，仅能更新由该实例获取的DataSet对象
        /// </summary>
        /// <param name="dataSet">要执行更新的数据集</param>
        public void Update(DataSet dataSet)
        {
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            foreach (DataTable dt in dataSet.Tables)
            {
                int key = dt.GetHashCode();
                if (!sqlList.ContainsKey(key))
                    throw new Exception("该DataTable不是由本" + ToString() + "创建，无法更新！");
                string sql = sqlList[key];
                command.CommandText = sql;
                adapter.SelectCommand = command;
                commandBuilder.RefreshSchema();
                adapter.Update(dt);
            }
            if (odt == null && isdisposeConn) connection.Close();
        }

        public string GetId(string dwbm)
        {
            var rd = new Random();
            return dwbm + DateTime.Now.ToString("yyyyMMdd") + DateTime.Now.ToString("HHmmssfff") +
                   rd.Next(99999).ToString("00000");
        }

        private void DataTable_Disposed(object sender, EventArgs e)
        {
            sqlList.Remove(sender.GetHashCode());
        }


        public void BeginTrans()
        {
            if (odt == null)
            {
                odt = connection.BeginTransaction();
                command.Transaction = odt;
            }
        }


        public void Commit()
        {
            if (odt != null)
            {
                odt.Commit();
                command.Transaction = null;
                odt.Dispose();
                odt = null;
            }
        }

        public void Rollback()
        {
            if (odt != null)
            {
                odt.Rollback();
                command.Transaction = null;
                odt.Dispose();
                odt = null;
            }
        }

        //对象写入到数据库中
        public void Obj2Db(CDataAccessBase obj, string PkValue)
        {
            //修改版本号

            string tableName = obj.TableName;
            using (
                DataTable dt =
                    ExecQuery("select * from " + tableName + " where " + obj.PKFieldName + "='" +
                              (PkValue) + "' "))
            {
                obj.beforSave(this);
                DataRow dr;
                if (dt.Rows.Count > 0)
                {
                    dr = dt.Rows[0];
                    obj.proptertyToDataTable(dr);
                }
                else
                {
                    dr = dt.NewRow();
                    obj.proptertyToDataTable(dr);
                    dt.Rows.Add(dr);
                }

                Update(dt);
                obj.afterSave(this);
            }
        }

        public void Obj2Db(CDataAccessBase obj)
        {
            Obj2Db(obj, obj.getPkValue().ToString());
        }


        //从数据库中恢复对象
        public void Db2Obj(CDataAccessBase obj, string PkValue)
        {
            string tableName = obj.TableName;
            using (
                DataTable dt =
                    ExecQuery("select * from " + tableName + " where " + obj.PKFieldName + "='" +
                              (PkValue) + "' "))
            {
                DataRow dr;
                if (dt.Rows.Count > 0)
                {
                    dr = dt.Rows[0];
                    obj.InitFromDataRow(dr);
                }
                else
                {
                    obj = null;
                }
            }
        }




        public void Db2Obj(CDataAccessBase obj)
        {
            Db2Obj(obj, obj.getPkValue().ToString());
        }

        /// <summary>
        /// 获取对象列表
        /// </summary>
        /// <param name="obj">对象</param>
        /// <returns></returns>
        public CDataAccessBase[] getObjList(IDataAccessBase obj)
        {
            CClassQuery ccq = obj.generateListSql();
            if (ccq == null) return null;
            Type t = obj.GetType();

            DataTable dt = ExecQuery(ccq.sql, ccq.parameters);
            CDataAccessBase[] ba = null;
            if (dt.Rows.Count > 0)
            {
                ba = new CDataAccessBase[dt.Rows.Count];
            }
            int i = 0;
            foreach (DataRow dr in dt.Rows)
            {
                var k = (CDataAccessBase)Activator.CreateInstance(t);
                k.InitFromDataRow(dr);
                ba[i] = k;
                i++;
            }

            return ba;
        }


        public CDataAccessBase[] getObjList(IDataAccessBase obj, int PageIndex, int PageSize)
        {
            Type t = obj.GetType();

            DataTable dt = ExecProcedure("AllowPaging", new[]
                                                            {
                                                                new OleDbParameter("@pageindex", PageIndex),
                                                                new OleDbParameter("@PageSize", PageSize),
                                                                new OleDbParameter("@tsql", obj.generateListSql())
                                                            }
                ).Tables[0];


            CDataAccessBase[] ba = null;
            if (dt.Rows.Count > 0)
            {
                ba = new CDataAccessBase[dt.Rows.Count];
            }
            int i = 0;
            foreach (DataRow dr in dt.Rows)
            {
                var k = (CDataAccessBase)Activator.CreateInstance(t);
                k.InitFromDataRow(dr);
                ba[i] = k;
                i++;
            }

            return ba;
        }


        /// <summary>
        /// 判断对象是否在数据库中存在
        /// </summary>
        /// <param name="obj">对象</param>
        /// <returns></returns>
        public bool CheckObj(IDataAccessBase obj)
        {
            CClassQuery ccq = obj.CheckObj();
            if (ccq == null) return false;
            return HasRecord(ccq.sql, ccq.parameters);
        }

        /// <summary>
        /// 从数据库中删除对象
        /// </summary>
        /// <param name="Obj">对象</param>
        /// <param name="PkValue">主键值</param>
        public void deleteObj(CDataAccessBase Obj, string PkValue)
        {
            ExecSQL("delete " + Obj.TableName + " where " + Obj.PKFieldName + "='" + PkValue + "'");
        }

        public void deleteObj(CDataAccessBase Obj)
        {
            deleteObj(Obj, Obj.getPkValue().ToString());
        }

        public CDataAccessBase getObj(IDataAccessBase obj)
        {
            CClassQuery ccq = obj.CheckObj();
            if (ccq == null) return null;
            Type t = obj.GetType();

            DataTable dt = this.ExecQuery(ccq.sql, ccq.parameters);
            CDataAccessBase ba = null;

            int i = 0;
            foreach (DataRow dr in dt.Rows)
            {
                CDataAccessBase k = (CDataAccessBase)Activator.CreateInstance(t);
                k.InitFromDataRow(dr);
                ba = k;
                break;
            }
            return ba;
        }



        public CDataAccessBase[] getObjList(IDataAccessBase obj, int RecordCount)
        {
            CClassQuery ccq = obj.generateListSql(RecordCount);
            if (ccq == null) return null;
            Type t = obj.GetType();

            DataTable dt = this.ExecQuery(ccq.sql, ccq.parameters);
            CDataAccessBase[] ba = null;
            if (dt.Rows.Count > 0)
            {
                ba = new CDataAccessBase[dt.Rows.Count];
            }
            int i = 0;
            foreach (DataRow dr in dt.Rows)
            {
                CDataAccessBase k = (CDataAccessBase)Activator.CreateInstance(t);
                k.InitFromDataRow(dr);
                ba[i] = k;
                i++;
            }

            return ba;
        }


        public static string getPagingSqlStr(string Tsql, int PageIndex, int PageSize)
        {
            string pattern = @"\w*order\s+by\w*";
            RegexOptions options = RegexOptions.None | RegexOptions.IgnoreCase;
            Regex regex = new Regex(pattern, options);
            MatchCollection matches = regex.Matches(Tsql);
            string G = "";
            int OrderByIndex = 0;
            string selectString = "";
            string OrderByString = "";
            string sqlstr = "";
            if (matches.Count > 0)
            {
                G = matches[0].Value;
                OrderByIndex = Tsql.IndexOf(G);
                OrderByString = Tsql.Substring(OrderByIndex);
                selectString = Tsql.Substring(0, OrderByIndex - 1);
                sqlstr = @"select * from (SELECT  ROW_NUMBER() OVER(" + OrderByString + ") AS AllowPagingId,* FROM (" + selectString + ") as table1) as table2 where AllowPagingId between " + ((PageIndex - 1) * PageSize + 1).ToString() + " and " + (PageIndex * PageSize).ToString();
            }
            else
            {
                selectString = Tsql;
                sqlstr = @"select *  from (SELECT  *,ROW_NUMBER() OVER(ORDER BY orderbyID DESC) AS AllowPagingId FROM  ( select *, 1 as orderbyID from (" + selectString + ")  as  tbs1 )   as Tabl1 ) as table2 where AllowPagingId between " + ((PageIndex - 1) * PageSize + 1).ToString() + " and   " + (PageIndex * PageSize).ToString(); ;
            }
            return sqlstr;
        }


        public DataSet Paging(string Tsql, OleDbParameter[] SqlParams, int PageIndex, int PageSize, bool getPageInfo = true)
        {
            string pattern = @"\w*order\s+by\w*";
            RegexOptions options = RegexOptions.None | RegexOptions.IgnoreCase;
            Regex regex = new Regex(pattern, options);
            MatchCollection matches = regex.Matches(Tsql);
            string G = "";
            int OrderByIndex = 0;
            string selectString = "";
            string OrderByString = "";
            string sqlstr = "";
            if (matches.Count > 0)
            {
                G = matches[0].Value;
                OrderByIndex = Tsql.IndexOf(G);
                OrderByString = Tsql.Substring(OrderByIndex);
                selectString = Tsql.Substring(0, OrderByIndex - 1);
                sqlstr = @"select * from (SELECT  ROW_NUMBER() OVER(" + OrderByString + ") AS AllowPagingId,* FROM (" + selectString + ") as table1) as table2 where AllowPagingId between " + ((PageIndex - 1) * PageSize + 1).ToString() + " and " + (PageIndex * PageSize).ToString();
            }
            else
            {
                selectString = Tsql;
                sqlstr = @"select *  from (SELECT  *,ROW_NUMBER() OVER(ORDER BY orderbyID DESC) AS AllowPagingId FROM  ( select *, 1 as orderbyID from (" + selectString + ")  as  tbs1 )   as Tabl1 ) as table2 where AllowPagingId between " + ((PageIndex - 1) * PageSize + 1).ToString() + " and   " + (PageIndex * PageSize).ToString(); ;
            }

            DataTable dt = ExecQuery(sqlstr, SqlParams);
            int aa = dt.Rows.Count;
            string sqlStr2 = @"select case when count(*)%" + PageSize + "=0 then count(*)/" + PageSize + "  when count(*)%" + PageSize + "<>0 then count(*)/" + PageSize + "+1 end as pageCount,count(*) as RowsCount from (" + selectString + ") as tab1";

            DataSet ds = new DataSet();
            ds.Tables.Add(dt);
            if (getPageInfo)
            {
                DataTable dt2 = ExecQuery(sqlStr2, SqlParams);
                ds.Tables.Add(dt2);
            }
            int a = ds.Tables[0].Rows.Count;
            return ds;
        }


        public DataSet Pagingaccess(string Tsql, OleDbParameter[] SqlParams, int PageIndex, int PageSize,int RowCount)
        {
            int StartIndex = 1;//起始位置
            int EndIndex = 1;//终止位置
            int PageCount = RowCount % PageSize == 0 ? RowCount / PageSize : (RowCount / PageSize) + 1;//总页数
            StartIndex = (PageIndex-1) * PageSize;
            EndIndex = PageIndex * PageSize;
            if (EndIndex > RowCount)
            {
                EndIndex = EndIndex + (RowCount - EndIndex);
                //

            }
            DataSet ds = new DataSet();
            if (connection.State == ConnectionState.Closed)
                connection.Open();
            var dt = new DataTable();
            command.Parameters.Clear();
            command.Parameters.AddRange(SqlParams);
            command.CommandText = Tsql;
            adapter.SelectCommand = command;
            adapter.Fill(ds, StartIndex, PageSize, "datatable");
            dt.Disposed += DataTable_Disposed;
            command.Parameters.Clear();
            if (odt == null && isdisposeConn) connection.Close();
            //return dt;
            return ds;
        }
    }
}