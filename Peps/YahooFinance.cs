using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Peps
{
    public class YahooFinance
    {
        public static List<Price> Parse(string csvData)
        {
            List<Price> prices = new List<Price>();

            string[] rows = csvData.Replace("\r", "").Split('\n');

            foreach (string row in rows)
            {
                if (string.IsNullOrEmpty(row)) continue;

                string[] cols = row.Split(',');
                Price p = new Price();
                Console.WriteLine(row);
                Console.WriteLine(cols[0]);
                p.Symbol = cols[0];
                p.Name = cols[0];
                Console.WriteLine(cols[1]);
                p.Open = double.Parse(cols[1], System.Globalization.CultureInfo.InvariantCulture);
                Console.WriteLine(cols[2]);
                p.High = double.Parse(cols[2], System.Globalization.CultureInfo.InvariantCulture);
                Console.WriteLine(cols[3]);
                p.Low = double.Parse(cols[3], System.Globalization.CultureInfo.InvariantCulture);
                Console.WriteLine(cols[4]);
                p.Close = double.Parse(cols[4], System.Globalization.CultureInfo.InvariantCulture);

                prices.Add(p);
            }

            return prices;
        }

        public static void replaceHisto(Hashtable table, string symbol, List<Price> prices)
        {
            Console.WriteLine("TO DO : ");
            Console.WriteLine("Remplacer historique dans la table quand on choisit un actif deja existant : ");
        }


        //Fonction qui ecrit dans la table l'historique de l'actif symbol
        
        public static void HistoricalParse(string csvData, Hashtable table, string symbol, bool isCurrency)
        {

            List<Price> temp = new List<Price>();

            string[] rows = csvData.Replace("\r", "").Split('\n');
            //on remplit la table si elle ne contient pas deja l'actif desire

            int count = 0;
            foreach (string row in rows)
            {
                if (string.IsNullOrEmpty(row)) continue;
                Console.WriteLine("Ligne : " + count);
                if (count != 0)       //on ne tient pas compte de la premiere ligne du csv 
                {
                    string[] cols = row.Split(',');
                    Price p = new Price();
                    p.Symbol = symbol;
                    p.Name = symbol;

                    string[] dates = cols[0].Split('-');
                    p.y = double.Parse(dates[0], System.Globalization.CultureInfo.InvariantCulture);
                    p.m = double.Parse(dates[1], System.Globalization.CultureInfo.InvariantCulture);
                    p.d = double.Parse(dates[2], System.Globalization.CultureInfo.InvariantCulture);

                    if(!isCurrency){
                        p.High = double.Parse(cols[2], System.Globalization.CultureInfo.InvariantCulture);

                        p.Low = double.Parse(cols[3], System.Globalization.CultureInfo.InvariantCulture);   
                    }else{
                        p.High = double.Parse(cols[1], System.Globalization.CultureInfo.InvariantCulture);
                        p.Low = double.Parse(cols[1], System.Globalization.CultureInfo.InvariantCulture);
                    }

                    temp.Add(p);
                }
                count++;
            }
            if (!table.Contains(symbol))    //si la table ne contient d'historique pour le symbol on l'ajoute à la table
            {
                table.Add(symbol, temp);
            }

            else                            //sinon on remplace son historique
            {
                replaceHisto(table, symbol, temp);
            }
        }

    }
}