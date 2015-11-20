using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using OxyPlot;
using OxyPlot.Series;
using PerformanceComputing;
// Документацию по шаблону элемента "Пустая страница" см. по адресу http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Plasma
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void Button_Click(object sender, RoutedEventArgs r)
        {

            //MaxwellParticleDistribution maxwell = new MaxwellParticleDistribution(0, 7000, Environment.ProcessorCount);
            //await maxwell.DistributionParticleAsync();
            //var e = maxwell.Electrons.Count;
            //var h = maxwell.Heliums.Count;
            //var c = maxwell.Carbons.Count;

            SpeedParticle speedParticle = new SpeedParticle(0, 7000, Environment.ProcessorCount);
            await speedParticle.DecompositionSpeedAsync();

        }
    }

    public class MainViewModel
    {
        public MainViewModel()
        {
            this.MyModel = new PlotModel { Title = "Максвелл" };
            //var list = maxwell.compare();

            //this.Compare(list);
            //this.MyModel.Series.Add(new FunctionSeries(Math.Cos, 0, 10, 0.1, "cos(x)"));
        }

        void Compare(IList<double> list)
        {
            var column = new ColumnSeries { Title = "Пример", StrokeColor = OxyColors.Black, StrokeThickness = 1 };
            var count = 0;
            //for (int i = 0; i < 20; i++)
            //{
            //    column.Items.Add(new ColumnItem { Value = list[count] });
            //    count += 49;
            //}

            foreach (double item in list)
            {
                column.Items.Add(new ColumnItem { Value = item });
            }
            this.MyModel.Series.Add(column);
        }

        public PlotModel MyModel { get; private set; }
    }
}
