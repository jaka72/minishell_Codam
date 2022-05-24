#include "minishell.h"

char	*free_and_read(char *line, t_infos *info)
{
	if (line != NULL)
		free(line);
	line = readline(info->prompt);
	return (line);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;
	t_cmd		*cmd_list;
	t_infos		info;
	char		*line;

	(void) argc;
	(void) argv;
	cmd_list = NULL;
	line = NULL;
	ms_init(&info, envp);
	line = free_and_read(line, &info);
	while (line)
	{
		if (ft_strlen(line) > 0)
		{
			if (check_syntax_errors(line, &src, &info) != 0)
				return (SYNTAX_ERROR);
			add_history(line);
			cmd_list = make_commands(&src);
			g_status = run_cmd(&info, cmd_list);
			free_commands_list(cmd_list);	
		}
		line = free_and_read(line, &info);
	}
	return (clean_data(&info, "exit\n"));
}
