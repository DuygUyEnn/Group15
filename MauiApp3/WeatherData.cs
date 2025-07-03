using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MauiApp3
{
    public class WeatherData
    {
        public DateTime Time { get; set; }

        public DateTime Date { get; set; }

        public string DeviceId { get; set; }

        public long timestamp { get; set; }
        public float humidity { get; set; }
        public float light { get; set; }
        public float pressure { get; set; }
        public float temperature { get; set; }


        // Dùng để lọc chung bất kể mode
        public DateTime ActualTime => Time != default ? Time : Date;
    }
}
