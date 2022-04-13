#include "../minishell.h"

void	errtext_exit(char *text)
{
	perror(text);
	exit(ERROR_RETURN);
}

void	free_envlist(t_infos *infos)
{
	t_env	*env;
	t_env	*temp_env;

	env = infos->start_env;
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
	return (0);
}

void	free_tcmd(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*temp;

	current = cmds;
	while (current)
	{
		free_strings(current->args);
		free_strings(current->infile);
		free_strings(current->outfile);
		temp = current;
		current = current->next;
		free(temp);
	}
}

void	err_free_env_exit(t_infos *infos, char *text)
{
	perror(text);
	free_envlist(infos);
	exit(ERROR_RETURN);
}
