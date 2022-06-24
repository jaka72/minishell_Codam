/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:12:19 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	count_commands(t_cmd *list)
{
	int		i;
	t_cmd	*temp;

	temp = list;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

static	int	check_if_numeric(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

static	int	print_msg_var_exit_numeric(char *str)
{
	write(2, "minishell: exit: ", 17);
	write(2, str, ft_strlen(str));
	write(2, ": numeric argument required\n", 28);
	return (1);
}

static	int	check_args_and_print(t_cmd *cmd, int nr_commands)
{
	int	exit_code;

	exit_code = ft_atoi_errcode(cmd->args[1]);
	if (check_if_numeric(cmd->args[1]) != 0 && count_elems(cmd->args) >= 2)
	{
		if (nr_commands == 1)
			write(2, "exit\n", 5);
		print_msg_var_exit_numeric(cmd->args[1]);
		exit(255);
	}
	else if (check_if_numeric(cmd->args[1]) == 0 && count_elems(cmd->args) > 2)
	{
		if (nr_commands == 1)
			write(2, "exit\n", 5);
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	else if (check_if_numeric(cmd->args[1]) == 0 && count_elems(cmd->args) == 2)
	{
		if (nr_commands == 1)
			write(2, "exit\n", 5);
		exit(exit_code);
	}
	return (0);
}
// int	check_args_and_print(t_cmd *cmd, int nr_commands)
// int	check_args_and_print(t_cmd *cmd, int nr_commands, t_infos *info)

int	run_exit_builtin(t_cmd *cmd, t_cmd *list)
{
	int	nr_commands;

	nr_commands = count_commands(list);
	if (count_elems(cmd->args) == 1)
	{
		if (nr_commands == 1)
			write(cmd->fd_out, "exit\n", 5);
		exit(0);
	}
	else if (count_elems(cmd->args) > 1)
	{
		if (check_args_and_print(cmd, nr_commands) != 0)
			return (1);
	}
	return (0);
}
// IT WILL NEED THE *list	STRUCT FOR COUNTING NR OF COMMANDS,
// UNLESS THERE IS ANOTHER WAY OF DOING IT
// nr_commands checks if there are pipes or just single command
// int	run_exit_builtin(t_cmd *cmd, t_cmd *list)
