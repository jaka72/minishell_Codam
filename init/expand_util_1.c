#include "../minishell.h"
#define FLAG1 1

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
	if (newtx == NULL)
		free(tx);
	return (newtx);
}

char	*add_singlequote(char *dst, char *src)
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
	while (src[i] != '\0' && src[i] != '\'' && src[i] != '\"'
		&& src[i] != '$' && src[i] != ' ' && src[i] != '='
		&& src[i] != '/' && src[i] != ':')
		i++;
	return (i);
}
