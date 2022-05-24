#include "make_commands.h"


void print_command_info(t_cmd *cmd)
{
	int i = 0;
	printf(WHT2"  Command  array: ["RES);
	if (cmd->args != NULL)
	{
		while (i < cmd->count_args)
		{
			printf(DYEL" %s "RES, cmd->args[i]);
			i++;
		}
	}

	printf(WHT2"]\n"RES);

	printf(WHT2"  Command  fd_in: "DYEL"%2d\n"RES, cmd->fd_in);
	printf(WHT2"  Command fd_out: "DYEL"%2d\n\n"RES, cmd->fd_out);



	// printf(GRE"nr outfiles: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2"  Infile   array: ["RES);
	while (i < cmd->count_infiles)
	{
		printf(DYEL" %s "RES, cmd->infile[i]);
		//printf(YEL" %i "RES, i);
		i++;
	}
	printf(WHT2"]\n"RES);




	// printf(GRE"nr_infiles: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2"  Outfile  array: ["RES);
	while (i < cmd->count_outfiles)
	{
		printf(DYEL" %s "RES, cmd->outfile[i]);
		//printf(YEL" %i "RES, i);
		i++;
	}
	printf(WHT2"]!\n"RES);


	// printf(GRE"count_heredocs: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2"  Heredocs array: ["RES);
	printf("cmd->count_heredoc is %d\n", cmd->count_heredocs);
	while (i < cmd->count_heredocs)
	{
		printf(DYEL" %s "RES, cmd->heredoc[i]);
		i++;
	}
	printf(WHT2"]\n"RES);

//printf(WHT2"--------------------------------------------------------------------------------------\n\n"RES);


}

