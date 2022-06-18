#include "../minishell.h"
#define FLAG1 1

void	print_env(void) 
{
	t_env	*env;

	env = g_gl.start_env;
	while (env->next)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	printf("%s=%s\n", env->name, env->value);
}

char	*name_expand(char *tx)
{
	char	*newtx;
	t_env	*env;

	newtx = NULL;
	env = g_gl.start_env;
	while (env)
	{
		if (ft_strncmp(tx, env->name, ft_strlen(tx) + 1) == 0
			&& ft_strlen(tx) == ft_strlen(env->name))
		{
			// printf("tx is %s found %s %s!!\n", tx, env->name, env->value);	
			newtx = malloc(ft_strlen(env->value) + 1);
			if (newtx == NULL)
				exit(errtx_all_free_exit(1, "check expand malloc failed\n"));
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
	while (src[i] != '\0' && src[i] != '\'' && src[i] != '\"' && src[i] != '$' && src[i] != ' ' && src[i] != '=' && src[i] != '/' && src[i] != ':')
		i++;
	return (i);
}

int	count_expand_length_hd(char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && src[i] != ' ')
		i++;
	return (i);
}

char	*add_expanded(char *dst, char *src)
{
	int	i;
	char	*temp;
	char	*expanded;

	i = 0;
	temp = NULL;
	expanded = NULL;
	while (src[i] != '\0' && src[i] != '\'' && src[i] != '\"' && src[i] != '$' && src[i] != ' ' && src[i] != '=' && src[i] != '/' && src[i] != ':')
		i++;
	temp = malloc(i + 1);
	if (temp == NULL)
		exit(errtx_all_free_exit(1, "add expand malloc failed\n"));
	ft_strlcpy(temp, src, i + 1);
	expanded = name_expand(temp);
	dst = ft_strjoin_free(dst, expanded);
	if (dst == NULL)
		exit(errtx_all_free_exit(1, "add expand malloc failed\n"));
	return (dst);
}


char	*add_laststatus(char *dst, int g_status)
{
	char	*stat;

	stat = NULL;
	stat = ft_itoa(g_status);
	if (stat == NULL)
		exit(errtx_all_free_exit(1, "last status malloc failed\n"));
	dst = ft_strjoin(dst, stat);
	if (dst == NULL)
		exit(errtx_all_free_exit(1, "last status malloc failed\n"));
	return (dst);
}

char	*check_expand_hd(char *tx)
{
	int		i;
	char	*expanded;

	i = 0;
	if (tx[0] == '$' && tx[1] == '\0')
		return (tx);
	expanded = malloc(1);
	if (expanded == NULL)
		err_free_env_exit("check expand malloc failed");
	expanded[0] = '\0';
	while (tx[i] != '\0')
	{
		if (tx[i] == '$' && tx[i + 1] == '?' )
		{
			expanded = ft_add_c_free(expanded, '0');
			i++;
		}
		else if (tx[i] == '$')
		{
			expanded = add_expanded(expanded, &tx[i + 1]);
			// write(2, expanded, ft_strlen(expanded));
			// write(2, "!", 1);
			i = i + count_expand_length(&tx[i + 1]);
		}
		else
			expanded = ft_add_c_free(expanded, tx[i]);
		i++;
	}
	free(tx);
	return (expanded);
}

char	*check_expand_file(char *tx)
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
		err_free_env_exit("check expand malloc failed");
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
			expanded = add_laststatus(expanded, g_gl.g_status);
			i++;
		}
		else if (tx[i] == '$')
		{
			expanded = add_expanded(expanded, &tx[i + 1]);
			i = i + count_expand_length(&tx[i + 1]);
		}
		else
			expanded = ft_add_c_free(expanded, tx[i]);
		i++;
	}
	if ((tx[0] != '\'' && tx[0] != '\"') && expanded[0] == '\0')
	{
		free(expanded);
		return (NULL);
	}
	return (expanded);
}

char	*check_expand(char *tx)
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
		err_free_env_exit("check expand malloc failed");
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
			expanded = add_laststatus(expanded, g_gl.g_status);
			i++;
		}
		else if (tx[i] == '$')
		{
			expanded = add_expanded(expanded, &tx[i + 1]);
			i = i + count_expand_length(&tx[i + 1]);
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

char	**expand_array(char **args)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;
	if (args == NULL)
		return (args);
	while (args[i])
	{
		args[i] = check_expand(args[i]);
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
