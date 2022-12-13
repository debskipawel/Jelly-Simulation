using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using LiveChartsCore;
using LiveChartsCore.SkiaSharpView;
using SpringSimulationGUI.Interop;

namespace SpringSimulationGUI
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            LiveCharts.Configure(config =>
                config
                    .AddSkiaSharp()
                    .AddDefaultMappers()
                    .AddDarkTheme()
                    // finally register your own mappers
                    // you can learn more about mappers at:
                    // ToDo add website link...
                    //.HasMap<City>((city, point) =>
                    //{
                    //    point.PrimaryValue = city.Population;
                    //    point.SecondaryValue = point.Context.Index;
                    //})
                // .HasMap<Foo>( .... )
                // .HasMap<Bar>( .... )
                );
        }

        private void App_OnExit(object sender, EventArgs e)
        {
            InteropMethods.InvokeWithDllProtection(() => InteropMethods.Cleanup());
        }

        private void App_OnShutdown(object sender, SessionEndingCancelEventArgs e)
        {
            InteropMethods.InvokeWithDllProtection(() => InteropMethods.Cleanup());
        }
    }
}
