using System;
using System.Windows.Input;

namespace SpringSimulationGUI.ViewModels.Commands
{
    public class RelayCommand : ICommand
    {
        public Action action;
        public Predicate<object> predicate;

        public RelayCommand(Action action, Predicate<object> predicate)
        {
            this.action = action;
            this.predicate = predicate;
        }

        public event EventHandler? CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public bool CanExecute(object? parameter)
        {
            return predicate(parameter);
        }

        public void Execute(object? parameter)
        {
            this.action.Invoke();
        }
    }
}
