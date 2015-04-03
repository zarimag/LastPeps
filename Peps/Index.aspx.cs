using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using Wrapper;

namespace Peps
{
    public partial class Index : System.Web.UI.Page
    {

        public Portfolio CurrentPortfolio
        {
            get
            {
                object temp = Session["Pf"];
                return temp == null ? null : (Portfolio)temp;
            }
            set { Session["Pf"] = value; }
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!Page.IsPostBack)
            {
                this.PnLDiv.Text = "0.0983";
                this.PdtValue.Text = Properties.Settings.Default.Nominal.ToString();
                this.PtfValue.Text = "99.31";

                if (CurrentPortfolio == null)
                {
                    CurrentPortfolio = new Portfolio(new WrapperClass(), new MarketData());
                }

                CurrentPortfolio.compute();

                PrintResultsOnGUI();
                
          

                using (StreamWriter sw = new StreamWriter(@"D:\PEPS\bis\Peps-MultiView\deltaBis.txt"))
                {

                    for (int i = 0; i < CurrentPortfolio.wrapper.getDelta().Length; i++)
                    {
                        sw.WriteLine(CurrentPortfolio.wrapper.getDelta()[i]);
                    }
                }

                //FillHedgingParameters();
                //HedgingFillFxRates();
                //for (int j = 0; j < hedgingPreviousStocksPrices.GetLength(0); j++)
                //{
                //    hedgingPreviousStocksPrices[j, RBSindex] = hedgingPreviousStocksPrices[j, RBSindex] * hedgingPreviousStocksPrices[j,
                //        Properties.Settings.Default.AssetNb + 1] / 100.0;
                //    hedgingPreviousStocksPrices[j, citiGroupIndex] = hedgingPreviousStocksPrices[j, citiGroupIndex] * hedgingPreviousStocksPrices[j,
                //                            Properties.Settings.Default.AssetNb + 3];
                //}
                //using (StreamWriter sw = new StreamWriter(@"D:\PEPS\bis\Peps-MultiView\stockPrices.txt"))
                //{

                //    for (int i = 0; i < hedgingPreviousStocksPrices.GetLength(0); i++)
                //    {
                //        for (int j = 0; j < hedgingPreviousStocksPrices.GetLength(1) - 1; j++)
                //        {
                //            sw.Write(hedgingPreviousStocksPrices[i, j] + "\t");
                //        }
                //        sw.Write(hedgingPreviousStocksPrices[i, hedgingPreviousStocksPrices.GetLength(1) - 1]+"\n");
                //    }
                //}

                //using (StreamWriter sw = new StreamWriter(@"D:\PEPS\bis\Peps-MultiView\initStocks.txt"))
                //{

                //    for (int i = 0; i < previousStocksPrices.GetLength(0); i++)
                //    {
                //        for (int j = 0; j < previousStocksPrices.GetLength(1) - 1; j++)
                //        {
                //            sw.Write(previousStocksPrices[i, j] + "\t");
                //        }
                //        sw.Write(previousStocksPrices[i, hedgingPreviousStocksPrices.GetLength(1) - 1] + "\n");
                //    }
                //}

                //using (StreamWriter sw = new StreamWriter(@"D:\PEPS\bis\Peps-MultiView\interestRates.txt"))
                //{

                //    for (int i = 0; i < previousInterestRates.Length; i++)
                //    {
                //        sw.WriteLine(previousInterestRates[i]);
                //    }
                //}

                //using (StreamWriter sw = new StreamWriter(@"D:\PEPS\bis\Peps-MultiView\stockToFx.txt"))
                //{

                //    for (int i = 0; i < stockToFxIndex.Length; i++)
                //    {
                //        sw.WriteLine(stockToFxIndex[i]);
                //    }
                //}


                    //int k = 0;
                    //hedgeDPreviousStockPrices = new double[hedgingPreviousStocksPrices.GetLength(0) * hedgingPreviousStocksPrices.GetLength(1)];

                    //for (int i = 0; i < hedgingPreviousStocksPrices.GetLength(0); i++)
                    //{
                    //    for (int j = 0; j < hedgingPreviousStocksPrices.GetLength(1); j++)
                    //    {
                    //        hedgeDPreviousStockPrices[k++] = hedgingPreviousStocksPrices[i, j];
                    //    }
                    //}
                    //double H = 300;
                    //CurrentPortfolio.wrapper.computeHedge(hedgeDPreviousStockPrices, previousInterestRates, stockToFxIndex,
                    //    Properties.Settings.Default.AssetNb, Properties.Settings.Default.FxNb, Properties.Settings.Default.Maturity,
                    //    Properties.Settings.Default.McSamplesNb, Properties.Settings.Default.TimeSteps, hedgingPreviousStocksPrices.GetLength(0),
                    //    Properties.Settings.Default.StepFiniteDifference, H);
                    // File.WriteAllLines(
                    //    @"c:\data\myfile.txt" // <<== Put the file name here
                    //, CurrentPortfolio.wrapper.ptfValues.Select(d => d.ToString()).ToArray()
                    //);

                    // File.WriteAllLines(
                    //        @"c:\data\myficashle.txt" // <<== Put the file name here
                    //    , CurrentPortfolio.wrapper.cashValues.Select(d => d.ToString()).ToArray()
                    //    );
            }
        }

