#include "make_commands.h"


void print_command_info(t_cmd *cmd)
{
	int i = 0;
	printf(WHT2"  Command  array: "RES);
	while (i < count_args(cmd->args))
	{
		printf(DYEL" [%s%s%s] "RES, MAG, cmd->args[i], DYEL);
		i++;
	}
	printf(WHT2"\n"RES);

	printf(WHT2"  Command  fd_in: "DYEL"%2d\n"RES, cmd->fd_in);
	printf(WHT2"  Command fd_out: "DYEL"%2d\n\n"RES, cmd->fd_out);



	// printf(GRE"nr outfiles: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2"  Infile   array: ["RES);
	// while (i < cmd->count_infiles)
	while (i < count_elems(cmd->infile))
	{
		printf(DYEL" %s "RES, cmd->infile[i]);
		//printf(YEL" %i "RES, i);
		i++;
	}
	printf(WHT2"]\n"RES);




	// printf(GRE"nr_infiles: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2"  Outfile  array: ["RES);
	// while (i < cmd->count_outfiles)
	while (i < count_elems(cmd->outfile))
	{
		printf(DYEL" %s "RES, cmd->outfile[i]);
		//printf(YEL" %i "RES, i);
		i++;
	}
	printf(WHT2"]\n"RES);


	// printf(GRE"count_heredocs: %d \n"RES, cmd->count_outfiles);
	i = 0;
	printf(WHT2"  Heredocs array: ["RES);
	// while (i < cmd->count_heredocs)
	while (i < count_elems(cmd->heredoc))
	{
		printf(DYEL" %s "RES, cmd->heredoc[i]);
		i++;
	}
	printf(WHT2"]\n"RES);

//printf(WHT2"--------------------------------------------------------------------------------------\n\n"RES);


}

