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
		src.inputline = line;
		src.inputline_size = strlen(src.inputline);
		if (ft_strlen(line) > 0)
		{
			if (check_syntax_errors(&src, &info) != 0)
				return (SYNTAX_ERROR);
			add_history(line);
			cmd_list = make_commands(&src);
			g_status = run_cmd(&info, cmd_list);
			free_commands_list(cmd_list);	
		}
		free(line);
		line = readline(info.prompt);
	}
	cleandata(&info);
	system("leaks minishell");
	return (0);
}
