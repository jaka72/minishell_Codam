#include "minishell.h"

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

// ----- to check heredoc -----
//
// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_infos	info;
// 	char	*tx;
// 	t_env	*env;

// 	(void) argc;
// 	(void) argv;
// 	tx = NULL;
// 	ms_init(&info, envp);
// 	env = info.start_env;
// 	// while (env)
// 	// {
// 	// 	printf("%s : %s \n", env->name, env->value);
// 	// 	env = env->next;
// 	// }
// 	printf("\'hello$PWD\' : %s\n", tx = check_expand(&info, "\'hello$PWD\'"));
// 	free (tx);
// 	printf("\"hello$PWD\" : %s\n", tx = check_expand(&info, "\"hello$PWD\""));
// 	free (tx);
// 	printf("hello$PWD : %s\n", tx = check_expand(&info, "hello$PWD"));
// 	free (tx);
// 	printf("hello $PWD : %s\n", tx = check_expand(&info, "hello $PWD"));
// 	free (tx);
// 	printf("$5 : %s\n", tx = check_expand(&info, "$5"));
// 	free (tx);
// 	printf("$a : %s\n", tx = check_expand(&info, "$a"));
// 	free (tx);
// 	printf("$pp : %s\n", tx = check_expand(&info, "$pp"));
// 	free (tx);
// 	printf("hello$PWD123 : %s\n", tx = check_expand(&info, "hello$PWD123"));
// 	free (tx);
// 	printf("\"hello$PWD $PWD\" : %s\n", tx = check_expand(&info, "\"hello$PWD $PWD\""));
// 	free (tx);
// 	free_envlist(&info);
// 	return (0);
// }