#include "../minishell.h"
#define FLAG1 1

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

char	*name_expand(t_infos *info, char *tx)
{
	char	*newtx;
	t_env	*env;

	newtx = NULL;
	env = info->start_env;
	while (env)
	{
		if (ft_strncmp(tx, env->name, ft_strlen(tx) + 1) == 0
			&& ft_strlen(tx) == ft_strlen(env->name))
		{
			// printf("tx is %s found %s %s!!\n", tx, env->name, env->value);	
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

char *add_singlequote(char *dst, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && src[i] != '\'')
	{
		dst = ft_add_c_free(dst, src[i]);
		i++;
	}
	i++;
	return (dst);
}

int	count_single_length(char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\'' && src[i] != '\0')
		i++;
	if (src[i] == '\'')
		i++;
	return (i);
}

int	count_expand_length(char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && src[i] != '\'' && src[i] != '\"' && src[i] != '$' && src[i] != ' ' && src[i] != '=' && src[i] != '/')
		i++;
	return (i);
}

char	*add_expanded(char *dst, char *src, t_infos *info)
{
	int	i;
	char	*temp;
	char	*expanded;

	i = 0;
	temp = NULL;
	expanded = NULL;
	while (src[i] != '\0' && src[i] != '\'' && src[i] != '\"' && src[i] != '$' && src[i] != ' ' && src[i] != '=' && src[i] != '/')
		i++;
	temp = malloc(i + 1);
	if (temp == NULL)
		errtext_exit("malloc failed");
	ft_strlcpy(temp, src, i + 1);
	expanded = name_expand(info, temp);
	dst = ft_strjoin_free(dst, expanded);
	if (dst == NULL)
		errtext_exit("malloc failed");
	return (dst);
}

char	*add_laststatus(char *dst, int g_status)
{
	char	*stat;

	stat = NULL;
	stat = ft_itoa(g_status);
	if (stat == NULL)
		errtext_exit("malloc failed");
	dst = ft_strjoin(dst, stat);
	if (dst == NULL)
		errtext_exit("malloc failed");
	return (dst);
}

char	*check_expand(t_infos *info, char *tx)
{
	int		i;
	int		d_flag;
	char	*expanded;

	i = 0;
	d_flag = 0;
	if (tx[0] == '$' && tx[1] == '\0')
		return (tx);
	expanded = malloc(1);
	if (expanded == NULL)
		err_free_env_exit(info, "check expand malloc failed");
	expanded[0] = '\0';
	while (tx[i] != '\0')
	{
		if (tx[i] == '\"')
			d_flag ^= FLAG1;
		else if (tx[i] == '\'' && (d_flag & FLAG1) > 0)
			expanded = ft_add_c_free(expanded, '\'');
		else if (tx[i] == '\'' && tx[i + 1] != '\0')
		{
			expanded = add_singlequote(expanded, &tx[i + 1]);
			i = i + count_single_length(&tx[i + 1]);
		}
		// else if (tx[i] == '$' && tx[i + 1] == '\0')
		// 	return (tx);
		else if (tx[i] == '$' && tx[i + 1] == '\"' && d_flag == 1)
		{
			expanded = ft_add_c_free(expanded, '$');
			d_flag ^= FLAG1;
			i++;
		}
		else if ((tx[i] == '$' && tx[i + 1] == ' ') || (tx[i] == '$' && tx[i + 1] == '\0'))
			expanded = ft_add_c_free(expanded, '$');
		else if (tx[i] == '$' && tx[i + 1] == '?' )
		{
			// printf("now $? is called, g_status is %d\n", g_status);
			expanded = add_laststatus(expanded, g_status);
			printf("expanded is %s\n", expanded);
			i++;
		}
		else if (tx[i] == '$')
		{
			expanded = add_expanded(expanded, &tx[i + 1], info);
			i = i + count_expand_length(&tx[i + 1]);
			// printf("tx[i] is %c\n", tx[i]);
		}
		else
			expanded = ft_add_c_free(expanded, tx[i]);
		i++;
	}
	if ((tx[0] != '\'' && tx[0] != '\"') && expanded[0] == '\0')
	{
		free(expanded);
		free(tx);
		return (NULL);
	}
	free(tx);
	return (expanded);
}

char	**expand_array(char **args, t_infos *info)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;
	if (args == NULL)
		return (args);
	while (args[i])
	{
		args[i] = check_expand(info, args[i]);
		if (args[i] == NULL)
		{
			temp = i;
			while (args[temp + 1])
			{
				args[temp] =  args[temp + 1];
				temp++;
			}
			args[temp] =  NULL;
			i--;
		}
		i++;
	}
	return (args);
}
