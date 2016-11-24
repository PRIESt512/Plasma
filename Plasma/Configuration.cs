using System;
using System.Collections.Generic;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml.Controls;
using Plasma.Scenarios;

namespace Plasma
{
    public partial class MainPage : Page
    {
    }

    public class Scenario
    {
        public string Title { get; set; }
        public Type ClassType { get; set; }
    }

    public static class MyExtensions
    {
        /// <summary>
        /// Загружает информацию из источника
        /// </summary>
        /// <param name="b"></param>
        public static void ResourceBinding(this TextBlock b, String source)
        {   
            var rl = new ResourceLoader();
            b.Text = rl.GetString(source);
        }
    }
}

