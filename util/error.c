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

void	err_free_env_exit(t_infos *infos, char *text)
{
	perror(text);
	free_envlist(infos);
	exit(ERROR_RETURN);
}
