<%@ Page Title="" Language="C#" MasterPageFile="~/pages/Site1.Master" AutoEventWireup="true" CodeBehind="chap.aspx.cs" Inherits="adminWeb.pages.chap" %>
<%@ Register Src="~/publicControl/PageInfoControl.ascx" TagPrefix="uc1" TagName="PageInfoControl" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
     <form id="Form1" runat="server">
     <div class="right_wrap">
    	<div class="guidewrap">当前位置：<a href="#">后台管理</a> > <a href="#">章节信息</a></div>
        <div class="box_wrap1 fixed">
        	<div class="fleft"><input name="" type="button" value="添加章节" class="btn01"/></div>
            <div class="mainsearch_wrap"><span>
                <asp:TextBox ID="Textserch" runat="server" CssClass="mainsearch_input"></asp:TextBox> </span>
                <asp:Button ID="BtnSerch" runat="server" Text="搜索" CssClass="mainsearch_btn" OnClick="BtnSerch_Click" /> </div>
        </div>
        <div class="box_wrap2 fixed">
        	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="tb01">
              <tr>
                <th>ID</th>
                <th>话名称</th>
                <th>功能</th>
              </tr>
                <% if(_dt!=null)
                   {
                       foreach (System.Data.DataRow dr in _dt.Rows)
                       {
                       %>
              <tr>
                <td><%=dr["C00029"] %></td>
                <td class="tleft"><%=dr["C00031"] %></td>
                <td><a href="#" class="txtcolor_yellow">修改</a>&nbsp;&nbsp;<a href="pic.aspx?id=<%=dr["C00029"] %>" class="txtcolor_red">图片</a>&nbsp;&nbsp;<a href="#" class="txtcolor_red">删除</a></td>
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
</asp:Content>
