#include "minishell.h"

/*
// NOTES, ISSUES ////////////////////////////////////////////////////

***************************************************************************
*** A temp Line to see if this will dissapear after merging from Kitos'main
*** Because I merged my branch to main, then I started changing this branch
*** Then Kito merged her branch to Main.
*** So now some files on main are older than the ones on my branch.
*** If I now pull from main, will my new files be overwritten by my old files?



Cases like:  < asdqwe   or    < nopermissionfile
	It should not exit, just print error
	I added a similar function like errtext_exit. But it probably needs to 
		return to all the previous calls.


*/

void	free_and_read(t_source *src, t_infos *info, int history)
{
	if (history == 1)
		add_history(src->inputline);	
	if (src->inputline != NULL)
		free(src->inputline);
	src->inputline = readline(info->prompt);
}



int	g_status;
// ORIGINAL FROM testmain.c
int	main(int argc, char *argv[], char *envp[])
{
	printf("\n");
	t_source	src;
	t_cmd		*cmd_list;
	t_infos		info;
	//char		*line;

	(void) argc;
	(void) argv;
	// src.inputline = NULL;
	ms_init(&info, envp);
	//free_and_read(&src, &info, 0);

	//line = readline(info.prompt);

	if (argc == 2)	// JUST FOR TESTING ////////////////////////
	{
		//printf(GRN"tester mode:\n"RES);
		src.inputline = argv[1];
		src.inputline_size = strlen(src.inputline);
		if (check_syntax_errors(&src) != 0)
			return (SYNTAX_ERROR);
		cmd_list = make_commands(&src  /*, &info  */);
		g_status = run_cmd(&info, cmd_list);
		free_commands_list(cmd_list);
		clean_data(g_status, &info, NULL);
		printf(WHT"exit! (tester mode)\n"RES);
		return (0);
	}
	else
	{
		printf(GRN"Real mode:\n"RES);
		//line = readline(info.prompt);
		src.inputline = NULL;
		free_and_read(&src, &info, 0);
		while (src.inputline)
		{
			if (ft_strlen(src.inputline) > 0)
			{
				if (check_syntax_errors(&src) != 0)
				{
					//add_history(line);				// ADDED JAKA: INCASE OF ERROR MUST NOT EXIT, BUT LOOP AGAIN
					//free(line);
					//line = readline(info.prompt);
					free_and_read(&src, &info, 1);
					continue ;
				}
				add_history(src.inputline);
				cmd_list = make_commands(&src);
				g_status = run_cmd(&info, cmd_list);
				free_commands_list(cmd_list);	
			}
			free_and_read(&src, &info, 0);
			//free(line);
			//line = readline(info.prompt);
		}
	}

	//system("leaks minishell");
	return (clean_data(g_status, &info, "exit\n"));
}
