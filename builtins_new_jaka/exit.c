#include "builtins.h"

/*
exit 123 456	writes exit, but does not exit:   	"too many arguments"		ec: 1	ok
exit 123 qwe	writes exit, but does not exit:   	"too many arguments"			1	ok

exit asd		writes exit, it exits:				"numeric argument required"		2	ok
exit asd qwe	writes exit, it exits:				"numeric argument required"		2	ok
exit asd 123	writes exit, it exits:				"numeric argument required"		2	ok

exit					it only exits if 1 command
ls | exit				does not exit, no output, no exit message								KO
ls | exit qwe			does not exit, no exit message, error: "num arg required"				ok
ls | exit 123			does not exit, no output, no exit message								KO
ls | exit 123 qwe		does not exit, no output, no exit message								KO
ls | exit asd qwe		does not exit, no output, no exit message								KO

exit | ls				does not exit, ls output, no exit message								ok
ls | wc | exit			does not exit, no output, no exit message

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
	int	nr_commands;	// pipe or no pipe

	nr_commands = count_commands(list);
	printf(GRN"nr_commands: %d\n"RES, nr_commands);
	if (cmd->count_args == 1)
	{
		if (nr_commands == 1)
			write(cmd->fd_out, "exit a\n", 7);
		exit (0);
	}
	else if (cmd->count_args > 1)
	{
		// write(1, "exit\n", 5); // WRITE EXIT, ONLY IF NO PIPES !!!!
		if (check_if_numeric(cmd->args[1]) != 0 && cmd->count_args >= 2)
		{
			if (nr_commands == 1)
				write(2, "exit d\n", 7);
			write(2, "minishell: exit: ", 17);
			write(2, cmd->args[1], ft_strlen(cmd->args[1]));
			write(2, ": numeric argument required\n", 28);
			exit (2);
			// return (255);
		}
		else if (check_if_numeric(cmd->args[1]) == 0 && cmd->count_args > 2)
		{
			// dont_exit = 1;
			if (nr_commands == 1)
				write(2, "exit c\n", 7);
			write(2, "minishell: exit: too many arguments\n", 36);
			//g_status = 1;
			return (1);
			//exit (1);
		}
		else if (check_if_numeric(cmd->args[1]) == 0 && cmd->count_args == 2)
		{
			// dont_exit = 1;
			if (nr_commands == 1)
				write(2, "exit b\n", 7);
			exit (ft_atoi(cmd->args[1]));
		}
	}
	return (0);
}

/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////