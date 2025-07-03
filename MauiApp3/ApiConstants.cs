using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MauiApp3
{
    internal class ApiConstants
    {
        public static readonly string BaseUrl = "https://ecd4-2001-1c06-1b17-4f00-3db9-ccdb-dd35-c0c8.ngrok-free.app";
        public static readonly string WeatherEndpoint = $"{BaseUrl}/api/Weather";
    }
}
