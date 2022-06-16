/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   choose_correct_array.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 14:07:33 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/16 09:18:12 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

void	is_infile(t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->infile;
	//cmd->count_infiles++;
	// t->count = cmd->count_infiles;
	//t->count = count_infiles(cmd->infile);
	
	//printf("    is_infile: t.count == nr_args == %d\n", t->count);
	
	cmd->fd_in = -2;
}

void	is_heredoc(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->heredoc;
	// cmd->count_heredocs++;
	// t->count = cmd->count_heredocs;
	cmd->fd_in = -3;
	src->currpos++;
}

void	is_outfile(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->outfile;
	//cmd->count_outfiles++;
	//t->count = cmd->count_outfiles;
	cmd->fd_out = -2;
	if (src->inputline[src->currpos + 1] == '>')
	{
		src->currpos++;
		cmd->fd_out = -3;
	}
}

void	is_files_in(t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->files;
	//cmd->count_outfiles++;
	//t->count = cmd->count_outfiles;
	cmd->fd_in = -2;
}

void	is_files_out(t_source *src, t_cmd *cmd, t_tmp *t)
{
	t->temp_arr = cmd->files;
	//cmd->count_outfiles++;
	//t->count = cmd->count_outfiles;
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
	// t->count = count_elems(cmd->outfile);
	t->count = count_elems(t->temp_arr);
	//printf(WHT"       t.count: %d\n"RES, t->count);
}

// void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t)
// {
// 	if (t->arrow == '<' && src->inputline[src->currpos + 1] != '<')
// 	{
// 		is_infile(cmd, t);
// 	}
// 	else if (t->arrow == '<' && src->inputline[src->currpos + 1] == '<')
// 	{
// 		is_heredoc(src, cmd, t);
// 	}
// 	else if (t->arrow == '>')
// 	{
// 		is_outfile(src, cmd, t);
// 	}
// 	// t->count = count_elems(cmd->outfile);
// 	t->count = count_elems(t->temp_arr);
// 	//printf(WHT"       t.count: %d\n"RES, t->count);
// }
