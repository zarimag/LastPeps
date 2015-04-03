using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Peps
{
   public class Price
    {
        public string Symbol { get; set; }
        public string Name { get; set; }
        public double Open { get; set; }
        public double High { get; set; }
        public double Low { get; set; }
        public double Close { get; set; }

        public double d { get; set; }
        public double m { get; set; }
        public double y { get; set; }

    }
}