#include "builtins.h"

int	count_commands(t_cmd *cmd)
{
	int	i;
	t_cmd *temp;

	temp = cmd;
	i = 0;
	while (temp)
	{
		printf("temp->args[0]: [%s]\n", temp->args[0]);
		temp = temp->next;
		i++;
	}
	printf(YEL"Number of all commands: %d\n"RES, i);
	return (i);
}

/*
exit 123 456	writes exit, but does not exit:   	"too many arguments"
exit 123 qwe	writes exit, but does not exit:   	"too many arguments"
exit asd qwe	writes exit, it exits:				"numeric argument required"

exit					it only exits if only 1 command
ls | exit				does not exit
exit | ls				does not exit
ls | wc | exit			does not exit

*/


int check_numeric(char *arg)
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

int	run_exit_builtin(t_cmd *cmd /*, t_infos *info */)
{
	int	ret;
	printf(GRN"Start run_exit_builtin\n"RES);
	//printf(GRN"Print temp for testing: %s %d\n"RES, cmd->args[0], info->ini_fd[0]);

	ret = count_commands(cmd);

	if (cmd->count_args > 2)
	{
		if (check_numeric(cmd->args[1]) != 0)
		{
			write(2, "minishell: exit: numeric argument required\n", 43);
			return (2);
		}
		//printf(GRN"minishell: exit: too many arguments\n"GRN);
		write(2, "minishell: exit: too many arguments\n", 37);
		return (2);
	}
	else if (cmd->count_args == 1)
	{
		printf(RED"exit\n"RES);
		return (2);
	}
	return (0);
}
