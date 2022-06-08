/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmurovec <jmurovec@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 12:56:23 by jmurovec      #+#    #+#                 */
/*   Updated: 2022/06/08 10:12:24 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	count_commands(t_cmd *list)
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

int	check_if_numeric(char *arg)
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

// int	check_args_and_print(t_cmd *cmd, int nr_commands)
// int	check_args_and_print(t_cmd *cmd, int nr_commands, t_infos *info)
int	check_args_and_print(t_cmd *cmd, int nr_commands, t_cmd *list)
{
	int	exit_code;	// TO STORE, OTHERWISE IT READS cmd->args AFTER FREEING

	exit_code = ft_atoi(cmd->args[1]);
	if (check_if_numeric(cmd->args[1]) != 0 && count_elems(cmd->args) >= 2)
	{
		if (nr_commands == 1)
			write(2, "exit\n", 5);
		write(2, "minishell: exit: ", 17);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2, ": numeric argument required\n", 28);

		free_commands_list(list);	// THIS OK, , only 1 block reachable, add_history
		clean_data(gl.g_status, "");

		exit (255);
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
		free_commands_list(list);	// THIS IS CAUSING SEGFAULT IN IF READLINE DISABLED "exit 123"
		clean_data(gl.g_status, "");
		exit (exit_code);
	}
	return (0);
}

// IT WILL NEED THE *list	STRUCT FOR COUNTING NR OF COMMANDS,
// UNLESS THERE IS ANOTHER WAY OF DOING IT
// nr_commands checks if there are pipes or just single command
// int	run_exit_builtin(t_cmd *cmd, t_cmd *list)
int	run_exit_builtin(t_cmd *cmd, t_cmd *list)
{
	int	nr_commands;

	nr_commands = count_commands(list);
	if (count_elems(cmd->args) == 1)
	{
		if (nr_commands == 1)
			write(cmd->fd_out, "exit\n", 5);
		free_commands_list(list);		// THIS OK, REMOVES REACHABLE, IF READLINE DISABLED, CHECK ALSO WITH READLINE
	
		clean_data(gl.g_status, "");  // new, IS THIS NECESSARY HERE?? add_history() STILL SHOWS MALLOCED
		exit (0);							// SEEMS NO DIFFERENCE IN NONTESTER MODE
	}										// STILL SHOWS READLINE REACHABLE BYTES
	else if (count_elems(cmd->args) > 1)
	{
		// if (check_args_and_print(cmd, nr_commands) != 0)
		// if (check_args_and_print(cmd, nr_commands, info) != 0)
		if (check_args_and_print(cmd, nr_commands, list) != 0)
			return (1);
	}
	return (0);
}
