/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_util_2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 10:05:46 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define FLAG1 1

char	*add_expanded(char *dst, char *src, t_util *st_base)
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
		msg_and_exit("add expand malloc failed\n", 1);
	ft_strlcpy(temp, src, i + 1);
	expanded = name_expand(temp, st_base);
	dst = ft_strjoin_free(dst, expanded);
	if (dst == NULL)
		msg_and_exit("add expand malloc failed\n", 1);
	return (dst);
}

char	*add_two_cha(char *dst, char a, char b, int *i)
{
	dst = ft_add_c_free(dst, a);
	dst = ft_add_c_free(dst, b);
	*i = *i + 1;
	return (dst);
}

char	*add_laststatus(char *dst, int *ex_stat)
{
	char	*stat;

	stat = NULL;
	stat = ft_itoa(*ex_stat);
	if (stat == NULL)
		msg_and_exit("last status malloc failed\n", 1);
	dst = ft_strjoin_free(dst, stat);
	if (dst == NULL)
		msg_and_exit("last status malloc failed\n", 1);
	return (dst);
}

char	*ini_expanded(void)
{
	char	*expanded;

	expanded = malloc(sizeof(char) * 1);
	if (expanded == NULL)
		msg_and_exit("check expand malloc failed\n", 1);
	expanded[0] = '\0';
	return (expanded);
}

char	*check_expand_hd(char *tx, t_util *st_base)
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
			expanded = add_expanded(expanded, &tx[i + 1], st_base);
			i = i + count_expand_length(&tx[i + 1]);
		}
		else
			expanded = ft_add_c_free(expanded, tx[i]);
		i++;
	}
	free(tx);
	return (expanded);
}
