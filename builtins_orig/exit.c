#include "builtins.h"

/*
exit 123 456	writes exit, but does not exit:   	"too many arguments"
exit 123 qwe	writes exit, but does not exit:   	"too many arguments"

exit asd		writes exit, it exits:				"numeric argument required"
exit asd qwe	writes exit, it exits:				"numeric argument required"
exit asd 123	writes exit, it exits:				"numeric argument required"

exit					it only exits if only 1 command
ls | exit				does not exit
exit | ls				does not exit
ls | wc | exit			does not exit

*/


// int	count_commands(t_cmd *list)
// {
// 	int	i;
// 	t_cmd *temp;

// 	temp = list;
// 	i = 0;
// 	while (temp)
// 	{
// 		//printf("temp->args[0]: [%s]\n", temp->args[0]);
// 		temp = temp->next;
// 		i++;
// 	}
// 	//printf(YEL"Number of all commands: %d\n"RES, i);
// 	return (i);
// }



int check_if_numeric(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}


// IT WILL NEED THE *list	STRUCT FOR COUNTING NR OF COMMANDS,
// UNLESS THERE IS ANOTHER WAY OF DOING IT

int	run_exit_builtin(t_cmd *cmd /*, t_infos *info */ /*, t_cmd *list */)
{
	// int	dont_exit;
	// int	exit_code;

	// exit_code = 0;
	// if (cmd->args[1])
	// 	exit_code = ft_atoi(cmd->args[1]);
	// dont_exit = 0;
	if (cmd->count_args == 1)
	{
		write(cmd->fd_out, "exit\n", 5);
		// dont_exit = 0;
		exit (0);
	}
	else if (cmd->count_args > 1)
	{
		// write(1, "exit\n", 5); // WRITE EXIT, ONLY IF NO PIPES !!!!
		if (check_if_numeric(cmd->args[1]) != 0)
		{
			write(2, "minishell: exit: ", 17);
			write(2, cmd->args[1], ft_strlen(cmd->args[1]));
			write(2, ": numeric argument required\n", 28);
			return (255);
		}
		else if (check_if_numeric(cmd->args[1]) == 0 && cmd->count_args > 2)
		{
			// dont_exit = 1;
			write(2, "exit\n", 5);
			write(2, "minishell: exit: too many arguments\n", 36);
			exit (1);
		}
		else if (check_if_numeric(cmd->args[1]) == 0 && cmd->count_args == 2)
		{
			// dont_exit = 1;
			write(2, "exit\n", 5);
			exit (ft_atoi(cmd->args[1]));
		}
	}

	// if (dont_exit == 1)
	// 	return (0);
	// // exit (exit_code);
	return (0);
}

/////////////////////////////////////////////////////////