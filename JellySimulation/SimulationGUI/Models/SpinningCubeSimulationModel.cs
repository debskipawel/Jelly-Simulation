namespace SpringSimulationGUI.Models
{
    public struct SpinningCubeSimulationModel
    {
        public SpinningCubeSimulationModel()
        {
            this.EdgeLength = 2.0f;
            this.Density = 3.0f;
            this.Deviation = 15.0f;
            this.AngularVelocity = 5.0f;
            this.IntegrationStep = 0.01f;
        }

        public float EdgeLength { get; set; }
        public float Density { get; set; }
        public float Deviation { get; set; }
        public float AngularVelocity { get; set; }
        public float IntegrationStep { get; set; }
    }
}
