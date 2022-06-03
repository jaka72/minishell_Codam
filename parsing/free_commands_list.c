/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_commands_list.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 14:40:36 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/03 15:42:03 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

void	free_arr_outfile_and_heredoc(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->outfile != NULL)
	{
		while (i < cmd->count_outfiles)
		{
			free(cmd->outfile[i]);
			i++;
		}
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	i = 0;
	if (cmd->heredoc != NULL)
	{
		while (i < cmd->count_heredocs)
		{
			free(cmd->heredoc[i]);
			i++;
		}
		free(cmd->heredoc);
		cmd->heredoc = NULL;
	}
}

void	free_arr_args_and_infile(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->args != NULL)
	{
		while (i < count_args(cmd))
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	i = 0;
	if (cmd->infile != NULL)
	{
		while (i < cmd->count_infiles)
		{
			free(cmd->infile[i]);
			i++;
		}
		free(cmd->infile);
		cmd->infile = NULL;
	}
}

// REMOVE i, JUST FOR PRINTING
int	free_commands_list(t_cmd *first_cmd)
{
	int		i;
	t_cmd	*temp;

	i = 0;
	temp = first_cmd;
	while (temp)
	{
		//printf("    i:%d\n", i);
		
		free_arr_args_and_infile(temp);
		//printf("    after free arr\n");
		temp = temp->next;
		free(first_cmd);
		//printf("    after free first_cmd\n");
		first_cmd = temp;
		i++;
	}
	return (0);
}
