# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

int	 main()
{
	char		*prompt;
	char		*line = NULL;
	// HIST_ENTRY	*history = NULL;

	prompt = getenv("PS1");
	printf("PS1 is %s\n", prompt);
	if (prompt == NULL)
		prompt = "minishell > ";
	// before while loop, get env
	while ((line = readline(prompt)))
	{
		printf("line is %s\n", line);
		add_history(line);
		// here parsing and make a linkedlist of t_cmd
		// after makint t_cmd list, fork and execute
		free(line);
	}
	rl_clear_history();
	return (0);
}
