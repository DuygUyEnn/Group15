using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Json;
using System.Threading.Tasks;
using LiveChartsCore;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;

namespace MauiApp3
{
    public class ViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private readonly Dictionary<string, SKColor> colorMap = new()
    {
        { "mkr-grp15-2024", SKColors.Red },
        { "mkr-saxion", SKColors.Blue },
        { "lht-tester", SKColors.Green },
        { "lht-wierden", SKColors.Orange },
        { "lht-gronau", SKColors.Purple },
        { "lht-saxion", SKColors.Black },
    };

        private ISeries[] _series;
        public ISeries[] Series
        {
            get => _series;
            set
            {
                _series = value;
                OnPropertyChanged(nameof(Series));
            }
        }

        private Axis[] _xAxes;
        public Axis[] XAxes
        {
            get => _xAxes;
            set
            {
                _xAxes = value;
                OnPropertyChanged(nameof(XAxes));
            }
        }

        public Axis[] YAxes { get; set; } = new Axis[]
        {
            new Axis
            {
                Name = "Value",
                LabelsPaint = new SolidColorPaint(SKColors.Gray),
                TextSize = 14
            }
        };

        private string apiUrl = ApiConstants.WeatherEndpoint;

        private string selectedParameter = "Temperature";
        public string SelectedPeriod { get; set; } = "Day";
        public string CurrentPeriod { get; private set; } = "Day";

        public ViewModel()
        {
            UpdateSeries("Month");
        }

        public void UpdateSeries(string period)
        {
            SelectedPeriod = period;
            CurrentPeriod = period;
            GetWeatherData(period);
        }

