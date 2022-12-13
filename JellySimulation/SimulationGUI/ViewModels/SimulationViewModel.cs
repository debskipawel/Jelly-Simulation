using SpringSimulationGUI.Interop;
using SpringSimulationGUI.Models;
using SpringSimulationGUI.ViewModels.Commands;

namespace SpringSimulationGUI.ViewModels
{
    public class SimulationViewModel : ObservableObject
    {
        protected SpinningCubeSimulationModel model;

        public bool IsPaused { get; set; }

        public bool WasStarted { get; set; }

        public float EdgeLength
        {
            get => model.EdgeLength;
            set { model.EdgeLength = value; NotifyPropertyChanged(); }
        }

        public float Density
        {
            get => model.Density;
            set { model.Density = value; NotifyPropertyChanged(); }
        }

        public float Deviation
        {
            get => model.Deviation;
            set { model.Deviation = value; NotifyPropertyChanged(); }
        }

        public float AngularVelocity
        {
            get => model.AngularVelocity;
            set { model.AngularVelocity = value; NotifyPropertyChanged(); }
        }

        public float IntegrationStep
        {
            get => model.IntegrationStep;
            set { model.IntegrationStep = value; NotifyPropertyChanged(); }
        }

        public SimulationViewModel()
        {   
            this.IsPaused = false;
            this.WasStarted = false;

            this.model = new SpinningCubeSimulationModel();
        }
    }
}