        private void PrintResultsOnGUI()
        {
            this.PtfValue.Text = Math.Round(CurrentPortfolio.wrapper.getPrice(), Properties.Settings.Default.Precision).ToString();
            //     this.PtfValue.Text = CurrentPortfolio.wrapper.getPrice().ToString();
            this.IcInterval.Text = Math.Round(CurrentPortfolio.wrapper.getIC(), Properties.Settings.Default.Precision).ToString();
            this.PnLDiv.Text = (100 - Math.Round(CurrentPortfolio.wrapper.getPrice(), Properties.Settings.Default.Precision)).ToString();
            FillAssetsTable("27", "02", "2015");
            FillCurrenciesTable("2005-11-29", "2005-11-29");
        }

        private void FillCurrenciesTable(string fxStartDate, string fxEndDate) {
            double[] deltaVect = CurrentPortfolio.wrapper.getDelta();
            int cpt = Properties.Settings.Default.AssetNb;
            string tmp;
            foreach (PropertyInfo property in
                typeof(Properties.Resources).GetProperties(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic))
            {
                if(property.Name.Substring(0,2).Equals("Fx")){
                    TableRow tr = new TableRow();
                    tr.BackColor = Color.White;
                    TableCell isin = new TableCell();
                    isin.Text = property.Name.Substring(2);
                    tr.Cells.Add(isin);

                    TableCell name = new TableCell();
                    name.Text = Properties.Resources.ResourceManager.GetString(property.Name);
                    tr.Cells.Add(name);

                    TableCell price = new TableCell();
                    tmp = CurrentPortfolio.marketData.getLastCurrencyPrice(property.Name.Substring(2), fxStartDate, fxEndDate);
                    price.Text = tmp + "€";
                    // price.Text = "73.73 €";
                    tr.Cells.Add(price);


                    TableCell delta = new TableCell();
                    delta.Text = Math.Round(deltaVect[cpt], Properties.Settings.Default.Precision).ToString();
                    tr.Cells.Add(delta);

                    TableCell totalValue = new TableCell();
                    totalValue.Text = (Math.Round(Double.Parse(tmp, CultureInfo.InvariantCulture) * deltaVect[cpt], Properties.Settings.Default.Precision)).ToString();
                    tr.Cells.Add(totalValue);

                    stocksTable.Rows.Add(tr);
                    cpt++;
                }
            }
        }
        //month de 00 à 11
        //day et year: normaux
        private void FillAssetsTable(string day, string month, string year )
        {
            double[] deltaVect = CurrentPortfolio.wrapper.getDelta();
            string tmpStockTicker;
            int cpt = 0;
            string tmp;
            foreach (PropertyInfo property in
                typeof(Properties.Resources).GetProperties(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic))
            {
                if (property.Name.Length == 12)
                {
                    //i.e it is an ISIN, TODO: add more robust verification
                    TableRow tr = new TableRow();

                    TableCell isin = new TableCell();
                    isin.Text = property.Name;
                    tr.Cells.Add(isin);

                    TableCell name = new TableCell();
                    name.Text = Properties.Resources.ResourceManager.GetString(property.Name).Split(';')[0];
                    tr.Cells.Add(name);

                    tmpStockTicker = Properties.Resources.ResourceManager.GetString(property.Name).Split(';')[1];
                    TableCell price = new TableCell();
                    tmp = CurrentPortfolio.marketData.getStockPrice(tmpStockTicker, day, month, year);
                    price.Text = tmp + "€";
                   // price.Text = "73.73 €";
                    tr.Cells.Add(price);


                    TableCell delta = new TableCell();

                    delta.Text = Math.Round(deltaVect[cpt], Properties.Settings.Default.Precision).ToString();
                    tr.Cells.Add(delta);

                    TableCell totalValue = new TableCell();
                    totalValue.Text = (Math.Round(Double.Parse(tmp, CultureInfo.InvariantCulture) * deltaVect[cpt], Properties.Settings.Default.Precision)).ToString();
                    tr.Cells.Add(totalValue);

                    stocksTable.Rows.Add(tr);

                    cpt++;
                }
            }
        }

