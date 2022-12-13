using SpringSimulationGUI.Interop;
using System;
using System.Windows;
using System.Windows.Input;

namespace SpringSimulationGUI.ViewModels
{
    public class RenderTargetViewModel : ObservableObject
    {
        protected bool inMoveMode;
        protected float x, y;

        protected float moveSensitivity = 1.0f;
        protected float zoomSensitivity = 0.01f;

        public RenderTargetViewModel()
        {
        }

        public void MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.MiddleButton == MouseButtonState.Pressed)
            {
                inMoveMode = true;
                var p = e.GetPosition(sender as IInputElement);
                
                this.x = (float)p.X;
                this.y = (float)p.Y;
            }
        }

        public void MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.MiddleButton == MouseButtonState.Released)
            {
                inMoveMode = false; 
            }
        }

        public void MouseMove(object sender, MouseEventArgs e)
        {
            if (!inMoveMode)
            {
                return;
            }

            var p = e.GetPosition(sender as IInputElement);

            var dx = (float)p.X - x;
            var dy = (float)p.Y - y;

            dx = moveSensitivity * dx * MathF.PI / 180.0f;
            dy = moveSensitivity * dy * MathF.PI / 180.0f;

            InteropMethods.MoveCamera(dx, dy);

            this.x = (float)p.X;
            this.y = (float)p.Y;
        }

        public void MouseWheel(object sender, MouseWheelEventArgs e)
        {
            float dd = -e.Delta * zoomSensitivity;

            InteropMethods.Zoom(dd);
        }
    }
}
