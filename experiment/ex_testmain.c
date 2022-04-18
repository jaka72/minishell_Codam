#include "../minishell.h"

int	 main()
{
	char		*prompt;
	char		*line = NULL;
	HIST_ENTRY	*history = NULL;

	prompt = getenv("PS1");
	printf("PS1 is %s\n", prompt);
	if (prompt == NULL)
		prompt = "minishell > ";
	while ((line = readline(prompt)))
	{
		add_history(line);
		free(line);
	}
	rl_clear_history();
	printf(" last input = [ %s ]\n", line);
	free(line);
	return (0);
}
