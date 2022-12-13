using SpringSimulationGUI.Interop;

namespace SpringSimulationGUI.ViewModels
{
    public class VisualizationViewModel : ObservableObject
    {
        protected uint traceLength = 10000;
        protected bool showTrace = false;
        protected bool showCube = true;
        protected bool showDiagonal = true;

        public uint TraceLength
        {
            get => traceLength;
            set 
            { 
                traceLength = value; 
                NotifyPropertyChanged();
                InteropMethods.DisplayTraceChanged(showTrace, traceLength);
            }
        }

        public bool ShowTrace
        {
            get => showTrace;
            set 
            { 
                showTrace = value; 
                NotifyPropertyChanged();
                InteropMethods.DisplayTraceChanged(showTrace, traceLength);
            }
        }

        public bool ShowCube
        {
            get => showCube;
            set 
            { 
                showCube = value; 
                NotifyPropertyChanged();
                InteropMethods.DisplayCubeChanged(value);
            }
        }

        public bool ShowDiagonal
        {
            get => showDiagonal;
            set 
            { 
                showDiagonal = value; 
                NotifyPropertyChanged();
                InteropMethods.DisplayDiagonalChanged(value);
            }
        }

    }
}
