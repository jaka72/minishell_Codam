#include "minishell.h"

int	g_status;

int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;
	t_cmd		*cmd_list;
	t_infos		info;
	char		*line;

	(void) argc;
	(void) argv;
	ms_init(&info, envp);
	line = readline(info.prompt);
	while (line)
	{
		if (ft_strlen(line) > 0)
		{
			if (check_syntax_errors(line, &src, &info) != 0)
				return (SYNTAX_ERROR);
			printf("test\n");	
			add_history(line);
			printf("src.corrpos is %ld, size is %ld, line is %s \n", src.currpos , src.inputline_size, src.inputline);		
			cmd_list = make_commands(&src);
			printf("test3\n");		
			g_status = run_cmd(&info, cmd_list);
			free_commands_list(cmd_list);	
		}
		free(line);
		line = readline(info.prompt);
	}
	cleandata(&info);
	// system("leaks minishell");
	return (0);
}
