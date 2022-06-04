/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   choose_correct_array.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 14:07:33 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/03 17:46:23 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

void	is_infile(t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->infile;
	cmd->count_infiles++;
	// t->count = cmd->count_infiles;
	t->count = count_infiles(cmd->infile);
	
	printf("  counted infiles: %d\n", t->count);
	
	cmd->fd_in = -2;
}

void	is_heredoc(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->heredoc;
	cmd->count_heredocs++;
	t->count = cmd->count_heredocs;
	cmd->fd_in = -3;
	src->currpos++;
}

void	is_outfile(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->outfile;
	cmd->count_outfiles++;
	t->count = cmd->count_outfiles;
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
		is_infile(cmd, t);
	}
	else if (t->arrow == '<' && src->inputline[src->currpos + 1] == '<')
	{
		is_heredoc(src, cmd, t);
	}
	else if (t->arrow == '>')
	{
		is_outfile(src, cmd, t);
	}
}
