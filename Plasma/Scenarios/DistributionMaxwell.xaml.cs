using System;
using System.Collections.Generic;
using System.IO;
using Windows.Storage;
using Windows.Storage.Provider;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using PerformanceComputing;
using System.Threading.Tasks;

// Шаблон элемента пустой страницы задокументирован по адресу http://go.microsoft.com/fwlink/?LinkId=234238

namespace Plasma.Scenarios
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public sealed partial class DistributionMaxwell : Page
    {
        public DistributionMaxwell()
        {
            this.InitializeComponent();
            DistrMaxwell.ResourceBinding("DistributionMaxwell");
        }

        public class ElectronModel
        {
            public ElectronModel()
            {
                this.MyElectronPlotModel = new PlotModel { Title = "Распределение Максвелла для частиц электрона" };
                //this.MyElectronPlotModel.Series.Add(new FunctionSeries(Math.Cos, 0, 10, 0.1, "cos(x)"));
            }

            public void BindingSource(IList<int> distributionList)
            {
                var column = new ColumnSeries { Title = "Electron", StrokeColor = OxyColors.Black, StrokeThickness = 1 };
                var count = 0;
                for (int i = 0; i < 20; i++)
                {
                    column.Items.Add(new ColumnItem { Value = distributionList[count] });
                    count += 49;
                }
                //foreach (var item in distributionList)
                //{
                //    column.Items.Add(new ColumnItem { Value = item });
                //}
                this.MyElectronPlotModel.Series.Add(column);
            }

            public PlotModel MyElectronPlotModel { get; private set; }
        }

        private async void ComparePdf_Click(object sender, RoutedEventArgs e)
        {
            switch (PivotMaxwell.SelectedIndex)
            {
                case 0:
                    ElectronPlot.Model = await ComparePdfElectron(3.75);
                    break;
                case 1:
                    CarbonPlot.Model = ComparePdfCarbon(1.8);
                    break;
                case 2:
                    HeliumPlot.Model = ComparePdfHelium(1.8);
                    break;
                case 3:
                    ElectronPlot2.Model = await ComparePdfElectron(3.75);
                    CarbonPlot2.Model = ComparePdfCarbon(3.5);
                    HeliumPlot2.Model = ComparePdfHelium(3.5);
                    break;
            }
        }

        private async Task<PlotModel> ComparePdfElectron(Double step)
        {
            //var sp = new PerformanceComputing.SpeedParticle(0, 1100000, Environment.ProcessorCount);
            //await sp.DecompositionSpeedAsync();

            //var t = sp.Electrons;

            //List<double> tempList = new List<double>(970303);

            //foreach (Particle particle in t)
            //{
            //    var speed = particle.Vx + particle.Vy + particle.Vz;
            //    tempList.Add(speed);
            //}

            //tempList.Sort();

            var maxwell = new MaxwellParticleDistribution(0, 1100000, Environment.ProcessorCount);

            await maxwell.DistributionParticleAsync();
            var distributionElectrons = maxwell.ElectronsPdf();



            var plotModel = new PlotModel { Title = "Распределение электронов" };
            var linearBarSeries = new LinearBarSeries
            {
                FillColor = OxyColor.FromArgb(69, 76, 175, 80),
                StrokeThickness = 1,
                StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                Title = "ElectronPDF"
            };
            var categoryAxis = new CategoryAxis
            {
                TickStyle = TickStyle.Outside,
                MajorStep = step,
                Title = "Скорость частиц",
                Unit = "м/с",
                TitleFontSize = 15
            };
            var k = 0;
            for (var i = 0; i < 1100000; i += 11000)
            {
                linearBarSeries.Points.Add(new DataPoint(k++, distributionElectrons[i]));
                categoryAxis.ActualLabels.Add(i.ToString());
            }

            plotModel.Axes.Add(categoryAxis);
            plotModel.Series.Add(linearBarSeries);
            return plotModel;
        }

        //private int ii = 0;
        private PlotModel ComparePdfCarbon(Double step)
        {
            var maxwell = new MaxwellParticleDistribution(0, 7000, Environment.ProcessorCount);
            var distributionElectrons = maxwell.CarbonsPdf();

            var plotModel = new PlotModel { Title = "Распределение частиц углерода" };
            var linearBarSeries = new LinearBarSeries
            {
                FillColor = OxyColor.FromArgb(69, 76, 175, 80),
                StrokeThickness = 1,
                StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                Title = "CarbonPDF"
            };
            var categoryAxis = new CategoryAxis
            {
                TickStyle = TickStyle.Outside,
                MajorStep = step,
                Title = "Скорость частиц",
                Unit = "м/с",
                TitleFontSize = 15
            };
            var k = 0;
            for (int i = 0; i < 7000; i += 100)
            {
                linearBarSeries.Points.Add(new DataPoint(k++, distributionElectrons[i]));
                categoryAxis.ActualLabels.Add(i.ToString());
            }
            plotModel.Axes.Add(categoryAxis);
            plotModel.Series.Add(linearBarSeries);
            return plotModel;
        }

        private PlotModel ComparePdfHelium(Double step)
        {
            var maxwell = new MaxwellParticleDistribution(0, 7000, Environment.ProcessorCount);
            var distributionElectrons = maxwell.HeliumsPdf();

            var plotModel = new PlotModel { Title = "Распределение частиц гелия" };
            var linearBarSeries = new LinearBarSeries
            {
                FillColor = OxyColor.FromArgb(69, 76, 175, 80),
                StrokeThickness = 1,
                StrokeColor = OxyColor.FromArgb(255, 76, 175, 80),
                Title = "HeliumPDF"
            };
            var categoryAxis = new CategoryAxis
            {
                TickStyle = TickStyle.Outside,
                MajorStep = step,
                Title = "Скорость частиц",
                Unit = "м/с",
                TitleFontSize = 15
            };
            var k = 0;
            for (int i = 0; i < 7000; i += 100)
            {
                linearBarSeries.Points.Add(new DataPoint(k++, distributionElectrons[i]));
                categoryAxis.ActualLabels.Add(i.ToString());
            }
            plotModel.Axes.Add(categoryAxis);
            plotModel.Series.Add(linearBarSeries);
            return plotModel;
        }

        private void Save_Click(object sender, RoutedEventArgs e)
        {
            switch (PivotMaxwell.SelectedIndex)
            {
                case 0:
                    SaveDocument(ElectronPlot.Model, "Распределение электронов");
                    break;
                case 1:
                    SaveDocument(CarbonPlot.Model, "Распределение частиц углерода");
                    break;
                case 2:
                    SaveDocument(HeliumPlot.Model, "Распределение частиц гелия");
                    break;
                default:
                    break;
            }
        }

        private async void SaveDocument(IPlotModel model, String newDocument)
        {
            if (model == null)
            {
                var msg = new MessageDialog("График не создан, рассчитайте распределение", "Ошибка сохранения");
                await msg.ShowAsync();
                return;
            }

            var savePicker = new Windows.Storage.Pickers.FileSavePicker
            {
                SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.PicturesLibrary
            };
            savePicker.FileTypeChoices.Add("PDF Document", new List<string>() { ".pdf" });
            savePicker.SuggestedFileName = newDocument;
            StorageFile file = await savePicker.PickSaveFileAsync();
            if (file != null)
            {
                CachedFileManager.DeferUpdates(file);
                var stream = await file.OpenAsync(FileAccessMode.ReadWrite);

                using (var outputStream = stream.GetOutputStreamAt(0))
                {
                    using (var dataWriter = new Windows.Storage.Streams.DataWriter(outputStream))
                    {
                        using (var memoryStream = new MemoryStream())
                        {
                            var pdf = new PdfExporter();
                            PdfExporter.Export(model, memoryStream, 1000, 400);
                            var bt = memoryStream.ToArray();
                            dataWriter.WriteBytes(bt);
                            await dataWriter.StoreAsync();
                            await outputStream.FlushAsync();
                        }
                    }
                }
                var status = await CachedFileManager.CompleteUpdatesAsync(file);
                if (status == FileUpdateStatus.Complete)
                {
                    var msg = new MessageDialog("По пути " + file.Path, "Файл сохранен");
                    await msg.ShowAsync();
                }
                else
                {
                    var msg = new MessageDialog("Произошла ошибка сохранения");
                    await msg.ShowAsync();
                }
            }
        }
    }
}