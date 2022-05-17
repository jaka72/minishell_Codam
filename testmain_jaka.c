#include "minishell.h"


int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;		// jaka
	t_cmd		*cmd_list;	// jaka

	t_infos	info;
	char	*line;

	(void) argc;
	(void) argv;
	ms_init(&info, envp);


	src.inputline = NULL;
	if (argc == 2)	// JUST FOR TESTING ////////////////////////
	{
		src.inputline = argv[1];
		src.inputline_size = strlen(src.inputline);
		//printf(CYN"line len: %ld\n"RES, src.inputline_size);
		if (check_syntax_errors(&src, &info) != 0)
				return (SYNTAX_ERROR);
		//line = src.inputline;	// maybe not needed
		cmd_list = make_commands(&src  /*, &info  */);

		//run_cd_builtin(cmd_list, &info);
				
		run_cmd(&info, cmd_list);
		//printf("From main: after run builtin\n");

		free_commands_list(cmd_list);
		//rl_clear_history();
		free_envlist(&info);
		tcsetattr(0, 0, &info.termios_save);
		printf(WHT"exit! (tester mode)\n"RES);
		return (0);
	}
	else
	{
		line = readline(info.prompt);
		//printf(RED"Line: [%s]\n", line);
		// if (line)
		// {
		// 	src.inputline = line;	// Can be moved into the while (line) ...
		// 	src.inputline_size = strlen(src.inputline);
		// 	//if (src.inputline[0] == '\0')
		// 	//	return (0);
		// }
		while (line)
		{
			// printf(RED"main a)\n"RES);
			src.inputline = line;
			src.inputline_size = strlen(src.inputline);
			// if (ft_strlen(line) > 0)
			if (ft_strlen(line) > 0)
			{
				if (check_syntax_errors(&src, &info) == 0)
				{
					cmd_list = make_commands(&src /*, &info */);
					run_cmd(&info, cmd_list);
					// FREE AFTER EXECUTION /////////////////////////////////////////////////////
					free_commands_list(cmd_list);
				}
				add_history(line);
				free(line);
			}
			line = readline(info.prompt);
			// if (line)
			// {
			// 	src.inputline = line;
			// 	src.inputline_size = strlen(src.inputline);
			// 	// if (src.inputline[0] == '\0')
			// 	// 	return (0);
			// }
		}
	}
	// printf("exit!\n");
	// rl_clear_history();
	// free_envlist(&info);
	// tcsetattr(0, 0, &info.termios_save);
	cleandata(&info);
	return (0);
}