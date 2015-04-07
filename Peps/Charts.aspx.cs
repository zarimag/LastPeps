using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace Peps
{
    public partial class Charts : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected string[] parseFile()
        {
            string[] lines = System.IO.File.ReadAllLines(@"D:\GitHub\LastPeps\TextFile.txt");
            foreach (string line in lines)
            {
                // Use a tab to indent each line of the file.
                Console.WriteLine("\t" + line);
            }
            return lines;
        }
    }
}