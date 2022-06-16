#include "minishell.h"

void	free_and_read(t_source *src, int history)
{
	if (history == 1)
		add_history(src->inputline);	
	if (src->inputline != NULL)
		free(src->inputline);
	src->inputline = readline(gl.prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;
	int	i = 0;

	(void) argc;
	(void) argv;
	src.inputline = NULL;
	ms_init(envp);
	free_and_read(&src, 0);
	while (src.inputline)
	{
		if (ft_strlen(src.inputline) > 0)
		{
			if (check_syntax_errors(&src) != 0)
			{
				free_and_read(&src, 1);
				continue ;
			}
			add_history(src.inputline);
			gl.start_cmd = make_commands(&src);
			i = 0;
			while(gl.start_cmd->files && gl.start_cmd->files[i])
			{
				printf("%s \n", gl.start_cmd->files[i]);
				i++;
			}
			i = 0;
			while(gl.start_cmd->heredoc && gl.start_cmd->heredoc[i])
			{
				printf("%s \n", gl.start_cmd->heredoc[i]);
				i++;
			}			
			gl.g_status = run_cmd();
			free_tcmd();
		}
		free_and_read(&src, 0);
	}
	return (clean_data(gl.g_status, "exit\n"));
}
