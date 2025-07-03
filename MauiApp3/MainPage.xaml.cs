namespace MauiApp3
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent(); // cần dòng này để load XAML
        }

        private void OnDayClicked(object sender, EventArgs e)
        {
            ResetPeriodButtonBackgrounds();
            (BindingContext as ViewModel).UpdateSeries("Day");
            UpdatePeriodButtonStates("Day");
        }

        // Handle Week button click
        private void OnWeekClicked(object sender, EventArgs e)
        {
            ResetPeriodButtonBackgrounds();
            (BindingContext as ViewModel).UpdateSeries("Week");
            UpdatePeriodButtonStates("Week");
        }

        // Handle Month button click
        private void OnMonthClicked(object sender, EventArgs e)
        {
            ResetPeriodButtonBackgrounds();
            (BindingContext as ViewModel).UpdateSeries("Month");
            UpdatePeriodButtonStates("Month");
        }

        private async void OnTemperatureClicked(object sender, EventArgs e)
        {
            ResetParameterButtonBackgrounds();
            var vm = BindingContext as ViewModel;
            vm?.SetSelectedParameter("Temperature");
            UpdateParameterButtonStates("Temperature");
            await Task.Delay(50);
            vm?.UpdateSeries(vm.CurrentPeriod);
        }

        private async void OnHumidityClicked(object sender, EventArgs e)
        {
            ResetParameterButtonBackgrounds();
            var vm = BindingContext as ViewModel;
            vm?.SetSelectedParameter("Humidity");
            UpdateParameterButtonStates("Humidity");
            await Task.Delay(50);
            vm?.UpdateSeries(vm.CurrentPeriod);
        }

        private async void OnPressureClicked(object sender, EventArgs e)
        {
            ResetParameterButtonBackgrounds();
            var vm = BindingContext as ViewModel;
            vm?.SetSelectedParameter("Pressure");
            UpdateParameterButtonStates("Pressure");
            await Task.Delay(50);
            vm?.UpdateSeries(vm.CurrentPeriod);
        }

        private async void OnLightClicked(object sender, EventArgs e)
        {
            ResetParameterButtonBackgrounds();
            var vm = BindingContext as ViewModel;
            vm?.SetSelectedParameter("Light");
            UpdateParameterButtonStates("Light");
            await Task.Delay(50);
            vm?.UpdateSeries(vm.CurrentPeriod);
        }

        private void UpdatePeriodButtonStates(string selected)
        {
            var gray = Colors.LightGray;

            switch (selected)
            {
                case "Day":
                    DayButton.BackgroundColor = gray;
                    break;
                case "Week":
                    WeekButton.BackgroundColor = gray;
                    break;
                case "Month":
                    MonthButton.BackgroundColor = gray;
                    break;
            }
        }

        private void UpdateParameterButtonStates(string selected)
        {
            var gray = Colors.LightGray;
            

           
            switch (selected)
            {
                case "Temperature":
                    TemperatureButton.BackgroundColor = gray;
                    break;
                case "Humidity":
                    HumidityButton.BackgroundColor = gray;
                    break;
                case "Pressure":
                    PressureButton.BackgroundColor = gray;
                    break;
                case "Light":
                    LightButton.BackgroundColor = gray;
                    break;
            }
        }

        private void ResetPeriodButtonBackgrounds()
        {
            DayButton.BackgroundColor = Colors.DarkCyan;
            WeekButton.BackgroundColor = Colors.DarkCyan;
            MonthButton.BackgroundColor = Colors.DarkCyan;
        }

        private void ResetParameterButtonBackgrounds()
        {
            TemperatureButton.BackgroundColor = Colors.DarkCyan;
            HumidityButton.BackgroundColor = Colors.DarkCyan;
            PressureButton.BackgroundColor = Colors.DarkCyan;
            LightButton.BackgroundColor = Colors.DarkCyan;
        }

        private async void OnBackToHomeClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new HomePage());
        }




    }

}
