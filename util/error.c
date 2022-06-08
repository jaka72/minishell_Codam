#include "../minishell.h"

void	errtext_exit(char *text)
{
	perror(text);
	exit(ERROR_RETURN);
}

void	free_envlist(void)
{
	t_env	*env;
	t_env	*temp_env;

	env = gl.start_env;
	while (env->next)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		temp_env = env;
		env = env->next;
		free(temp_env);
	}
	if (env->name)
		free(env->name);
	if (env->value)
		free(env->value);
	free(env);
}

int	free_strings(char **strs)
{
	int	i;

	i = 0;
	if (strs == NULL)
		return (0);
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (0);
}

void	free_tcmd(t_cmd *st_cmd)
{
	t_cmd	*current;
	t_cmd	*temp;

	current = st_cmd;
	while (current)
	{
		free_strings(current->args);
		free_strings(current->infile);
		free_strings(current->outfile);
		free_strings(current->heredoc);
		temp = current;
		current = current->next;
		free(temp);
	}
}

void	err_free_env_exit(char *text)
{
	perror(text);
	free_envlist();
	exit(ERROR_RETURN);
}

int	err_all_free_exit(t_cmd *st_cmd, int exitnum)
{
	if (st_cmd != NULL)
		free_tcmd(st_cmd);
	clean_data(gl.g_status, NULL);
	return (exitnum);
}
