/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 17:00:31 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define FLAG1 1

static char	*check_quote_expand(char *expanded, char *tx, int *i, int *d_flag)
{
	if (tx[*i] == '\"')
		*d_flag ^= FLAG1;
	else if (tx[*i] == '\'' && (*d_flag & FLAG1) > 0)
		expanded = ft_add_c_free(expanded, '\'');
	else if (tx[*i] == '\'' && tx[*i + 1] != '\0')
	{
		expanded = add_singlequote(expanded, &tx[*i + 1]);
		*i = *i + count_single_length(&tx[*i + 1]);
	}
	return (expanded);
}

static char	*check_doller_expand(char *expanded, char *tx, t_util *st_base)
{
	if (tx[st_base->i] == '$' && tx[st_base->i + 1] == '\"'
		&& st_base->flag == 1)
	{
		expanded = ft_add_c_free(expanded, '$');
		st_base->flag ^= FLAG1;
		(st_base->i)++;
	}
	else if (tx[st_base->i] == '$' && tx[st_base->i + 1] == '=')
		expanded = add_two_cha(expanded, '$', '=', &st_base->i);
	else if ((tx[st_base->i] == '$' && tx[st_base->i + 1] == ' ')
		|| (tx[st_base->i] == '$' && tx[st_base->i + 1] == '\0'))
		expanded = ft_add_c_free(expanded, '$');
	else if (tx[st_base->i] == '$')
	{
		expanded = add_expanded(expanded, &tx[st_base->i + 1], st_base);
		st_base->i = st_base->i + count_expand_length(&tx[st_base->i + 1]);
	}
	return (expanded);
}

char	*check_expand_file(char *tx, int *ex_stat, t_util *st_base)
{
	char	*str;

	st_base->i = 0;
	st_base->flag = 0;
	str = ini_expanded();
	if ((tx[0] == '\'' && tx[1] == '\'' && tx[2] == '\0')
		|| (tx[0] == '\"' && tx[1] == '\"' && tx[2] == '\0'))
		return (ft_add_c_free(str, '\0'));
	if (tx[0] == '$' && tx[1] == '\0')
		return (ft_add_c_free(str, '$'));
	while (tx[st_base->i] != '\0')
	{
		if (tx[st_base->i] == '\"' || tx[st_base->i] == '\'')
			str = check_quote_expand(str, tx, &st_base->i, &st_base->flag);
		else if (tx[st_base->i] == '$' && tx[st_base->i + 1] == '?')
			str = show_last_status(str, ex_stat, &st_base->i);
		else if (tx[st_base->i] == '$')
			str = check_doller_expand(str, tx, st_base);
		else
			str = ft_add_c_free(str, tx[st_base->i]);
		st_base->i++;
	}
	if (str[0] == '\0')
		return (free_return_null(str));
	return (str);
}

char	*check_expand(char *tx, int *ex_stat, t_util *st_base)
{
	char	*expanded;

	expanded = check_expand_file(tx, ex_stat, st_base);
	free(tx);
	return (expanded);
}

char	**expand_array(char **args, int *ex_stat, t_util *st_base)
{
	int	i;

	i = 0;
	while (args[i])
	{
		args[i] = check_expand(args[i], ex_stat, st_base);
		if (args[i] == NULL)
		{
			while (args[i + 1])
			{
				args[i] = args[i + 1];
				i++;
			}
			args[i] = NULL;
			break ;
		}
		i++;
	}
	if (args[0] == NULL)
	{
		free(args);
		args = NULL;
	}
	return (args);
}
