// File: WeatherApi/Dtos/WeatherResponseDto.cs
using WeatherApi.Models;

namespace WeatherApi.Dtos
{
    public class WeatherResponseDto
    {
        public string mode { get; set; }
        public string cursorDate { get; set; }
        public List<WeatherData> data { get; set; }
        public string prevCursorDate { get; set; }
        public string nextCursorDate { get; set; }
    }
}
