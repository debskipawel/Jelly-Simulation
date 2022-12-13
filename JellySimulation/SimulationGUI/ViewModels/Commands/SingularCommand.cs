using System;
using System.Windows.Input;

namespace SpringSimulationGUI.ViewModels.Commands
{
    public class SingularCommand : ICommand
    {
        protected Action<object> action;

        public event EventHandler? CanExecuteChanged;

        public SingularCommand(Action<object> action)
        {
            this.action = action;
        }

        public bool CanExecute(object? parameter)
        {
            return true;
        }

        public void Execute(object? parameter)
        {
            this.action.Invoke(parameter);
        }
    }
}
