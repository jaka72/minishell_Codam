#include "../minishell.h"

int	find_name_delate(char *targetname)
{
	t_env	*env;
	t_env	*prev;

	env = g_gl.start_env;
	prev = NULL;
	while (env)
	{
		if (ft_strncmp(targetname, env->name, ft_strlen(targetname) + 1) == 0
			&& ft_strlen(targetname) == ft_strlen(env->name))
		{
			if (prev == NULL)
				g_gl.start_env = env->next;
			else
				prev->next = env->next;
			free(env);
			break ;
		}
		prev = env;
		env = env->next;
	}
	return (0);
}

int	run_unset_builtin(t_cmd *cmd)
{
	int	i;

	i = 1;
	g_gl.g_status = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] >= '0' && cmd->args[i][0] <= '9')
		{
			write(2, "minishell: unset: ", 18);
			write(2, cmd->args[i],  ft_strlen(cmd->args[i]));
			write(2, ": not a valid identifier\n", 25);
			g_gl.g_status = 1;
		}
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			write(2, "minishell: unset: ", 18);
			write(2, cmd->args[i],  ft_strlen(cmd->args[i]));
			write(2, ": not a valid identifier\n", 25);
			g_gl.g_status = 1;
		}
		find_name_delate(cmd->args[i]);
		i++;
	}
	return (g_gl.g_status);
}
