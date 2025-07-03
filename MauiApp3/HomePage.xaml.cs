namespace MauiApp3;

public partial class HomePage : ContentPage
{
    private HomePageViewModel _viewModel;

    public HomePage()
    {
        InitializeComponent();
        _viewModel = new HomePageViewModel();
        BindingContext = _viewModel;
    }

    private async void OnChartButtonClicked(object sender, EventArgs e)
    {
        await Shell.Current.GoToAsync("MainPage");
    }

    private void OnNextClicked(object sender, EventArgs e)
    {
        _viewModel.NextSensor();
    }

    private void OnPreviousClicked(object sender, EventArgs e)
    {
        _viewModel.PreviousSensor();
    }
}
