using System;
using System.Runtime.InteropServices;
using System.Windows;

namespace SpringSimulationGUI.Interop
{
    static internal class InteropMethods
    {
        /// <summary>
        /// Variable used to track whether the missing dependency dialog has been displayed,
        /// used to prevent multiple notifications of the same failure.
        /// </summary>
        private static bool errorHasDisplayed;

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Init();

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Cleanup();

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Render(IntPtr resourcePointer, bool isNewSurface);

        
        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveOnLeftClick(float dx, float dy);

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveOnMiddleClick(float dx, float dy);

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveOnRightClick(float dx, float dy);

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Scroll(float dd);

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void UpdateVisualizationParameters(bool drawControlPoints, bool drawSteeringCube, bool drawShadedCube);

        [DllImport("../SimulationVisualization.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RestartSimulation(float pointMass, float stickiness, float massesElasticity, float steeringSpringsElasticity, float maxImbalance);

        /// <summary>
        /// Method used to invoke an Action that will catch DllNotFoundExceptions and display a warning dialog.
        /// </summary>
        /// <param name="action">The Action to invoke.</param>
        public static void InvokeWithDllProtection(Action action)
        {
            InvokeWithDllProtection(
                () =>
                {
                    action.Invoke();
                    return 0;
                });
        }

        /// <summary>
        /// Method used to invoke A Func that will catch DllNotFoundExceptions and display a warning dialog.
        /// </summary>
        /// <param name="func">The Func to invoke.</param>
        /// <returns>The return value of func, or default(T) if a DllNotFoundException was caught.</returns>
        /// <typeparam name="T">The return type of the func.</typeparam>
        public static T InvokeWithDllProtection<T>(Func<T> func)
        {
            try
            {
                return func.Invoke();
            }
            catch (DllNotFoundException e)
            {
                if (!errorHasDisplayed)
                {
                    MessageBox.Show("This sample requires:\nManual build of the SpringSimulationVisualization project, which requires installation of Windows 10 SDK or DirectX SDK.\n" +
                                    "Installation of the DirectX runtime on non-build machines.\n\n" +
                                    "Detailed exception message: " + e.Message, "WPF D3D11 Interop",
                                    MessageBoxButton.OK, MessageBoxImage.Error);
                    errorHasDisplayed = true;

                    if (Application.Current != null)
                    {
                        Application.Current.Shutdown();
                    }
                }
            }

            return default;
        }
    }
}
