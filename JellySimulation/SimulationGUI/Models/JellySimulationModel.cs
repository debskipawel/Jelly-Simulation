namespace SimulationGUI.Models
{
    public struct JellySimulationModel
    {
        public JellySimulationModel()
        {
            this.ControlPointMass = 1.0f / 64;
            this.Stickiness = 0.1f;
            this.ElasticityBetweenMasses = 0.5f;
            this.ElasticityOnSteeringSprings = 0.5f;
            this.MaxInitialImbalance = 0.05f;
        }

        public float ControlPointMass { get; set; }
        public float Stickiness { get; set; }
        public float ElasticityBetweenMasses { get; set; }
        public float ElasticityOnSteeringSprings { get; set; }
        public float MaxInitialImbalance { get; set; }
    }
}
