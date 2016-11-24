using System;
using System.Collections.Generic;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using OxyPlot;
using OxyPlot.Series;
using PerformanceComputing;
using Plasma.Scenarios;
using SpeedParticle = Plasma.Scenarios.SpeedParticle;

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

        private void Hamburger_Click(object sender, RoutedEventArgs e)
        {
            MySplitPanel.IsPaneOpen = !MySplitPanel.IsPaneOpen;
        }

        public List<Scenario> Scenarios { get; } = new List<Scenario>
        {
            new Scenario() { Title="DistributionMaxwell", ClassType=typeof(DistributionMaxwell)},
            new Scenario() { Title="SpeedParticle", ClassType=typeof(SpeedParticle)},
        };

        private void DistributionMaxwell_Click(object sender, RoutedEventArgs e)
        {
            var source = ((Button)e.OriginalSource).Name;
            Navigate(source);
        }

        private void SpeedParticle_Click(object sender, RoutedEventArgs e)
        {
            var source = ((Button)e.OriginalSource).Name;
            Navigate(source);
        }

        private void Navigate(String source)
        {
            var s = Scenarios.Find((Scenario item) => item.Title == source);
            if (s != null)
            {
                ScenarioFrame.Navigate(s.ClassType);
            }
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
            //var count = 0;
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
