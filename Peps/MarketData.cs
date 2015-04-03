using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Web;

namespace Peps
{
    public class MarketData
    {
        public Hashtable table;
        public Hashtable currSymbol;
        public List<String> Symbols;
        public List<String> CurrSymbols;
        public int sizeList;

        public static string urlPrototype = @"http://ichart.finance.yahoo.com/table.csv?s={0}&a={1}&b={2}&c={3}&d={4}&e={5}&f={6}&g={7}&ignore=.csv";
        public static string currencyURL = @"https://www.quandl.com/api/v1/datasets/CURRFX/{0}.csv?&trim_start={1}&trim_end={2}";

        public Dictionary<String, List<String>> currencyToPriceList;


        public MarketData(){
            table = new Hashtable();
            currSymbol = new Hashtable(); 
            Symbols= new List<String>();
            CurrSymbols = new List<String>();

            currencyToPriceList = new Dictionary<string, List<string>>();
            sizeList = 2371;
        }

        public void storeData(){
            String[][] temp= new String[table.Keys.Count+1][];
            Symbols.AddRange(CurrSymbols);
            
            int i = 1;
            foreach (String sym in Symbols)
            {
                if(i==7){
                    temp[0]= new String[((List <Price>) table[sym]).Count+1];
                }
                
                temp[i]= new String[((List <Price>) table[sym]).Count+1];
                temp[i][0] = (String) sym;
                int j = 1;
                foreach (Price price in ((List<Price>)table[sym]))
                {
                    if(i==7){
                         temp[0][j]= price.d.ToString()+"-"+price.m.ToString()+"-"+price.y.ToString();
                    }
                    temp[i][j] = ((price.High + price.Low) / 2).ToString();
                    j++;
                }
                i++;
            }
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"C:\Users\el azari\Desktop\market.txt"))
            
