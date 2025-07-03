using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MauiApp3
{
    public class WeatherResponse
    {
        public string mode { get; set; }
        public string cursorDate { get; set; }
        public List<WeatherData> data { get; set; }
    }
}
