/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_commands_list.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 14:40:36 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/07 12:14:24 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

void	free_arr_outfile_and_heredoc(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->outfile != NULL)
	{
		// while (i < cmd->count_outfiles)
		while (i < count_elems(cmd->outfile))
		{
			printf(LMAG"   Free %-10s %i [%s]\n", "outfile", i, cmd->outfile[i]);
			free(cmd->outfile[i]);
			i++;
		}
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	i = 0;
	if (cmd->heredoc != NULL)
	{
		// while (i < cmd->count_heredocs)
		while (i < count_elems(cmd->heredoc))
		{
			printf(MAG"   Free %-10s %i [%s]\n", "heredoc", i, cmd->heredoc[i]);
			//printf(YEL"Free heredoc %i\n", i);
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
		while (i < count_elems(cmd->args))
		{
			printf(LBLU"   Free %-10s %i [%s]\n", "args", i, cmd->args[i]);
			// printf(YEL"Free arg %i\n", i);
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	i = 0;
	if (cmd->infile != NULL)
	{
		// while (i < cmd->count_infiles)
		while (i < count_elems(cmd->infile))
		{
			// printf(YEL"Free infile %i\n", i);
			printf(BLU"   Free %-10s %i [%s]\n", "infile", i, cmd->infile[i]);
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
	int		i;		// JUST FOR PRINTING
	t_cmd	*temp;

	i = 0;
	temp = first_cmd;
	printf(RED"FREE ALL, first_cmd [%p]\n"RES, first_cmd);
	while (temp)
	{
		// THIS PRINT CAUSES SEGFAULT IF INPUT LINE IS SPACES ONLY
		//printf(RED"Free Command %d, temp->args[0]: [%s]\n"RES, i, temp->args[0]);

		free_arr_args_and_infile(temp);
		free_arr_outfile_and_heredoc(temp);
		//printf("    after free arr\n");
		temp = temp->next;
		free(first_cmd);

		//first_cmd = NULL; // NOT SURE IF NECESSARY ??

		
		//printf("    after free first_cmd\n");
		first_cmd = temp;
		i++;
	}
	return (0);
}

