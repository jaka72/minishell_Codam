#include "minishell.h"

/*
NOTES, ISSUES


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
	char		*line;

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
				add_history(line);
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
