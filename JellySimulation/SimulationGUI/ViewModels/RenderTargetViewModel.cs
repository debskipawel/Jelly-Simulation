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

        protected delegate void MouseMoveEventHandler(float dx, float dy);
        protected MouseMoveEventHandler MouseMoveHandler;

        public RenderTargetViewModel()
        {
            this.MouseMoveHandler = new MouseMoveEventHandler((_, _) => { });
        }

        public void MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.MiddleButton != MouseButtonState.Pressed && e.LeftButton != MouseButtonState.Pressed && e.RightButton != MouseButtonState.Pressed)
            {
                return;
            }

            this.inMoveMode = true;

            var p = e.GetPosition(sender as IInputElement);
            this.x = (float)p.X;
            this.y = (float)p.Y;

            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.MouseMoveHandler += this.MoveOnLeftClick;
            }

            if (e.MiddleButton == MouseButtonState.Pressed)
            {
                this.MouseMoveHandler += this.MoveOnMiddleClick;
            }

            if (e.RightButton == MouseButtonState.Pressed)
            {
                this.MouseMoveHandler += this.MoveOnRightClick;
            }
        }

        public void MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Released)
            {
                this.MouseMoveHandler -= this.MoveOnLeftClick;
            }

            if (e.MiddleButton == MouseButtonState.Released)
            {
                this.MouseMoveHandler -= this.MoveOnMiddleClick;
            }

            if (e.RightButton == MouseButtonState.Released)
            {
                this.MouseMoveHandler -= this.MoveOnRightClick;
            }

            if (e.MiddleButton != MouseButtonState.Pressed && e.LeftButton != MouseButtonState.Pressed && e.RightButton != MouseButtonState.Pressed)
            {
                this.inMoveMode = false;
            }
        }

        public void MouseMove(object sender, MouseEventArgs e)
        {
            if (!this.inMoveMode)
            {
                return;
            }

            var p = e.GetPosition(sender as IInputElement);

            var dx = (float)p.X - this.x;
            var dy = (float)p.Y - this.y;

            dx = moveSensitivity * dx * MathF.PI / 180.0f;
            dy = moveSensitivity * dy * MathF.PI / 180.0f;

            this.MouseMoveHandler.Invoke(dx, dy);

            this.x = (float)p.X;
            this.y = (float)p.Y;
        }

        public void MouseWheel(object sender, MouseWheelEventArgs e)
        {
            float dd = -e.Delta * zoomSensitivity;

            InteropMethods.Scroll(dd);
        }

        private void MoveOnLeftClick(float dx, float dy)
        {
            InteropMethods.MoveOnLeftClick(dx, dy);
        }

        private void MoveOnMiddleClick(float dx, float dy)
        {
            InteropMethods.MoveOnMiddleClick(dx, dy, Keyboard.IsKeyDown(Key.LeftShift));
        }

        private void MoveOnRightClick(float dx, float dy)
        {
            InteropMethods.MoveOnRightClick(dx, dy);
        }
    }
}
