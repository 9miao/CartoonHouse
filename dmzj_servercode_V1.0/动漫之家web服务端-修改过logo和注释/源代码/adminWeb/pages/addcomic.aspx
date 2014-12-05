<%@ Page Title="" Language="C#" MasterPageFile="~/pages/Site1.Master" AutoEventWireup="true" CodeBehind="addcomic.aspx.cs" Inherits="adminWeb.pages.addcomic" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <div class="right_wrap">
        <div class="guidewrap">当前位置：<a href="#">后台管理</a> > <a href="#">漫画管理</a></div>
        <div class="box_wrap1 fixed">
        </div>
        <div class="box_wrap2 fixed">
            <form runat="server">
                <table width="100%" border="0" cellspacing="0" cellpadding="0" class="tb02">
                    <tr>
                        <th>名称：</th>
                        <td>
                            <asp:TextBox ID="TextBox1" runat="server" CssClass="input01"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <th>简介：</th>
                        <td>
                            <asp:TextBox ID="TextBox2" runat="server" CssClass="input01" TextMode="MultiLine" Height="30"></asp:TextBox>
                        </td>
                    </tr>
                    <%
                        if (dttype != null && thisdttypes != null && dt01 != null)
                        {
                            foreach (System.Data.DataRow dr_ in dt01.Rows)
                            {
                    %>
                    <tr>
                        <th width="70"><%=dr_[2]%>：</th>
                        <td>
                            <%
                                        foreach (System.Data.DataRow dr in dttype.Select("C00101='" + dr_[0].ToString() + "'"))
                                        {
                            %>
                            <input type="checkbox" value="<%=dr[0] %>" name="checkname" <%=thisdttypes.Select("C00008='"+dr[0].ToString()+"'").Length>0?"checked=\"checked\"":"" %> /><%=dr[2] %>
                            <%
                                        }
                             
                            %> 
                        </td>
                    </tr>
                    <%
                                    }
                                } %>
                    <tr>
                        <th>图片地址：</th>
                        <td>
                            <asp:TextBox ID="TextBox4" runat="server" CssClass="input01"></asp:TextBox></td>
                    </tr>
                    <tr>
                        <th>作者：</th>
                        <td>
                            <asp:TextBox ID="TextBox5" runat="server" CssClass="input01"></asp:TextBox></td>
                    </tr>

                    <tr>
                        <th>介绍：</th>
                        <td>
                            <asp:TextBox ID="TextBox6" runat="server" CssClass="input01" TextMode="MultiLine" Height="50"></asp:TextBox>
                        </td>
                    </tr>

                    <tr>
                        <th>&nbsp;</th>
                        <td>
                            <div class="btn_wrap">
                                <asp:Button ID="Button1" runat="server" Text="保存" CssClass="btn01" OnClick="Button1_Click" />
                            </div>
                        </td>
                    </tr>
                </table>
            </form>
            <div class="height40"></div>
        </div>
    </div>
</asp:Content>
