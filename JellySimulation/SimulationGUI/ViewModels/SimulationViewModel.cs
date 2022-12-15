using SpringSimulationGUI.Interop;
using SimulationGUI.Models;
using SpringSimulationGUI.ViewModels.Commands;
using System.Windows.Input;

namespace SpringSimulationGUI.ViewModels
{
    public class SimulationViewModel : ObservableObject
    {
        protected JellySimulationModel model;

        public ICommand RestartSimulationCommand { get; set; }

        public float ControlPointMass
        {
            get => model.ControlPointMass;
            set { model.ControlPointMass = value; NotifyPropertyChanged(); }
        }

        public float Stickiness
        {
            get => model.Stickiness;
            set { model.Stickiness = value; NotifyPropertyChanged(); }
        }

        public float ElasticityBetweenMasses
        {
            get => model.ElasticityBetweenMasses;
            set { model.ElasticityBetweenMasses = value; NotifyPropertyChanged(); }
        }

        public float ElasticityOnSteeringSprings
        {
            get => model.ElasticityOnSteeringSprings;
            set { model.ElasticityOnSteeringSprings = value; NotifyPropertyChanged(); }
        }
        public float ElasticityOnCollitions
        {
            get => model.ElasticityOnCollitions;
            set { model.ElasticityOnCollitions = value; NotifyPropertyChanged(); }
        }

        public float MaxInitialImbalance
        {
            get => model.MaxInitialImbalance;
            set { model.MaxInitialImbalance = value; NotifyPropertyChanged(); }
        }

        public SimulationViewModel()
        {   
            this.model = new();
            this.RestartSimulationCommand = new RelayCommand(RestartSimulation, (_) => true);
        }

        private void RestartSimulation()
        {
            InteropMethods.RestartSimulation(ControlPointMass, Stickiness, ElasticityBetweenMasses, ElasticityOnSteeringSprings, ElasticityOnCollitions, MaxInitialImbalance);
        }
    }
}
