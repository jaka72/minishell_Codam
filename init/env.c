#include "../minishell.h"

t_env	*get_env(char *envp[])
{
	int		i;
	t_env	*temp_env;
	t_env	*current_env;

	temp_env = NULL;
	current_env = NULL;
	i = 0;
	while (envp[i])
	{
		temp_env = init_tempenv();
		temp_env = get_name_value(temp_env, envp[i]);
		if (temp_env == NULL)
			err_free_env_exit("getting env failed\n");
		if (g_gl.start_env == NULL)
			g_gl.start_env = temp_env;
		else
		{
			current_env = last_env();
			current_env->next = temp_env;
		}
		i++;
	}
	return (g_gl.start_env);
}

// char	**ft_realloc_i(char **str, int i)
// {
// 	char	**temp;
// 	int		j;

// 	j = 0;
// 	temp = malloc(sizeof(char **) * i);
// 	if (temp == NULL)
// 		return (NULL);
// 	while (str[j] && j < i)
// 	{
// 		temp[j] = str[j];
// 		j++;
// 	}
// 	temp[i - 1] = NULL;
// 	return (temp);
// }

static char	**make_envstr(t_env *current, char **envs, int i)
{
	char	*temp;

	temp = malloc((ft_strlen(current->name) + ft_strlen(current->value) + 2));
	if (temp == NULL)
	{
		free_strings(envs);
		exit(errtx_all_free_exit(1, "malloc for envs failed\n"));
	}		
	ft_memcpy(temp, current->name, ft_strlen(current->name));
	ft_memcpy(&temp[ft_strlen(current->name)], "=", 1);
	ft_memcpy(&temp[ft_strlen(current->name) + 1],
		current->value, ft_strlen(current->value) + 1);
	envs[i] = temp;
	envs[i + 1] = NULL;
	return (envs);
}

char	**get_env_array(void)
{
	char	**envs;
	t_env	*current;
	int		i;

	i = 0;
	envs = malloc(sizeof(char **) * 1);
	if (envs == NULL)
		exit(errtx_all_free_exit(1, "malloc for envs failed\n"));
	envs[0] = NULL;
	current = g_gl.start_env;
	while (current)
	{
		envs = ft_add_str(envs);
		if (envs == NULL)
			return (NULL);
		envs = make_envstr(current, envs, i);
		current = current->next;
		i++;
	}
	return (envs);
}
