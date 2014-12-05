<%@ Page Title="" Language="C#" MasterPageFile="~/pages/Site1.Master" AutoEventWireup="true" CodeBehind="downclassify.aspx.cs" Inherits="adminWeb.pages.downclassify" %>
<%@ Register Src="~/publicControl/PageInfoControl.ascx" TagPrefix="uc1" TagName="PageInfoControl" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
     <script src="../javaScript/jquery-1.7.1.js"></script>
    <script src="../laryer/layer.min.js"></script>
    <link href="../laryer/skin/layer.css" rel="stylesheet" />
    <link href="../laryer/skin/layer.ext.css" rel="stylesheet" />
     <form id="form1" runat="server">
    <div class="right_wrap">
    	<div class="guidewrap">当前位置：<a href="#">后台管理</a> > <a href="#">分类管理</a></div>
        <div class="box_wrap1 fixed">
        	<div class="fleft"><input id="addClassfy" name="" type="button" value="添加分类" class="btn01"/></div>
            <div class="mainsearch_wrap"></div>
        </div>
        <div class="box_wrap2 fixed">
        	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="tb01">
              <tr>
                <th>ID</th>
                <th>分类名称</th>
                <%--<th>图片</th>--%>
                <th>操作</th>
              </tr>
                <% if(_dt!=null){
                       foreach (System.Data.DataRow dr in _dt.Rows)
                       {
                        %>
              <tr>
                <td><%=dr["C00004"] %></td>
                <td class="tleft"><%=dr["C00006"] %></td>
                <td><a href="comic.aspx?id=<%=dr["C00004"] %>" style="cursor:pointer;" class="txtcolor_yellow">查看漫画</a>&nbsp;&nbsp;&nbsp;&nbsp;<a name="update" attrid="<%=dr["C00004"] %>" style="cursor:pointer;" class="txtcolor_yellow">修改</a>&nbsp;&nbsp;<a href="#" style="cursor:pointer;" class="txtcolor_red">删除</a></td>
              </tr>
                <%} } %>
            </table>
            <uc1:PageInfoControl runat="server" ID="PageInfoControl" />
        	<div class="height40"></div>
        </div>
        
            <div id="show_update" class="box_wrap2 fixed" style="display:none;width:400px;height:150px">
        	<table class="tb02">
              <tr style="width:500px">
                <td>分类名称：</td>
                <td><input id="classify_name" type="text" class="input02" /></td>
              </tr>
              <tr>
                <td colspan="2"><input id="Sok" type="button" value="确定" class="btn01"/></td>
              </tr>
            </table>
        </div>
    </div>
        <script>
            var _tid;
            var pageii;
            var _ttyoe = "1";
            $("[name='update']").click(function () {
                _ttyoe = "0";
                var _id = $(this).attr("attrid");
                _tid = _id;
                //优先获取数据
                $.ajax({
                    url: "../Handler/getclassiifydown.ashx?" + Math.random(),
                    type: "post",
                    dataType: "json",
                    data: {
                        id: _id
                    }, success: function (data) {
                        if (data.Status == "Success") {
                            $("#classify_name").val(data.classifyName);
                            pageii = $.layer({
                                type: 1,
                                area: ['auto', 'auto'],
                                title: false,
                                page: { dom: '#show_update' }
                            });
                        } else {
                            layer.alert(data.Msg, 8);
                        }
                    }
                });
            });

            function layer_close() {
                if (pageii != null)
                    layer.close(pageii);
            }

            $("#addClassfy").click(function () {
                _ttyoe = "1";
                $("#classify_name").val("");
                _tid = "<%=Request.QueryString["id"]%>";
                pageii = $.layer({
                    type: 1,
                    area: ['auto', 'auto'],
                    title: false,
                    page: { dom: '#show_update' }
                });
            });

            $("#Sok").click(function () {
                var _id = _tid;
                $.ajax({
                    url: "../Handler/classifydown.ashx?" + Math.random(),
                    type: "post",
                    dataType: "json",
                    data: {
                        id: _id,
                        name: $("#classify_name").val(),
                        type: _ttyoe
                    }, success: function (data) {
                        if (data.Status == "Success") {
                            layer.alert(data.Msg, 1, function () {
                                window.location.href = 'downclassify.aspx?page=<%=Request.QueryString["page"]%>&id=<%=Request.QueryString["id"]%>';
                            });
                        } else {
                            layer.alert(data.Msg, 8);
                        }
                    }
                });
            });

        </script>
        </form>
</asp:Content>
