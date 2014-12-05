<%@ Page Title="" Language="C#" MasterPageFile="~/pages/Site1.Master" AutoEventWireup="true" CodeBehind="comic.aspx.cs" Inherits="adminWeb.pages.comic" %>

<%@ Register Src="~/publicControl/PageInfoControl.ascx" TagPrefix="uc1" TagName="PageInfoControl" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <form runat="server">
        <div class="right_wrap">
            <div class="guidewrap">当前位置：<a href="#">后台管理</a> > <a href="#">漫画管理</a></div>
            <div class="box_wrap1 fixed">
                <div class="fleft">
                    <input name="" type="button" value="添加漫画" class="btn01" onclick="window.location.href = 'addcomic.aspx'" />
                </div>
                <div class="mainsearch_wrap">
                    <span>
                        <asp:TextBox ID="Textserch" runat="server" CssClass="mainsearch_input"></asp:TextBox>
                    </span>
                    <asp:Button ID="BtnSerch" runat="server" Text="搜索" CssClass="mainsearch_btn" OnClick="BtnSerch_Click" />
                </div>
            </div>
            <div class="box_wrap2 fixed">
                <table width="100%" border="0" cellspacing="0" cellpadding="0" class="tb01">
                    <tr>
                        <th>ID</th>
                        <th>漫画名称</th>
                        <th>更新时间</th>
                        <th>漫画图片</th>
                        <th>功能</th>
                    </tr>
                    <% if (_dt != null)
                       {
                           foreach (System.Data.DataRow dr in _dt.Rows)
                           {
                    %>
                    <tr>
                        <td><%=dr["C00010"] %></td>
                        <td class="tleft"><%=dr["C00011"] %></td>
                        <td><%=adminWeb.adminClass.exPagers.IntToDatetime(dr["C00014"].ToString()) %></td>
                        <td><%=dr["C00013"].ToString().Length>25?dr["C00013"].ToString().Substring(0,25)+"...":dr["C00013"].ToString() %></td>
                        <td><a href="addcomic.aspx?id=<%=dr[0] %>" class="txtcolor_yellow">修改</a>&nbsp;&nbsp;<a href="chap.aspx?id=<%=dr["C00010"] %>" class="txtcolor_red">章节</a>&nbsp;&nbsp;<a href="#" class="txtcolor_red" onclick="delthis('<%=dr[0] %>')">删除</a></td>
                    </tr>
                    <%}
                       }
                    %>
                </table>
                <uc1:PageInfoControl runat="server" ID="PageInfoControl" />
                <div class="height40"></div>
            </div>
        </div>
    </form>
    <script>

        function delthis(thisid) {
            if (confirm("删除后对应章节将全部删除,无法恢复")) {
                $.ajax({
                    type: "post",
                    url: "../Handler/delmanhua.ashx?A=" + Math.random(),
                    dataType: "text",
                    data: { thisid: thisid },
                    success: function (data) {
                        if (data == "0") {
                            alert("删除成功");
                            window.location.href = "comic.aspx?value=<%=Request.QueryString["page"]%>&kw=<%=Request.QueryString["kw"]%>&id=<%=Request.QueryString["id"]%>";
                        }
                        else
                            alert(data);
                    }
                });
            }
        }
    </script>
</asp:Content>
