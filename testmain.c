#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>


int	 main()
{
	char	*prompt;
	char	*line = NULL;
	int history_no = 0;
	HIST_ENTRY *history = NULL;

	prompt = getenv("PS1");
	printf("PS1 is %s\n", prompt);
	if (prompt == NULL)
		prompt = "minishell > ";

	while ((line = readline(prompt)))
	{
		add_history(line);

		// printf("\n");
		// printf("line is %s, now excute program\n", line);


		if (++history_no > 3)
		{
			history = remove_history(0);
			free(history);
  		}
		free(line);
	}
	
	rl_clear_history();
	// line = readline(prompt);
	printf(" last input = [ %s ]\n", line);
	// free(line);
	return (0);
}