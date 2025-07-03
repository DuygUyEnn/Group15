using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace MauiApp3
{

    public class HomePageViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public string TodayString => DateTime.Now.ToString("dddd");
        public string DateString => DateTime.Now.ToString("dd/MM/yyyy");
        public ObservableCollection<WeatherData> Sensors { get; set; } = new();
        public ObservableCollection<TemperatureInfo> TemperatureSummary { get; set; } = new();

        private WeatherData _currentSensor;
        public WeatherData CurrentSensor
        {
            get => _currentSensor;
            set
            {
                if (_currentSensor != value)
                {
                    _currentSensor = value;
                    OnPropertyChanged(nameof(CurrentSensor));
                    OnPropertyChanged(nameof(WeatherIcon)); // <- thêm dòng này
                    OnPropertyChanged(nameof(WeatherDescription)); // <- thêm dòng này
                }
            }
        }
        public string WeatherDescription
        {
            get
            {
                if (CurrentSensor == null)
                    return "Sunny";

                if (CurrentSensor.humidity > 95)
                    return "Raining";

                if (DateTime.Now.Hour >= 19 || DateTime.Now.Hour < 5)
                    return "Night";

                return "Sunny";
            }
        }
        public string WeatherIcon
        {
            get
            {
                if (CurrentSensor == null)
                    return "sun.png";

                if (CurrentSensor.humidity > 95)
                    return "rain1.png";

                if (DateTime.Now.Hour >= 19 || DateTime.Now.Hour < 5)
                    return "moon.png";

                return "sun.png";
            }
        }

        private int _currentIndex = 0;

        private Timer _timer;

        public HomePageViewModel()
        {
            LoadData();
            LoadTemperatureSummary();
            // Cập nhật mỗi 5 phút (300 giây)
            _timer = new Timer(_ => LoadData(), null, TimeSpan.FromSeconds(300), TimeSpan.FromSeconds(300));
        }

        public async void LoadData()
        {
            try
            {
                using var httpClient = new HttpClient();
                var response = await httpClient.GetAsync($"{ApiConstants.WeatherEndpoint}?mode=Day&page=1&pageSize=1000");
                if (response.IsSuccessStatusCode)
                {
                    var weatherResponse = await response.Content.ReadFromJsonAsync<WeatherResponse>();
                    if (weatherResponse?.data != null)
                    {
                        var latestData = weatherResponse.data
                            .GroupBy(d => d.DeviceId)
                            .Select(g => g.OrderByDescending(x => x.ActualTime).First())
                            .ToList();

                        MainThread.BeginInvokeOnMainThread(() =>
                        {
                            Sensors.Clear();
                            foreach (var data in latestData)
                                Sensors.Add(data);

                            // Reset index nếu cần
                            _currentIndex = 0;
                            if (Sensors.Count > 0)
                                CurrentSensor = Sensors[_currentIndex];
                        });
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Lỗi khi tải dữ liệu: {ex.Message}");
            }
        }


        private async Task<int> GetLastPageAsync()
        {
            using var httpClient = new HttpClient();
            int page = 1;
            const int pageSize = 1000;
            bool hasData = true;

            while (hasData)
            {
                var response = await httpClient.GetFromJsonAsync<WeatherResponse>(
                    $"{ApiConstants.WeatherEndpoint}?mode=Month&page={page}&pageSize={pageSize}");

                if (response?.data == null || response.data.Count == 0)
                {
                    hasData = false;
                }
                else
                {
                    page++;
                }
            }

            return page - 1;
        }

        private async void LoadTemperatureSummary()
        {
            try
            {
                using var httpClient = new HttpClient();

                int lastPage = await GetLastPageAsync();
                const int pageSize = 1000;

                var collectedDays = new Dictionary<DateTime, List<WeatherData>>();
                bool moreData = true;
                int page = lastPage;

                while (collectedDays.Count < 4 && moreData && page > 0)
                {
                    var response = await httpClient.GetFromJsonAsync<WeatherResponse>(
                        $"{ApiConstants.WeatherEndpoint}?mode=Month&page={page}&pageSize={pageSize}");

                    if (response?.data == null || response.data.Count == 0)
                    {
                        moreData = false;
                        break;
                    }

                    foreach (var item in response.data.Where(d => d.temperature != -1 && d.ActualTime.Date < DateTime.Today))
                    {
                        var date = item.ActualTime.Date;
                        if (!collectedDays.ContainsKey(date))
                            collectedDays[date] = new List<WeatherData>();

                        collectedDays[date].Add(item);
                    }

                    page--;
                }

                var latest4Days = collectedDays
                    .OrderByDescending(kv => kv.Key)
                    .Take(4)
                    .OrderBy(kv => kv.Key);

                MainThread.BeginInvokeOnMainThread(() =>
                {
                    TemperatureSummary.Clear();
                    foreach (var item in latest4Days.Select(kv => new TemperatureInfo
                    {
                        Date = kv.Key.ToString("dd/MM"),
                        Temperature = $"{Math.Round(kv.Value.Average(d => d.temperature))}°C"
                    }))
                        TemperatureSummary.Add(item);
                });
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi lấy dữ liệu nhiệt độ: " + ex.Message);
            }
        }




        public void NextSensor()
        {
            if (Sensors.Count == 0) return;

            _currentIndex = (_currentIndex + 1) % Sensors.Count;
            CurrentSensor = Sensors[_currentIndex];
        }

        public void PreviousSensor()
        {
            if (Sensors.Count == 0) return;

            _currentIndex = (_currentIndex - 1 + Sensors.Count) % Sensors.Count;
            CurrentSensor = Sensors[_currentIndex];
        }

        private void OnPropertyChanged(string propertyName) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}
