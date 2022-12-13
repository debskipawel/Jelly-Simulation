using SpringSimulationGUI.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SpringSimulationGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private TimeSpan lastRender;
        private bool lastVisible;

        public MainWindow()
        {
            InitializeComponent();

            this.host.Loaded += new RoutedEventHandler(this.Host_Loaded);
            this.host.SizeChanged += new SizeChangedEventHandler(this.Host_SizeChanged);
        }
        private static void Init()
        {
            var initialized = InteropMethods.InvokeWithDllProtection(InteropMethods.Init);

            if (!initialized)
            {
                MessageBox.Show("Failed to initialize.", "WPF D3D Interop", MessageBoxButton.OK, MessageBoxImage.Error);

                if (Application.Current != null)
                {
                    Application.Current.Shutdown();
                }
            }
        }

        private void InitializeRendering()
        {
            this.InteropImage.WindowOwner = (new System.Windows.Interop.WindowInteropHelper(this)).Handle;
            this.InteropImage.OnRender = this.DoRender;

            this.InteropImage.RequestRender();
        }

        private void UninitializeRendering()
        {
            CompositionTarget.Rendering -= this.CompositionTarget_Rendering;
        }

        private void DoRender(IntPtr surface, bool isNewSurface)
        {
            InteropMethods.Render(surface, isNewSurface);
        }

        private void Host_Loaded(object sender, RoutedEventArgs e)
        {
            Init();
            this.InitializeRendering();
        }

        private void Host_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            double dpiScale = 1.0; // default value for 96 dpi

            // determine DPI
            // (as of .NET 4.6.1, this returns the DPI of the primary monitor, if you have several different DPIs)
            var hwndTarget = PresentationSource.FromVisual(this).CompositionTarget as HwndTarget;
            if (hwndTarget != null)
            {
                dpiScale = hwndTarget.TransformToDevice.M11;
            }

            int surfWidth = (int)(host.ActualWidth < 0 ? 0 : Math.Ceiling(host.ActualWidth * dpiScale));
            int surfHeight = (int)(host.ActualHeight < 0 ? 0 : Math.Ceiling(host.ActualHeight * dpiScale));

            // Notify the D3D11Image of the pixel size desired for the DirectX rendering.
            // The D3DRendering component will determine the size of the new surface it is given, at that point.
            InteropImage.SetPixelSize(surfWidth, surfHeight);

            // Stop rendering if the D3DImage isn't visible - currently just if width or height is 0
            // TODO: more optimizations possible (scrolled off screen, etc...)
            bool isVisible = (surfWidth != 0 && surfHeight != 0);
            if (this.lastVisible != isVisible)
            {
                this.lastVisible = isVisible;
                
                if (lastVisible)
                {
                    CompositionTarget.Rendering += CompositionTarget_Rendering;
                }
                else
                {
                    CompositionTarget.Rendering -= CompositionTarget_Rendering;
                }
            }
        }

        private void CompositionTarget_Rendering(object? sender, EventArgs e)
        {
            RenderingEventArgs args = (RenderingEventArgs)e;

            if (this.lastRender != args.RenderingTime)
            {
                InteropImage.RequestRender();
                this.lastRender = args.RenderingTime;
            }
        }
    }
}
