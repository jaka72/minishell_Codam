#include "minishell.h"

void	free_and_read(t_source *src, t_infos *info, int history)
{
	if (history == 1)
		add_history(src->inputline);	
	if (src->inputline != NULL)
		free(src->inputline);
	src->inputline = readline(info->prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;
	t_cmd		*cmd_list;
	t_infos		info;

	(void) argc;
	(void) argv;
	src.inputline = NULL;
	ms_init(&info, envp);
	free_and_read(&src, &info, 0);
	//printf(GRN"main)  cmd->args[0]: [%s]\n"RES, cmd_list->args);
	while (src.inputline)
	{
		if (ft_strlen(src.inputline) > 0)
		{
			if (check_syntax_errors(&src) != 0)
			{
				free_and_read(&src, &info, 1);
				continue ;
			}
			add_history(src.inputline);
			cmd_list = make_commands(&src);
			//printf(GRN"B)\n"RES);
			g_status = run_cmd(&info, cmd_list);
			free_commands_list(cmd_list);
		}
		free_and_read(&src, &info, 0);
	}
	return (clean_data(g_status, &info, "exit\n"));
}
