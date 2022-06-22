/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   choose_correct_array.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:15:19 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	is_heredoc(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->heredoc;
	cmd->fd_in = -3;
	src->currpos++;
}

static	void	is_files_in(t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->files;
	cmd->fd_in = -2;
}

static	void	is_files_out(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->files;
	cmd->fd_out = -2;
	if (src->inputline[src->currpos + 1] == '>')
	{
		src->currpos++;
		cmd->fd_out = -3;
	}
}

void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t)
{
	if (t->arrow == '<' && src->inputline[src->currpos + 1] != '<')
	{
		is_files_in(cmd, t);
	}
	else if (t->arrow == '<' && src->inputline[src->currpos + 1] == '<')
	{
		is_heredoc(src, cmd, t);
	}
	else if (t->arrow == '>')
	{
		is_files_out(src, cmd, t);
	}
	t->count = count_elems(t->temp_arr);
}
