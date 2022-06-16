#include "../minishell.h"

// void	errtext_exit(char *text)
// {
// 	perror(text);
// 	exit(ERROR_RETURN);
// }

int	return_perr(int i, char *tx)
{
	write(2, "minishell: ", 11);
	write(2, tx, ft_strlen(tx));
	perror(" ");
	return (i);
}

int	return_errtx(int i, char *tx)
{
	write(2, "minishell: ", 11);
	write(2, tx, ft_strlen(tx));
	return (i);
}

void	free_envlist(void)
{
	t_env	*env;
	t_env	*temp_env;

	env = gl.start_env;
	while (env)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		temp_env = env;
		env = env->next;
		free(temp_env);
	}
	gl.start_env = NULL;
}

int	free_strings(char **strs)
{
	int	i;

	i = 0;
	if (strs == NULL)
		return (-1);
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
	return (0);
}

void	free_tcmd(void)
{
	t_cmd	*current;
	t_cmd	*temp;

	current = gl.start_cmd;
	while (current)
	{
		free_strings(current->args);
		free_strings(current->infile);
		free_strings(current->outfile);
		free_strings(current->heredoc);
		free_strings(current->files);
		temp = current;
		current = current->next;
		free(temp);
	}
	gl.start_cmd = NULL;
}

void	err_free_env_exit(char *text)
{
	perror(text);
	free_envlist();
	exit(ERROR_RETURN);
}

int	err_all_free_exit(int exitnum)
{
	free_tcmd();
	free_envlist();
	rl_clear_history();
	return (exitnum);
}

int	errtx_all_free_exit(int exitnum, char *tx)
{
	if (tx != NULL)
		perror(tx);
	return (err_all_free_exit(exitnum));
}
