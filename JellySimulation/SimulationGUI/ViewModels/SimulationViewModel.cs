using SpringSimulationGUI.Interop;
using SpringSimulationGUI.Models;
using SpringSimulationGUI.ViewModels.Commands;

namespace SpringSimulationGUI.ViewModels
{
    public class SimulationViewModel : ObservableObject
    {
        protected SpinningCubeSimulationModel model;

        public RelayCommand Resume { get; protected set; }
        
        public RelayCommand Pause { get; protected set; }
        
        public RelayCommand Restart { get; protected set; }

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

        public bool GravityOn
        {
            get => model.GravityOn;
            set { 
                model.GravityOn = value; 
                NotifyPropertyChanged();
                InteropMethods.ToggleGravity(value);
            }
           
        }
        public SimulationViewModel()
        {   
            this.IsPaused = false;
            this.WasStarted = false;

            this.model = new SpinningCubeSimulationModel();

            this.Resume = new RelayCommand
                (
                    () => this.ResumeSimulation(),
                    (object _) => this.IsPaused
                );

            this.Pause = new RelayCommand
            (
                () => this.PauseSimulation(),
                (object _) => !this.IsPaused && this.WasStarted
            );

            this.Restart = new RelayCommand
            (
                () => this.RestartSimulation(),
                (object _) => true
            );
        }

        protected void ResumeSimulation()
        {
            this.IsPaused = false;

            InteropMethods.OnSimulationResume();
        }

        protected void PauseSimulation()
        {
            this.IsPaused = true;

            InteropMethods.OnSimulationPause();
        }

        protected void RestartSimulation()
        {
            this.WasStarted = true;
            this.IsPaused = false;

            InteropMethods.OnSimulationRestart(
                model.EdgeLength,
                model.Density,
                model.Deviation,
                model.AngularVelocity,
                model.IntegrationStep,
                model.GravityOn
            );
        }
    }
}
