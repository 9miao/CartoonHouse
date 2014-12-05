<%@ Page Title="" Language="C#" MasterPageFile="~/pages/Site1.Master" AutoEventWireup="true" CodeBehind="classify.aspx.cs" Inherits="adminWeb.pages.classify" %>

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
        	<div class="fleft"><input id="addClassfy" name="" type="button" value="添加大分类" class="btn01"/></div>
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
                <td><%=dr["C00001"] %></td>
                <td class="tleft"><%=dr["C00003"] %></td>
                <%--<td><%=dr["C00002"] %></td>--%>
                <td><a name="update" attrid="<%=dr["C00001"] %>" style="cursor:pointer;" class="txtcolor_yellow">修改</a>&nbsp;&nbsp;<a href="downclassify.aspx?id=<%=dr["C00001"] %>" style="cursor:pointer;" class="txtcolor_red">下级分类</a>&nbsp;&nbsp;<a href="#" onclick="delthis('<%=dr[0] %>')" style="cursor:pointer;" class="txtcolor_red">删除</a></td>
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
             <%-- <tr>
                <td>图片：</td>
                <td><input id="classify_img" type="text" class="input02" /></td>
              </tr>--%>
              <tr>
                <td colspan="2"><input id="Sok" type="button" value="确定" class="btn01"/></td>
              </tr>
            </table>
        </div>
    </div>
        <script>
            var _tid;
            var pageii;
            $("[name='update']").click(function () {
                var _id = $(this).attr("attrid");
                _tid = _id;
                //优先获取数据
                $.ajax({
                    url: "../Handler/Getclassify.ashx?" + Math.random(),
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
                $("#classify_name").val("");
                _tid = "";
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
                    url: "../Handler/classify.ashx?" + Math.random(),
                    type: "post",
                    dataType: "json",
                    data: {
                        id: _id,
                        name: $("#classify_name").val()
                    }, success: function (data) {
                        if (data.Status == "Success") {
                            layer.alert(data.Msg, 1, function () {
                                window.location.href = 'classify.aspx?page=<%=Request.QueryString["page"]%>';
                            });
                        } else {
                            layer.alert(data.Msg, 8);
                        }
                    }
                });
            });

            function delthis(thisid) {
                if(confirm("分类删除后无法回复,确认要删除吗?"))
                $.ajax({
                    url: "../Handler/delclassify.ashx?" + Math.random(),
                    type: "post",
                    dataType: "json",
                    data: {
                        thisid: thisid,
                    }, success: function (data) {
                        if (data== "0") {
                            alert("删除成功");
                            window.location.href = 'classify.aspx?page=<%=Request.QueryString["page"]%>';
                        } else {
                            alert(data);
                        }
                    }
                 });
            }
        </script>
        </form>
</asp:Content>