        public async void GetWeatherData(string period)
        {
            try
            {
                using var httpClient = new HttpClient();
                int page = 1;
                int pageSize = 1000;

                var allData = new List<WeatherData>();
                bool hasMoreData = true;

                while (hasMoreData)
                {
                    var response = await httpClient.GetAsync($"{apiUrl}?mode={period}&page={page}&pageSize={pageSize}");

                    if (response.IsSuccessStatusCode)
                    {
                        var weatherResponse = await response.Content.ReadFromJsonAsync<WeatherResponse>();
                        if (weatherResponse?.data == null || weatherResponse.data.Count == 0)
                        {
                            hasMoreData = false;
                        }
                        else
                        {
                            allData.AddRange(weatherResponse.data);
                            page++;
                        }
                    }
                    else
                    {
                        Console.WriteLine("Không thể kết nối API.");
                        hasMoreData = false;
                    }
                }

                Console.WriteLine($"Tổng dữ liệu nhận được: {allData.Count} dòng");

                if (allData.Count > 0)
                {
                    List<WeatherData> filteredData = FilterDataByPeriod(allData, period);
                    Console.WriteLine($"Dữ liệu sau khi lọc: {filteredData.Count} dòng");
                    UpdateChartData(filteredData);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Lỗi khi gọi API: {ex.Message}");
            }
        }


        private List<WeatherData> FilterDataByPeriod(List<WeatherData> data, string period)
        {
            List<WeatherData> filteredData = new List<WeatherData>();

            // Lấy ngày mới nhất từ dữ liệu
            var maxDate = data.Max(d => d.ActualTime).Date;
            Console.WriteLine($"Ngày mới nhất trong dữ liệu: {maxDate}");

            switch (period)
            {
                case "Day":
                    filteredData = data.Where(d => d.ActualTime.Date == maxDate).ToList();
                    break;

                case "Week":
                    var startOfWeek = maxDate.AddDays(-6);
                    filteredData = data.Where(d => d.ActualTime.Date >= startOfWeek && d.ActualTime.Date <= maxDate).ToList();
                    break;

                case "Month":
                    var startOfMonth = maxDate.AddDays(-29); // 30 ngày gần nhất
                    filteredData = data.Where(d => d.ActualTime.Date >= startOfMonth && d.ActualTime.Date <= maxDate).ToList();
                    break;
            }

            Console.WriteLine($"Dữ liệu lọc được: {filteredData.Count} dòng");
            return filteredData;
        }

        private DateTime GetTimeSlot(DateTime time, string period)
        {
            return period switch
            {
                "Day" => new DateTime(time.Year, time.Month, time.Day, time.Hour, time.Minute / 15 * 15, 0), // mỗi 15 phút
                "Week" => new DateTime(time.Year, time.Month, time.Day), // mỗi ngày
                "Month" => new DateTime(time.Year, time.Month, time.Day), // mỗi ngày
                _ => time
            };
        }
        private void UpdateChartData(List<WeatherData> data)
        {
            var groupedData = data.GroupBy(d => d.DeviceId);
            var seriesList = new List<ISeries>();
            List<string> timeLabels = new List<string>();

            List<string> allLabels = new List<string>();
            List<DateTime> timePoints = new List<DateTime>();

            foreach (var group in groupedData)
            {
                var orderedGroup = group.OrderBy(d => d.Time).ToList();

                var groupedByTimeSlot = orderedGroup
                    .GroupBy(d => GetTimeSlot(d.Time, CurrentPeriod))
                    .Select(g => g.First())
                    .OrderBy(d => d.Time)
                    .ToList();

                // Downsample sau group
                int desiredMaxPoints = 300;
                var downsampledGroup = groupedByTimeSlot
                    .TakeLast(desiredMaxPoints)
                    .ToList();

                var selectedValues = selectedParameter switch
                {
                    "Temperature" => downsampledGroup.Select(d => (double)d.temperature).ToArray(),
                    "Humidity" => downsampledGroup.Select(d => (double)d.humidity).ToArray(),
                    "Pressure" => downsampledGroup.Select(d => (double)d.pressure).ToArray(),
                    "Light" => downsampledGroup.Select(d => (double)d.light).ToArray(),
                    _ => new double[0]
                };

                if (allLabels.Count == 0) // chỉ cần tạo label 1 lần
                {
                    allLabels = downsampledGroup.Select(d =>
                        CurrentPeriod switch
                        {
                            "Day" => d.Time.ToString("HH:mm"),
                            "Week" => d.Time.ToString("ddd dd-MM"),
                            "Month" => d.Time.ToString("dd-MM"),
                            _ => d.Time.ToString("HH:mm")
                        }).ToList();

                    timePoints = downsampledGroup.Select(d => d.Time).ToList();
                }

                if (selectedValues.All(v => v == -1))
                {
                    Console.WriteLine($"Bỏ sensor {group.Key} vì toàn bộ giá trị = -1");
                    continue; // bỏ qua sensor này
                }


                seriesList.Add(new LineSeries<double>
                {
                    Values = selectedValues,
                    Fill = null,
                    Name = group.Key,
                    LineSmoothness = 0,
                    GeometrySize = 0,
                    Stroke = new SolidColorPaint
                    {
                        Color = colorMap.ContainsKey(group.Key) ? colorMap[group.Key] : SKColors.Gray,
                        StrokeThickness = 2
                    }
                });
            }

            // Xử lý timeLabels


            // Bình thường: skip cách đều
            //var skip = Math.Max(1, timePoints.Count / 12); // hiển thị 6 mốc
            //timeLabels = allLabels
            //    .Select((label, index) => index % skip == 0 ? label : "")
            //    .ToList();

            var skip = Math.Max(1, allLabels.Count / 20);

            timeLabels = allLabels
                .Select((label, index) => (index % skip == 0) ? label : "")
                .ToList();



            Series = seriesList.ToArray();

            XAxes = new Axis[]
            {
        new Axis
        {
            Labels = timeLabels.ToArray(),
            Name = "Time",
            LabelsRotation = 15,
            LabelsPaint = new SolidColorPaint(SKColors.Black),
            TextSize = 12,
            SeparatorsPaint = new SolidColorPaint(SKColors.LightGray),
                    MinStep = 1, // thêm dòng này để mỗi điểm là 1 label (kể cả label trống)
        ForceStepToMin = true, // ép phải theo MinStep
                // Thêm 2 dòng này
       
        }
            };

            OnPropertyChanged(nameof(Series));
            OnPropertyChanged(nameof(XAxes));
        }



        public void SetSelectedParameter(string parameter)
        {
            selectedParameter = parameter;
        }

        private void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));


    }
}