                for (int z = 0; z < 2371; z++ )
                {
                    String tempStr = "";
                    for (int k = 0; k < temp.Length; k++)
                    {
                        if(z<temp[k].Length){
                            tempStr += temp[k][z] + " ";
                        }
                    }
                     file.WriteLine(tempStr);
                }
        }
        //
        public void fixeDataSize()
        {
            foreach (String sym in Symbols)
            {
                int diff= ((List <Price>) table[sym]).Count -sizeList;
                while(diff<0){
                    List<Price> temp ;
                    if(-diff >= ((List<Price>)table[sym]).Count){
                        temp= ((List<Price>)table[sym]).ToList();
                    }else{
                        temp= ((List<Price>)table[sym]).GetRange(((List<Price>)table[sym]).Count + diff, -diff);
                    }
                    temp.Reverse();
                    ((List<Price>)table[sym]).AddRange(temp);
                    diff = ((List<Price>)table[sym]).Count - sizeList;
                    
                }
            }
        }


        public ArrayList getPreviousCurrencyPrices(string symbol, string startDate, string endDate)
        {
            ArrayList prices = new ArrayList();
            string url = string.Format(currencyURL, symbol, startDate, endDate);
            var webClient = new WebClient();
            string csvData = webClient.DownloadString(url);
            string[] rows = csvData.Replace("\r", "").Split('\n');
            for (int i = rows.Length - 2; i >= 1; i--)
            {
                prices.Add(Double.Parse(rows[i].Split(',')[1], CultureInfo.InvariantCulture));
            }
            return prices;
        }

        public string getLastCurrencyPrice(string symbol, string startDate, string endDate)
        {
//            String url = "https://www.quandl.com/api/v1/datasets/CURRFX/" + symbol + ".csv";
            string url = string.Format(currencyURL, symbol, startDate, endDate);
            var webClient = new WebClient();
            string csvData = webClient.DownloadString(url);
            string[] rows = csvData.Replace("\r", "").Split('\n');
            return rows[1].Split(',')[1];
        }


        public ArrayList getLastStockPrices(string symbol, string startDay, string startMonth,
                        string startYear, string endDay, string endMonth, string endYear)
        {
            ArrayList prices = new ArrayList();
            try
            {
                string url = string.Format(urlPrototype, symbol, startMonth, startDay, 
                    startYear, endMonth, endDay, endYear, "d");
                var webClient = new WebClient();
                String csvData = webClient.DownloadString(url);
                string[] rows = csvData.Replace("\r", "").Split('\n');
                for (int i = rows.Length - 2; i >= 1; i--)
                {
                    prices.Add(Double.Parse(rows[i].Split(',')[6], CultureInfo.InvariantCulture));
                }
                return prices;
            }
            catch
            {
                Console.WriteLine("No files for the stock " + symbol + " for the specified date range founded on the server.");
            }
            return null;
        }

        public string getStockPrice(string symbol, string day, string month, string year)
        {
            try
            {
                string url = string.Format(urlPrototype, symbol, month, day, year, month, day + 1, year, "d");
                var webClient = new WebClient();
                String csvData = webClient.DownloadString(url);
                string[] rows = csvData.Replace("\r", "").Split('\n');
                return rows[1].Split(',')[6];
            }
            catch
            {
                Console.WriteLine("No files for the stock " + symbol + " for the specified date range founded on the server.");
            }
            return null;
            /*

                String csvData =  webClient.DownloadString(url);
                YahooFinance.HistoricalParse(csvData, table, symbol, false);
            }
            catch
            {
                Console.WriteLine("No files for the stock " + symbol + " for the specified date range founded on the server.");
            }*/

            //Retrieve stock prices
         /*   foreach (String sym in Symbols)
            {
                String csvData;
                using (WebClient web = new WebClient())
                {
                    String url = "http://ichart.finance.yahoo.com/table.csv?s=" + sym + "&d=" + d + "&e=" + e + "&f=" + f + "&g=d&a=" + a + "&b=" + b + "&c=" + c + "&ignore=.csv";

                    csvData = web.DownloadString(url);
                }
                YahooFinance.HistoricalParse(csvData, table, sym, false);
            }*/
        }

        /*
         * Use it to retrieve stock price on yahoofinance
         * a et d: start and end month minus 1
         * b et e: start and end day 
         * c et f: start and end year
         */
        public void retrieveDataFromWeb(String a, String b, String c, String d, String e, String f){
            //Jusque norvatis
            //Euro
            Symbols.Add("IBDRY");
            //Euro
            Symbols.Add("ENLAY");
            //Livre
            currSymbol.Add("HSEA","GBPEUR");
            Symbols.Add("HSEA");
            //Livre
            currSymbol.Add("DEO","GBPEUR");
            Symbols.Add("DEO");
            //Livre
            currSymbol.Add("TSCDF","GBPEUR");
            Symbols.Add("TSCDF");
            //Dollars
            currSymbol.Add("WFC","USDEUR");
            Symbols.Add("WFC");
            //Dollars
            currSymbol.Add("PEP", "USDEUR");
            Symbols.Add("PEP");
            //CHF
            currSymbol.Add("NVS", "CHFEUR");
            Symbols.Add("NVS");      
            Symbols.Add("DTE.DE"); 
            Symbols.Add("GPDNF");
            currSymbol.Add("BP","GBPEUR");
            Symbols.Add("BP");
            currSymbol.Add("RBS","GBPEUR");
            Symbols.Add("RBS");
            currSymbol.Add("PG", "USDEUR");
            Symbols.Add("PG");
            currSymbol.Add("JNJ", "USDEUR");
            Symbols.Add("JNJ");
            currSymbol.Add("GE", "USDEUR");
            Symbols.Add("GE");
            currSymbol.Add("C", "USDEUR");
            Symbols.Add("C");
            currSymbol.Add("VZ", "USDEUR");
            Symbols.Add("VZ");
            currSymbol.Add("NTT", "EURJPY");
            Symbols.Add("NTT");   
            Symbols.Add("EOAN.DE");          
            Symbols.Add("OR.PA");          
           

            //Add the currency
          
            CurrSymbols.Add("GBPEUR");           
            CurrSymbols.Add("USDEUR");
            CurrSymbols.Add("CHFEUR");
            CurrSymbols.Add("EURJPY");

            //Retrieve stock prices
            foreach(String sym in Symbols){
                String csvData;
                using (WebClient web = new WebClient())
                {
                    //30 novembre 2005
                    //30 fevrier 2015.
                    //TO DO 
                    //automatiser la recherche url en fction de la date et de l'action
                    String url = "http://ichart.finance.yahoo.com/table.csv?s=" + sym + "&d=" + d + "&e=" + e + "&f=" + f + "&g=d&a=" + a + "&b=" + b + "&c=" + c + "&ignore=.csv";
                    
                    csvData = web.DownloadString(url);
                }
                YahooFinance.HistoricalParse(csvData, table, sym,false);
            }
            //Retrieve
            foreach (String sym in CurrSymbols)
            {
                String csvData;
                using (WebClient web = new WebClient())
                {
                    String url = "https://www.quandl.com/api/v1/datasets/CURRFX/" + sym + ".csv";

                    csvData = web.DownloadString(url);
                }
                YahooFinance.HistoricalParse(csvData, table, sym,true);
            }
        }
    }
}