        /*
        public void Get_Data(Object sender, EventArgs e)
        {
            CurrentPortfolio.getData();
        }

        public void updateDate(Object sender, EventArgs e)
        {
            String str = Request.Form[DisplayCalendar.UniqueID];
            String[] date = str.Split('/');
            if (date.Length == 3)
            {
                String y = date[2];
                String m = date[0];
                String d = date[1];
                CurrentPortfolio.setDate(Convert.ToInt32(y), Convert.ToInt32(m), Convert.ToInt32(d));
            }
        }

        //Cette fonction calcule le prix à la date spécifié par l'utilisateur
        //Après ça on calcule le portefeuille en rebalançant tous les jours 
        protected void Compute_Price(Object sender, EventArgs e)
        {
            initDisplay();
            CurrentPortfolio.loadFromComputations();
            displayData();
        }

        public void Compute_Simu1(Object sender, EventArgs e)
        {
            initDisplay();
            CurrentPortfolio.LoadFromResource(1);
            displayData();
        }
        
        public void Compute_Simu2(Object sender, EventArgs e)
        {
            initDisplay();
            CurrentPortfolio.LoadFromResource(2);
            displayData();
        }


        public void Compute_Simu3(Object sender, EventArgs e)
        {
            initDisplay();
            CurrentPortfolio.LoadFromResource(3);
            displayData();
        }

        public void initDisplay()
        {
            //Color and scale of the chart
            Chart1.Series.FindByName("ProductPrice").Color = Color.DarkBlue;
            Chart1.Series.FindByName("PortfolioPrice").Color = Color.DarkRed;
            Chart1.ChartAreas[0].AxisY.Maximum = 104;
            Chart1.ChartAreas[0].AxisY.Minimum = 88;
        }


        public void displayData()
        {
            deltaTable.Rows.Clear();
            assetTable.Rows.Clear();

            prixLabel.Text = CurrentPortfolio.getCurrentPrice().ToString();
            for (int i = 0; i < CurrentPortfolio.numberOfStock; i++)
            {
                TableRow deltaRow = new TableRow();
                TableCell quantityToBuy = new TableCell();
                quantityToBuy.Text = CurrentPortfolio.getCurrentDelta()[i].ToString();
                deltaRow.Cells.Add(quantityToBuy);
                TableCell marketValue = new TableCell();
                marketValue.Text = CurrentPortfolio.getCurrentMarketValues()[i].ToString();
                deltaRow.Cells.Add(marketValue);
                deltaTable.Rows.Add(deltaRow);
                TableRow portfolioRow = new TableRow();
                TableCell quantityAlreadyBought = new TableCell();
                quantityAlreadyBought.Text = CurrentPortfolio.getCurrentQuantities()[i].ToString();
                portfolioRow.Cells.Add(quantityAlreadyBought);
                TableCell secondColumnPortfolio = new TableCell();
                //Add the name of the asset
                secondColumnPortfolio.Text = CurrentPortfolio.symbols[i];
                portfolioRow.Cells.Add(secondColumnPortfolio);

                assetTable.Rows.Add(portfolioRow);
            }

            cashLabel.Text = CurrentPortfolio.getCash().ToString();
            vpLabel.Text = CurrentPortfolio.getCurrentPortfolioValue().ToString();
            teLabel.Text = CurrentPortfolio.trackingError.ToString();
            plLabel.Text = CurrentPortfolio.getCurrentProfitAndLoss().ToString();


            //Plot product price and portfolio value
            //number of Step
            int nbStep = CurrentPortfolio.prix.Length;
            initDisplay();
            for (int j = 0; j < CurrentPortfolio.index; j++)
            {
                Chart1.Series.FindByName("ProductPrice").Points.Add(CurrentPortfolio.prix[j]);
                Chart1.Series.FindByName("PortfolioPrice").Points.Add(CurrentPortfolio.pfvalue[j]);
            }
        }

        public void Continue_Simu(Object sender, EventArgs e)
        {
            if (CurrentPortfolio.prix == null) Compute_Simu3(sender, e);
            CurrentPortfolio.ComputeSimulation(10);
            displayData();
        }


        public void LoadPage1(Object sender, EventArgs e){
            MultiView1.SetActiveView(View1);
        }
        
        public void LoadPage2(Object sender, EventArgs e){
            MultiView1.SetActiveView(View2);
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
           
        }
*/

    }
}