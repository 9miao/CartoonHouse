<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="login.aspx.cs" Inherits="adminWeb.login" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>后台管理系统</title>
    <style type="text/css">
        * {
            padding: 0px;
            margin: 0px;
        }

        body {
            background: #EFEFEF;
            font-size: 14px;
            color: #666;
            font-family: Tahoma, Geneva, sans-serif, "微软雅黑", "宋体";
        }

        .tb {
            border-collapse: collapse;
        }

            .tb td {
                padding: 7px;
                font-weight: bold;
            }

        h1 {
            font-size: 30px;
            font-weight: bold;
            text-align: center;
            color: #fc923c;
            height: 80px;
        }

        .box {
            background: #FFF;
            height: 300px;
            width: 500px;
            margin: 150px auto;
            border: 5px solid #9d6c43;
            padding-top: 20px;
        }

        .input {
            width: 300px;
            height: 10px;
            padding: 6px;
            border: 1px solid #ccc;
        }

        .btn {
            width: 100px;
            height: 30px;
            border: none;
            background: #f8be6b;
            color: #9d6c43;
            font-weight: bold;
            margin-top: 30px;
            font-size: 14px;
            border: 1px solid #de9429;
            cursor: pointer;
        }
    </style>
</head>

<body>
    <div class="box">
        <table width="500" border="0" cellspacing="0" cellpadding="0" class="tb" align="center">
            <tr>
                <td>
                    <h1>后台管理系统</h1>
                </td>
            </tr>
        </table>
        <form runat="server">
            <table width="500" border="0" cellspacing="0" cellpadding="0" align="center" class="tb">
                <tr>
                    <td width="98" align="right">用户名：</td>
                    <td width="402">
                        <asp:TextBox ID="TextBox1" runat="server" Text="admin" CssClass="input"></asp:TextBox></td>
                </tr>
                <tr>
                    <td align="right">密&nbsp;&nbsp;&nbsp;码：</td>
                    <td>
                        <asp:TextBox ID="TextBox2" runat="server" TextMode="Password" CssClass="input"></asp:TextBox></td>
                </tr>
                <tr>
                    <td colspan="2" align="center">
                        <asp:Button ID="Button1" runat="server" Text="登录" CssClass="btn" OnClick="Button1_Click" />
                    </td>
                </tr>
            </table>
        </form>
        <br />
        <br />
    </div>
</body>
</html>
