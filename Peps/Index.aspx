<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Index.aspx.cs" Inherits="Peps.Index" %>

<%@ Register Assembly="System.Web.DataVisualization, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" Namespace="System.Web.UI.DataVisualization.Charting" TagPrefix="asp" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>PEPS</title>
    <!-- Bootstrap Core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet"/>

    <!-- Custom CSS -->
    <link href="css/sb-admin.css" rel="stylesheet"/>

    <!-- Morris Charts CSS -->
    <link href="css/plugins/morris.css" rel="stylesheet"/>

    <!-- Custom Fonts -->
    <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css"/>

    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.6/jquery.min.js" type="text/javascript"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js" type="text/javascript"></script>
    <link href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/themes/base/jquery-ui.css" rel="Stylesheet" type="text/css" />
    <script type="text/javascript">
        $(function () {
            var date = new Date(2005, 10, 30);
            $("[id$=DisplayCalendar]").datepicker({
                showOn: 'focus',
                defaultDate: date,
                changeYear: true,
                minDate: date,
                maxDate: new Date(2015, 1, 30)
            });
        });
    </script>
</head>
<body>
    <div id="wrapper">

        <!-- Navigation -->
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
    <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-ex1-collapse">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="Default.aspx">Gestion Moduleis</a>
            </div>

        <ul class="nav navbar-right top-nav">
                <li class="dropdown">
                    <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-user"></i> User <b class="caret"></b></a>
                    <ul class="dropdown-menu">
                        <li>
                            <a href="#"><i class="fa fa-fw fa-user"></i> Profile</a>
                        </li>
                        <li>
                            <a href="#" data-toggle="modal" data-target="#myModal"><i class="fa fa-fw fa-gear" ></i> Settings</a>
      
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#"><i class="fa fa-fw fa-repeat"></i> Restart</a>
                        </li>
                    </ul>
                </li>
            </ul>
        
            <!-- Sidebar Menu Items - These collapse to the responsive navigation menu on small screens -->
            <div class="collapse navbar-collapse navbar-ex1-collapse">
                <ul class="nav navbar-nav side-nav">
                    <li class="active">
                        <a href="Index.aspx"><i class="fa fa-fw fa-dashboard"></i> General</a>
                    </li>
                    <li>
                        <a href="Charts.aspx"><i class="fa fa-fw fa-bar-chart-o"></i> Charts</a>
                    </li>
                </ul>
            </div>
            <!-- /.navbar-collapse -->
        </nav>





                <div id="page-wrapper">

            <div class="container-fluid">

                <!-- Page Heading -->
                <div class="row">
                    <div class="col-lg-12">
                        <h1 class="page-header">
                            General <small>Product hedging</small>
                        </h1>
                        <div class="breadcrumb">
                                 <div class="row">
                                    <div class="col-lg-6">
                                        <span class="pull-left">  <i class="fa fa-dashboard"></i>  Current date: 30/11/2005 </span>
                                    </div>
                                    <div class="col-lg-6 text-center">
                                        <span class="pull-right"> 
                                            <button type="button" onclick="computeHedge" class="btn btn-xs btn-default"> 
                                            <span class="glyphicon glyphicon-refresh" ></span> Refresh</button></span>
                                                        
                                          </div>
                                </div>
                        </div>
                        </div>
                </div>
                <!-- /.row -->
                <!-- /.row -->
                        
                <div id ="FIRST" class="row">
                    <div class="col-lg-8">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h3 class="panel-title"><i class="fa fa-money fa-fw"></i> Portfolio composition</h3>
                            </div>
                            <div class="panel-body">
                                <div class="table-responsive">
                                     <asp:Table ID="stocksTable" runat="server" Width="100%" CssClass="table table-bordered table-hover table-striped"> 
                                            <asp:TableRow Font-Bold="True" BackColor="White">
                                                <asp:TableCell><b>ISIN</b></asp:TableCell>
                                                <asp:TableCell>Name</asp:TableCell>
                                                <asp:TableCell>Single Price</asp:TableCell>
                                                <asp:TableCell>Quantity (delta)</asp:TableCell>
                                                <asp:TableCell>Total value</asp:TableCell>
                                            </asp:TableRow>
                                         
                                      </asp:Table>  
                                 
                                </div>
                                <div class="text-right">
                                    <a href="#">View All Composition <i class="fa fa-arrow-circle-right"></i></a>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div class="col-lg-4">

                    <div class="row">
                        <div class="col-lg-9 ">
                        <div class="panel panel-red">
                            <div class="panel-heading">
                                <div class="row">
                                    <div class="col-xs-3">
                                        <i class="fa fa-trophy fa-3x"></i>
                                    </div>
                                    <div class="col-xs-9 text-right">
                                        <div class="huge">
                                        <span><asp:Literal ID="PnLDiv" runat="server" /></span> 
                                            </div>
                                        <div>%</div>
                                    </div>
                                </div>
                            </div>
                            <a href="#">
                                <div class="panel-footer">
                                    <span class="pull-left">Profit & Loss</span>
                                    <span class="pull-right"><i class="fa fa-money"></i></span>
                                    <div class="clearfix"></div>
                                </div>
                            </a>
                        </div>
                    </div>
                    </div>
                    <div class="row">
                            <div class="col-lg-9 ">
                                <div class="panel panel-yellow">
                                  <div class="panel-heading">
                                      <div class="row">
                                       <div class="col-xs-3">
                                        <i class="fa fa-euro fa-3x"></i>
                                       </div>
                                      <div class="col-xs-9 text-right">
                                        <div class="huge">
                                        <span><asp:Literal ID="PtfValue" runat="server" /></span> 
                                            </div>
                                        <div>euro</div>
                                      </div>
                                   </div>
                                </div>
                            <a href="#">
                                <div class="panel-footer">
                                    <span class="pull-left">Portfolio value</span>
                                    <span class="pull-right"><i class="fa fa-money"></i></span>
                                    <div class="clearfix"></div>
                                </div>
                            </a>
                                </div>
                            </div>
                    </div>
                    <div class="row">
                            <div class="col-lg-9 ">
                                <div class="panel panel-primary">
                                  <div class="panel-heading">
                                      <div class="row">
                                       <div class="col-xs-3">
                                        <i class="fa fa-euro fa-3x"></i>
                                       </div>
                                      <div class="col-xs-9 text-right">
                                          <div class="huge">
                                        <span><asp:Literal ID="PdtValue" runat="server" /></span> 
                                            </div>
                                        <div>euro</div>
                                      </div>
                                   </div>
                                </div>
                            <a href="#">
                                <div class="panel-footer">
                                    <span class="pull-left">Product value</span>
                                    <span class="pull-right"><i class="fa fa-money"></i></span>
                                    <div class="clearfix"></div>
                                </div>
                            </a>
                                </div>
                            </div>
                    </div>
                        <div class="row">
                            <div class="col-lg-9 ">
                                <div class="panel panel-green">
                                  <div class="panel-heading">
                                      <div class="row">
                                       <div class="col-xs-3">
                                        <i class="fa fa-tag fa-3x"></i>
                                       </div>
                                      <div class="col-xs-9 text-right">
                                          <div class="huge">
                                        <span><asp:Literal ID="IcInterval" runat="server" /></span> 
                                            </div>
                                        <div>%</div>
                                      </div>
                                   </div>
                                </div>
                            <a href="#">
                                <div class="panel-footer">
                                    <span class="pull-left">Confidence Interval Width</span>
                                    <span class="pull-right"><i class="fa fa-signal"></i></span>
                                    <div class="clearfix"></div>
                                </div>
                            </a>
                                </div>
                            </div>
                    </div>
                        </div>
                 </div>
                <!-- /.row -->
                <div class="row">
                    <div class="col-lg-12">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h3 class="panel-title"><i class="fa fa-bar-chart-o fa-fw"></i> Portfolio's value evolution</h3>
                            </div>
                            <div class="panel-body">
                                <div id="morris-area-chart"></div>
                            </div>
                        </div>
                    </div>
                </div>

                 <div class="row">
                    <div class="col-lg-6">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h3 class="panel-title"><i class="fa fa-clock-o fa-fw"></i> Cash Panel</h3>
                            </div>
                            <div class="panel-body">
                                <div class="list-group">
                                    <a href="#" class="list-group-item">
                                        <span class="badge">39.34</span>
                                        <i class="fa fa-fw fa-money"></i> Cash Euro
                                    </a>
                                    <a href="#" class="list-group-item">
                                        <span class="badge">10</span>
                                        <i class="fa fa-fw fa-money"></i> Cash GBP
                                    </a>
                                    <a href="#" class="list-group-item">
                                        <span class="badge">10.34</span>
                                        <i class="fa fa-fw fa-money"></i> Cash Dollar
                                    </a>
                                    <a href="#" class="list-group-item">
                                        <span class="badge">10.34</span>
                                        <i class="fa fa-fw fa-money"></i> Cash Yen
                                    </a>
                                    <a href="#" class="list-group-item">
                                        <span class="badge">10.34</span>
                                        <i class="fa fa-fw fa-money"></i> Cash CHF
                                    </a>
                                </div>
                                <div class="text-right">
                                    <a href="#">View All Stocks <i class="fa fa-arrow-circle-right"></i></a>
                                </div>
                            </div>
                        </div>
                    </div>
         </div>

            
                    <!-- Configuration Modal -->
                <div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                  <div class="modal-dialog">
                    <div class="modal-content">
                      <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                        <h4 class="modal-title" id="myModalLabel">Configuration Panel</h4>
                      </div>
                      <div class="modal-body">
                        <div class="form-group">
                         <div class="btn-toolbar">
                                    <!--Default buttons with dropdown menu-->
                                    <div class="btn-group">
                                        <button type="button" class="btn btn-default">Choose pricing model</button>
                                        <button type="button" data-toggle="dropdown" class="btn btn-default dropdown-toggle"><span class="caret"></span></button>
                                        <ul class="dropdown-menu">
                                            <li><a href="#">Black Scholes - Constant Parameters</a></li>
                                            <li><a href="#">Black Scholes - Vasicek Interest Rate Model</a></li>
                                            <li><a href="#">Black Scholes - Hull White Interest Rate Model</a></li>
                                            <li class="divider"></li>
                                            <li><a href="#">Heston Model</a></li>
                                        </ul>
                                    </div>
                              </div>
                         </div>
                            <div class="form-group">
                           <input type="email" class="form-control" id="mcSamples" placeholder="Monte Carlo Number of Samples"/>
                                </div>
                                  <div class="form-group">
                           <input type="email" class="form-control" id="ptfRebal" placeholder="Portfolio Rebalancing Dates Number"/>
                                      </div>
                      </div>
                      <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                        <button type="button" class="btn btn-primary">Save changes</button>
                      </div>
                    </div>
                  </div>
                </div>

            </div>
            <!-- /.container-fluid -->
        </div>
        <!-- /#page-wrapper -->

    </div>
    <!-- /#wrapper -->
                <!-- /.row -->

              

    <!-- jQuery -->
    <script src="js/jquery.js"></script>

    <!-- Bootstrap Core JavaScript -->
    <script src="js/bootstrap.min.js"></script>

    <!-- Morris Charts JavaScript -->
    <script src="js/plugins/morris/raphael.min.js"></script>
    <script src="js/plugins/morris/morris.min.js"></script>
    <script src="js/plugins/morris/morris-data.js"></script>

    <!-- Flot Charts JavaScript -->
    <!--[if lte IE 8]><script src="js/excanvas.min.js"></script><![endif]-->
    <script src="js/plugins/flot/jquery.flot.js"></script>
    <script src="js/plugins/flot/jquery.flot.tooltip.min.js"></script>
    <script src="js/plugins/flot/jquery.flot.resize.js"></script>
    <script src="js/plugins/flot/jquery.flot.pie.js"></script>
    <script src="js/plugins/flot/flot-data.js"></script>
</body>
</html>
