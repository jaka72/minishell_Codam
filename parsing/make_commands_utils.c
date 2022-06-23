/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   make_commands_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 09:46:59 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define FLAG1 1

void	init_values(t_cmd *cmd)
{
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->args = NULL;
	cmd->heredoc = NULL;
	cmd->files = NULL;
}

int	check_if_builtin(t_cmd *cmd)
{
	if (cmd->args == NULL)
		return (0);
	if (cmd->args[0] == NULL)
		return (-1);
	if (ft_strcmp("echo", cmd->args[0]) == 0)
		return (1);
	else if (ft_strcmp("pwd", cmd->args[0]) == 0)
		return (1);
	else if (ft_strcmp("cd", cmd->args[0]) == 0)
		return (1);
	else if (ft_strcmp("export", cmd->args[0]) == 0)
		return (1);
	else if (ft_strcmp("unset", cmd->args[0]) == 0)
		return (1);
	else if (ft_strcmp("env", cmd->args[0]) == 0)
		return (1);
	else if (ft_strcmp("exit", cmd->args[0]) == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_cmd *list)
{
	if (cmd->args == NULL)
		return (1);
	if (ft_strcmp("echo", cmd->args[0]) == 0)
		return (run_echo_builtin(cmd));
	else if (ft_strcmp("pwd", cmd->args[0]) == 0)
		return (run_pwd_builtin());
	else if (ft_strcmp("cd", cmd->args[0]) == 0)
		return (run_cd_builtin(cmd));
	else if (ft_strcmp("exit", cmd->args[0]) == 0)
		return (run_exit_builtin(cmd, list));
	else if (ft_strcmp("export", cmd->args[0]) == 0)
		return (run_export_builtin(cmd));
	else if (ft_strcmp("unset", cmd->args[0]) == 0)
		return (run_unset_builtin(cmd));
	else if (ft_strcmp("env", cmd->args[0]) == 0)
		return (run_env_builtin());
	return (0);
}

static	void	set_flags(t_source *src, int *s_flag, int *d_flag, int *len)
{
	if (src->inputline[src->currpos] == '\'')
	{
		if (*d_flag == 0)
			*s_flag ^= FLAG1;
	}			
	if (src->inputline[src->currpos] == '\"')
	{
		if (*s_flag == 0)
			*d_flag ^= FLAG1;
	}		
	*len = *len + 1;
	src->currpos++;
}

int	get_length_of_word(t_source *src)
{
	int	len;
	int	s_flag;
	int	d_flag;

	len = 0;
	s_flag = 0;
	d_flag = 0;
	while (
		!(src->inputline[src->currpos] == '|' && s_flag + d_flag == 0)
		&& !(src->inputline[src->currpos] == '<' && s_flag + d_flag == 0)
		&& !(src->inputline[src->currpos] == '>' && s_flag + d_flag == 0)
		&& !(src->inputline[src->currpos] == '\0' && s_flag + d_flag == 0)
		&& !(ft_isspace(src->inputline[src->currpos]) && s_flag + d_flag == 0))
	{
		set_flags(src, &s_flag, &d_flag, &len);
	}
	src->currpos -= 1;
	return (len);
}
