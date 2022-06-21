#include "../minishell.h"
#define FLAG1 1

char	*add_expanded(char *dst, char *src)
{
	int		i;
	char	*temp;
	char	*expanded;

	i = 0;
	temp = NULL;
	expanded = NULL;
	while (src[i] != '\0' && src[i] != '\'' && src[i] != '\"' && src[i] != '$'
		&& src[i] != ' ' && src[i] != '=' && src[i] != '/' && src[i] != ':')
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

char	*add_two_cha(char *dst, char a, char b, int *i)
{
	dst = ft_add_c_free(dst, a);
	dst = ft_add_c_free(dst, b);
	*i = *i + 1;
	return (dst);
}

char	*add_laststatus(char *dst, int g_status)
{
	char	*stat;

	stat = NULL;
	stat = ft_itoa(g_status);
	if (stat == NULL)
		exit(errtx_all_free_exit(1, "last status malloc failed\n"));
	dst = ft_strjoin_free(dst, stat);
	if (dst == NULL)
		exit(errtx_all_free_exit(1, "last status malloc failed\n"));
	return (dst);
}

char	*ini_expanded(void)
{
	char	*expanded;

	expanded = malloc(1);
	if (expanded == NULL)
		err_free_env_exit("check expand malloc failed");
	expanded[0] = '\0';
	return (expanded);
}

char	*check_expand_hd(char *tx)
{
	int		i;
	char	*expanded;

	i = 0;
	if (tx[0] == '$' && tx[1] == '\0')
		return (tx);
	expanded = ini_expanded();
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
			i = i + count_expand_length(&tx[i + 1]);
		}
		else
			expanded = ft_add_c_free(expanded, tx[i]);
		i++;
	}
	free(tx);
	return (expanded);
}
