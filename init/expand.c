#include "../minishell.h"

void	print_env(t_infos *info) 
{
	t_env	*env;

	env = info->start_env;
	while (env->next)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	printf("%s=%s\n", env->name, env->value);
}
// function to print env (not necessary)

char	*name_expand(t_infos *info, char *tx)
{
	char	*newtx;
	t_env	*env;

	newtx = NULL;
	env = info->start_env;
	while (env)
	{
		if (ft_strncmp(tx, env->name, ft_strlen(tx + 1)) == 0
			&& ft_strlen(tx) == ft_strlen(env->name))
		{
			newtx = malloc(ft_strlen(env->value) + 1);
			if (newtx == NULL)
				errtext_exit("check expand malloc failed");
			newtx = ft_memcpy(newtx, env->value, ft_strlen(env->value));
			newtx[ft_strlen(env->value)] = '\0';
			free(tx);
			return (newtx);
		}
		env = env->next;
	}
	return (newtx);
}

char	*check_expand(t_infos *info, char *tx)
{
	int		i;
	char	**temp;
	char	*expanded;

	i = 0;
	if (ft_strchr(tx, '$') == NULL || (tx[0] == '$' && tx[1] == '\0'))
		return (tx);
	expanded = malloc(1);
	if (expanded == NULL)
		err_free_env_exit(info, "check expand malloc failed");
	expanded[0] = '\0';
	temp = ft_split(tx, '$');
	if (temp == NULL)
		err_free_env_exit(info, "check expand split failed");
	free(tx);
	while (temp[i] != NULL)
	{
		temp[i] = name_expand(info, temp[i]);
		i++;
	}
	i = 0;
	while (temp[i] != NULL)
	{
		expanded = ft_strjoin_free(expanded, temp[i]);
		if (expanded == NULL)
			err_free_env_exit(info, "connect expanded text failed");
		i++;
	}
	free(temp);
	return (expanded);
}
