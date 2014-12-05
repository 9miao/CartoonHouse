<%@ Page Title="" Language="C#" MasterPageFile="~/pages/Site1.Master" AutoEventWireup="true" CodeBehind="comicInfo.aspx.cs" Inherits="adminWeb.pages.comicInfo" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
     <script src="../javaScript/jquery-1.7.1.js"></script>
    <script src="../laryer/layer.min.js"></script>
    <link href="../laryer/skin/layer.css" rel="stylesheet" />
    <link href="../laryer/skin/layer.ext.css" rel="stylesheet" />
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">s
    <div class="right_wrap">
    	<div class="guidewrap">当前位置：<a href="#">后台管理</a> > <a href="#">漫画信息</a></div>
        <div class="box_wrap1 fixed">
        	<div class="fleft"><input name="" type="button" value="添加教程" class="btn01"/></div>
            <div class="mainsearch_wrap"><span><input name="" type="text" value="搜索"  class="mainsearch_input"/></span><input name="" type="button" value="搜索" class="mainsearch_btn" /></div>
        </div>
        <div class="box_wrap2 fixed">
        	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="tb02">
              <tr>
                <th>漫画名称：</th>
                <td><asp:TextBox ID="textName" runat="server" CssClass="input02"></asp:TextBox></td>
              </tr>
              <tr>
                <th>last_update_chapter_name：</th>
                <td><asp:TextBox ID="Text_last_update_chapter_name" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
              <tr>
                <th>Cover：</th>
                <td><asp:TextBox ID="Text_Cover" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
              <tr>
                <th>hot_hits：</th>
                <td><asp:TextBox ID="Text_hot_hits" runat="server" CssClass="input03"></asp:TextBox></td>
              </tr>
              <tr>
                <th>Title：</th>
                <td><asp:TextBox ID="Text_Title" runat="server" CssClass="input04"></asp:TextBox></td>
              </tr>
              <tr>
                <th>Subtitle：</th>
                <td><asp:TextBox ID="Text_Subtitle" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
              <tr>
                <th>Types：</th>
                <td><asp:TextBox ID="Text_Types" runat="server" CssClass="input03"></asp:TextBox></td>
              </tr>
              <tr>
                <th>Zone：</th>
                <td><asp:TextBox ID="Text_Zone" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
                <tr>
                <th>Status：</th>
                <td><asp:TextBox ID="Text_Status" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
                 <tr>
                <th>Authors：</th>
                <td><asp:TextBox ID="Text_Authors" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
                 <tr>
                <th>first_letter：</th>
                <td><asp:TextBox ID="Text_first_letter" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
                 <tr>
                <th>Direction：</th>
                <td><asp:TextBox ID="Text_Direction" runat="server" CssClass="input01"></asp:TextBox></td>
              </tr>
              <tr>
                <th class="auto-style1">Description：</th>
                <td class="auto-style1"><textarea id="Text_Description" runat="server" cols="" rows="3" class="textarea"></textarea></td>
              </tr>
              <tr>
                <th>&nbsp;</th>
                <td><div  class="btn_wrap"><input name="" type="button" value="确定" class="btn01"/></div></td>
              </tr>
            </table>

        	<div class="height40"></div>
        </div>
    </div>
</asp:Content>
