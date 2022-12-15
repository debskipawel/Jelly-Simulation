﻿using SpringSimulationGUI.Interop;

namespace SpringSimulationGUI.ViewModels
{
    public class VisualizationViewModel : ObservableObject
    {
        private bool _drawControlPoints = true;
        private bool _drawSteeringFrame = false;
        private bool _drawShadedCube = false;
        private bool _drawBoundingCuboid = true;

        public bool DrawControlPoints
        {
            get { return _drawControlPoints; }
            set 
            { 
                _drawControlPoints = value; 
                NotifyPropertyChanged();
                UpdateVisualizationParameters();
            }
        }

        public bool DrawSteeringFrame
        {
            get { return _drawSteeringFrame; }
            set 
            { 
                _drawSteeringFrame = value; 
                NotifyPropertyChanged();
                UpdateVisualizationParameters();
            }
        }

        public bool DrawShadedCube
        {
            get { return _drawShadedCube; }
            set 
            { 
                _drawShadedCube = value; 
                NotifyPropertyChanged();
                UpdateVisualizationParameters();
            }
        }
        public bool DrawBoundingCuboid
        {
            get { return _drawBoundingCuboid; }
            set
            {
                _drawBoundingCuboid = value;
                NotifyPropertyChanged();
                UpdateVisualizationParameters();
            }
        }

        protected void UpdateVisualizationParameters()
        {
            InteropMethods.UpdateVisualizationParameters(_drawControlPoints, _drawSteeringFrame, _drawShadedCube, _drawBoundingCuboid);
        }
    }
}
