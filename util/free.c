#include "../minishell.h"

void	free_envlist(void)
{
	t_env	*env;
	t_env	*temp_env;

	env = g_gl.start_env;
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
	g_gl.start_env = NULL;
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

	current = g_gl.start_cmd;
	while (current)
	{
		free_strings(current->args);
		free_strings(current->heredoc);
		free_strings(current->files);
		temp = current;
		current = current->next;
		free(temp);
	}
	g_gl.start_cmd = NULL;
}
