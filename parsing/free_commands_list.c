#include "make_commands.h"


void free_arrays(t_cmd *cmd)
{
	int i;
	
	i = 0;
	printf(WHT2" Free command array\n"RES);
	if (cmd->args != NULL)
	{
		while (i < cmd->count_args)
		{
			printf(DYEL"    Free command[%d]\n"RES, i);
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		cmd->args = NULL;
	}


	// printf(GRE"nr outfiles: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2" Free infile array\n"RES);
	if (cmd->infile != NULL)
	{
		while (i < cmd->count_infiles)
		{
			printf(DYEL"    Free infile[%d]\n"RES, i);
			free(cmd->infile[i]);
			i++;
		}
		free(cmd->infile);
		cmd->infile = NULL;
	}




	// printf(GRE"nr_infiles: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2" Free outfile array\n"RES);
	if (cmd->outfile != NULL)
	{
		while (i < cmd->count_outfiles)
		{
			printf(DYEL"    Free outfile[%d]\n"RES, i);
			free(cmd->outfile[i]);
			i++;
		}
		free(cmd->outfile);
		cmd->outfile = NULL;
	}


	// printf(GRE"count_heredocs: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2" Free heredocs array\n"RES);
	if (cmd->heredoc != NULL)
	{
		while (i < cmd->count_heredocs)
		{
			printf(DYEL"    Free heredoc[%d]\n"RES, i);
			free(cmd->heredoc[i]);
			i++;
		}
		free(cmd->heredoc);
		cmd->heredoc = NULL;
	}
}

int	free_commands_list(t_cmd *first_cmd)
{
	int	i;	// just for printing
	t_cmd *temp;

	i = 0;
	temp = first_cmd;
	while (temp)
	{
		printf(YEL"Free struct[%d]\n"RES, i);
		free_arrays(temp);
		temp = temp->next;
		free(first_cmd);
		first_cmd = temp;
		i++;
	}
	return (0);
}
