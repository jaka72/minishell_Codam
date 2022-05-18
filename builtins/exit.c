#include "builtins.h"

/*
exit 123 456	writes exit, but does not exit:   	"too many arguments"
exit 123 qwe	writes exit, but does not exit:   	"too many arguments"
exit asd qwe	writes exit, it exits:				"numeric argument required"

exit					it only exits if only 1 command
ls | exit				does not exit
exit | ls				does not exit
ls | wc | exit			does not exit

*/


int	count_commands(t_cmd *list)
{
	int	i;
	t_cmd *temp;

	temp = list;
	i = 0;
	while (temp)
	{
		//printf("temp->args[0]: [%s]\n", temp->args[0]);
		temp = temp->next;
		i++;
	}
	//printf(YEL"Number of all commands: %d\n"RES, i);
	return (i);
}



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

int	run_exit_builtin(t_cmd *cmd /*, t_infos *info */ , t_cmd *list)
{
	int	nr_commands;
	int	must_exit;
	//printf(GRN"Start run_exit_builtin\n"RES);
	//printf(GRN"Print temp for testing: %s %d\n"RES, cmd->args[0], info->ini_fd[0]);

	must_exit = 0;
	nr_commands = count_commands(list);
	printf(GRN"   ret: %d\n"RES, nr_commands);

	if (nr_commands == 1)
	{
		printf(RED"exit\n"RES);
		must_exit = 1;
	}

	if (cmd->count_args > 1)
	{
		if (check_if_numeric(cmd->args[1]) != 0)
		{
			write(2, "minishell: exit: numeric argument required\n", 43);
		}
		else if (cmd->count_args > 2)
		{
			write(2, "minishell: exit: too many arguments\n", 37);
		}
	}

	if (must_exit == 1)
		return (222);
	
	return (1);
}
