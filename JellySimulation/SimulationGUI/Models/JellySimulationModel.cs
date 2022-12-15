namespace SimulationGUI.Models
{
    public struct JellySimulationModel
    {
        public JellySimulationModel()
        {
            this.ControlPointMass = 1.0f;
            this.Stickiness = 0.1f;
            this.ElasticityBetweenMasses = 1.0f;
            this.ElasticityOnSteeringSprings = 1.0f;
            this.MaxInitialImbalance = 0.05f;
            this.ElasticityOnCollitions = 0.8f;
        }

        public float ControlPointMass { get; set; }
        public float Stickiness { get; set; }
        public float ElasticityBetweenMasses { get; set; }
        public float ElasticityOnSteeringSprings { get; set; }
        public float ElasticityOnCollitions { get; set; }
        
        public float MaxInitialImbalance { get; set; }
    }
}
