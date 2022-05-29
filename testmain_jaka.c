#include "minishell.h"



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
	ms_init(&info, envp);
	//line = readline(info.prompt);

	src.inputline = NULL;
	if (argc == 2)	// JUST FOR TESTING ////////////////////////
	{
		// printf(YEL"TESTER STARTED, argc=%d = [%s]\n"RES, argc, argv[1]);
		src.inputline = argv[1];
		src.inputline_size = strlen(src.inputline);
		//printf(CYN"line len: %ld\n"RES, src.inputline_size);
		if (check_syntax_errors(src.inputline, &src, &info) != 0)
			return (SYNTAX_ERROR);
		//line = src.inputline;	// maybe not needed
		cmd_list = make_commands(&src  /*, &info  */);

		//run_cd_builtin(cmd_list, &info);

		//exec_builtin(cmd_list, &info);		
		run_cmd(&info, cmd_list);
		//printf("From main: after run builtin\n");

		free_commands_list(cmd_list);
		//rl_clear_history();
		// free_envlist(&info);
		// tcsetattr(0, 0, &info.termios_save);
		clean_data(&info, NULL);
		printf(WHT"exit! (tester mode)\n"RES);
		return (0);
	}
	else
	{
		line = readline(info.prompt);
		while (line)
		{
			if (ft_strlen(line) > 0)
			{
				if (check_syntax_errors(line, &src, &info) != 0)
				{
					add_history(line);				// ADDED JAKA: INCASE OF ERROR MUST NOT EXIT, BUT LOOP AGAIN
					free(line);
					line = readline(info.prompt);
					//return (SYNTAX_ERROR);		// MUST SET ERROR: echo $? == 258
					continue ;
				}
				add_history(line);
				cmd_list = make_commands(&src);
				//printf(GRN"A)  main\n"RES);
				//exec_builtin(cmd_list, &info); // added jaka, for testing
				g_status = run_cmd(&info, cmd_list);
				free_commands_list(cmd_list);	
			}
			free(line);
			line = readline(info.prompt);
		}
	}

	// cleandata(&info);
	//system("leaks minishell");
	return (clean_data(g_status, &info, "exit\n"));
}
