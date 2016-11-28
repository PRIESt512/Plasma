using System;
using System.Threading.Tasks;
using OxyPlot;
using Windows.UI.Xaml.Controls;
using OxyPlot.Axes;
using OxyPlot.Series;
using System.Collections.Generic;
using System.Text;
using PerformanceComputing;
using System.Diagnostics;

// Шаблон элемента пустой страницы задокументирован по адресу http://go.microsoft.com/fwlink/?LinkId=234238

namespace Plasma.Scenarios
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public sealed partial class SpeedParticle : Page
    {
        public SpeedParticle()
        {
            this.InitializeComponent();
            ParticleSpeed.ResourceBinding("SpeedParticle");
        }

        private async void CompareSpeed_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            switch (Speed.SelectedIndex)
            {
                case 0:
                    ElectronPlot.Model = await CompareSpeedElectron(3.75);
                    break;
                case 1:
                    CarbonPlot.Model = await CompareSpeedCarbon(1.8);
                    break;
                case 2:
                    HeliumPlot.Model = await CompareSpeedHelium(1.8);
                    break;
                case 3:
                    ElectronPlot2.Model = await CompareSpeedElectron(3.75);

                    CarbonPlot2.Model = await CompareSpeedCarbon(3.5);
                    HeliumPlot2.Model = await CompareSpeedHelium(3.5);
                    break;
            }
        }

        private async Task<PlotModel> CompareSpeedHelium(Double step)
        {
            var sp = new PerformanceComputing.SpeedParticle(0, 1100000, Environment.ProcessorCount);
            var plotModel = new PlotModel { Title = "Общая скорость гелия" };
            TextBlock item = (TextBlock)ViewSpeed.SelectedItem;
            var selectedItem = item?.Text;

            if (selectedItem != null && selectedItem.Equals("Общая"))
            {
                //await sp.SpeedHeliumsAsync(Dimensionless.IsChecked != null && (bool)Dimensionless.IsChecked);

                await
                    sp.DecompositionSpeedHeliumsAsync(Dimensionless.IsChecked != null && (bool)Dimensionless.IsChecked);

                var speed = sp.HeliumsDecompositionSpeed;

                //var speed = new List<double>(sp.HeliumsFullSpeed);
                //speed.Sort();
                //List<Double> _sp = new List<Double>();

                //var h = (speed[speed.Count - 1] - speed[0]) / 100;

                //for (int i = 0; i < 100; i++)
                //{
                //    var Ai = speed[0] + ((i + 1) - 1) * h;
                //    var Bi = speed[0] + ((i + 2) - 1) * h;
                //    _sp.Add(speed.FindAll(x => x > Ai && x < Bi).Count / (speed.Count * h));
                //}

                var lineSerie = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 76, 175, 80),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vx"
                };

                var lineSerie2 = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 255, 255, 0),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vy"
                };

                var lineSerie3 = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 72, 118, 255),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vx"
                };

                Int32 k = 0;
                for (var i = 0; i < speed.Count; i += 11000)
                {
                    lineSerie.Points.Add(new DataPoint(k++, speed[i].Vx));
                    //lineSerie2.Points.Add(new DataPoint(k++, speed[i].Vy));
                    //lineSerie3.Points.Add(new DataPoint(k++, speed[i].Vz));
                }
                for (var i = 0; i < speed.Count; i += 11000)
                {
                    lineSerie2.Points.Add(new DataPoint(k++, speed[i].Vy));
                }
                for (var i = 0; i < speed.Count; i += 11000)
                {
                    lineSerie3.Points.Add(new DataPoint(k++, speed[i].Vz));
                }


                //var lineSerie = new LineSeries
                //{
                //    StrokeThickness = 2,
                //    MarkerSize = 3,
                //    Title = "Значения скоростей для частиц"
                //};

                //Int32 k = 0;
                //for (var i = 0; i < speed.Count; i += 11000)
                //{
                //    lineSerie.Points.Add(new DataPoint(k++, speed[i]));
                //}

                plotModel.Series.Add(lineSerie);
                plotModel.Series.Add(lineSerie2);
                plotModel.Series.Add(lineSerie3);

            }
            return plotModel;
        }

        private async Task<PlotModel> CompareSpeedCarbon(Double step)
        {
            var sp = new PerformanceComputing.SpeedParticle(0, 1100000, Environment.ProcessorCount);
            var plotModel = new PlotModel { Title = "Общая скорость частиц углерода" };
            TextBlock item = (TextBlock)ViewSpeed.SelectedItem;
            var selectedItem = item?.Text;

            if (selectedItem != null && selectedItem.Equals("Общая"))
            {
                Stopwatch st = new Stopwatch();
                st.Start();
                await
                    sp.DecompositionSpeedCarbonsAsync(Dimensionless.IsChecked != null && (bool)Dimensionless.IsChecked);
                st.Stop();
                var speed = new List<Particle>(sp.CarbonsDecompositionSpeed);

                Compare(speed);

                var savePicker = new Windows.Storage.Pickers.FileSavePicker();
                savePicker.SuggestedStartLocation =
                    Windows.Storage.Pickers.PickerLocationId.DocumentsLibrary;
                savePicker.FileTypeChoices.Add("Plain Text", new List<string>() { ".txt" });
                // Default file name if the user does not type one in or select a file to replace
                savePicker.SuggestedFileName = "Распределение скорости карбона";

                Windows.Storage.StorageFile file = await savePicker.PickSaveFileAsync();
                if (file != null)
                {
                    //StringBuilder text = new StringBuilder();

                    //foreach (var particle in speed)
                    //{
                    //    text.AppendLine(particle.Vx.ToString());
                    //    text.AppendLine(particle.Vy.ToString());
                    //    text.AppendLine(particle.Vz.ToString());
                    //}

                    StringBuilder text = new StringBuilder();
                    text.AppendLine(st.ElapsedMilliseconds.ToString());
                    text.AppendLine(speed[0].x.ToString());
                    text.AppendLine(speed[0].y.ToString());
                    text.AppendLine(speed[0].z.ToString());


                    await Windows.Storage.FileIO.WriteTextAsync(file, text.ToString());

                    Windows.Storage.Provider.FileUpdateStatus status =
                        await Windows.Storage.CachedFileManager.CompleteUpdatesAsync(file);
                    Windows.Storage.CachedFileManager.DeferUpdates(file);
                    // write to file

                }

                var lineSerie = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 76, 175, 80),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vx"
                };

                var lineSerie2 = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 255, 255, 0),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vy"
                };

                var lineSerie3 = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 72, 118, 255),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vz"
                };

                Int32 k = 0;
                for (var i = 0; i < speed.Count; i += 100)
                {
                    lineSerie.Points.Add(new DataPoint(k++, speed[i].Vx));
                }
                for (var i = 0; i < speed.Count; i += 100)
                {
                    lineSerie2.Points.Add(new DataPoint(k++, speed[i].Vy));
                }
                for (var i = 0; i < speed.Count; i += 100)
                {
                    lineSerie3.Points.Add(new DataPoint(k++, speed[i].Vz));
                }

                plotModel.Series.Add(lineSerie);
                plotModel.Series.Add(lineSerie2);
                plotModel.Series.Add(lineSerie3);

            }
            return plotModel;
        }

        private async void Compare(List<Particle> speed)
        {
            var fullSpeed = new List<double>();
            speed.ForEach(p => fullSpeed.Add(p.Vx * p.Vx + p.Vy * p.Vy + p.Vz * p.Vz));
            fullSpeed.Sort();

            var h = (fullSpeed[fullSpeed.Count - 1] - fullSpeed[0]) / 100;
            var _sp = new List<Double>();

            for (int i = 0; i < 100; i++)
            {
                var Ai = fullSpeed[0] + ((i + 1) - 1) * h;
                var Bi = fullSpeed[0] + ((i + 2) - 1) * h;
                _sp.Add(fullSpeed.FindAll(x => x > Ai && x < Bi).Count);
            }

            MaxwellParticleDistribution maxwell = new MaxwellParticleDistribution(0, 1100000, 4);

            await maxwell.DistributionCarbonsAsync();

            var maxwellSpeed = new List<int>(maxwell.Carbons);

            var n = new List<double>();

            for (int i = 0; i < 100; i++)
            {
                var sp = Math.Pow(_sp[i] - 100 * maxwellSpeed[i], 2) / 100 * maxwellSpeed[i] * (1 - maxwellSpeed[i]);
                n.Add(sp);
            }

            double sum = 0;
            n.ForEach(p => sum += p);

        }

        private async Task<PlotModel> CompareSpeedElectron(Double step)
        {
            var sp = new PerformanceComputing.SpeedParticle(0, 1100000, Environment.ProcessorCount);
            var plotModel = new PlotModel { Title = "Общая скорость электронов" };
            TextBlock item = (TextBlock)ViewSpeed.SelectedItem;
            var selectedItem = item?.Text;

            if (selectedItem != null && selectedItem.Equals("Общая"))
            {
                await
                    sp.DecompositionSpeedElectronsAsync(Dimensionless.IsChecked != null && (bool)Dimensionless.IsChecked);

                var speed = sp.ElectronsDecompositionSpeed;

                var lineSerie = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 76, 175, 80),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vx"
                };

                var lineSerie2 = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 255, 255, 0),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vy"
                };

                var lineSerie3 = new LinearBarSeries
                {
                    FillColor = OxyColor.FromArgb(69, 72, 118, 255),
                    StrokeThickness = 1,
                    StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                    Title = "Распределение Vz"
                };

                Int32 k = 0;
                for (var i = 0; i < speed.Count; i += 11000)
                {
                    lineSerie.Points.Add(new DataPoint(k++, speed[i].Vx));
                    lineSerie2.Points.Add(new DataPoint(k++, speed[i].Vy));
                    lineSerie3.Points.Add(new DataPoint(k++, speed[i].Vz));
                }
                //for (var i = 0; i < speed.Count; i += 11000)
                //{
                //    lineSerie2.Points.Add(new DataPoint(k++, speed[i].Vy));
                //}
                //for (var i = 0; i < speed.Count; i += 11000)
                //{
                //    lineSerie3.Points.Add(new DataPoint(k++, speed[i].Vz));
                //}

                plotModel.Series.Add(lineSerie);
                plotModel.Series.Add(lineSerie2);
                plotModel.Series.Add(lineSerie3);

            }
            return plotModel;
        }

        private void Save_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {

        }
    }
}
