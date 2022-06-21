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

static char	*check_doller_expand(char *expanded, char *tx, int *i, int *d_flag)
{
	if (tx[*i] == '$' && tx[*i + 1] == '\"' && *d_flag == 1)
	{
		expanded = ft_add_c_free(expanded, '$');
		*d_flag ^= FLAG1;
		*i = *i + 1;
	}
	else if (tx[*i] == '$' && tx[*i + 1] == '=')
		expanded = add_two_cha(expanded, '$', '=', i);
	else if ((tx[*i] == '$' && tx[*i + 1] == ' ')
		|| (tx[*i] == '$' && tx[*i + 1] == '\0'))
		expanded = ft_add_c_free(expanded, '$');
	else if (tx[*i] == '$' && tx[*i + 1] == '?' )
	{
		expanded = add_laststatus(expanded, g_gl.g_status);
		*i = *i + 1;
	}
	else if (tx[*i] == '$')
	{
		expanded = add_expanded(expanded, &tx[*i + 1]);
		*i = *i + count_expand_length(&tx[*i + 1]);
	}
	return (expanded);
}

char	*check_expand_file(char *tx)
{
	int		i;
	int		d_flag;
	char	*expanded;

	i = 0;
	d_flag = 0;
	expanded = ini_expanded();
	if ((tx[0] == '\'' && tx[1] == '\'' && tx[2] == '\0')
		|| (tx[0] == '\"' && tx[1] == '\"' && tx[2] == '\0'))
		return (ft_add_c_free(expanded, '\0'));
	if (tx[0] == '$' && tx[1] == '\0')
		return (ft_add_c_free(expanded, '$'));
	while (tx[i] != '\0')
	{
		if (tx[i] == '\"' || tx[i] == '\'')
			expanded = check_quote_expand(expanded, tx, &i, &d_flag);
		else if (tx[i] == '$')
			expanded = check_doller_expand(expanded, tx, &i, &d_flag);
		else
			expanded = ft_add_c_free(expanded, tx[i]);
		i++;
	}
	if (expanded[0] == '\0')
		return (free_return_null(expanded));
	return (expanded);
}

char	*check_expand(char *tx)
{
	char	*expanded;

	expanded = check_expand_file(tx);
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
				args[temp] = args[temp + 1];
				temp++;
			}
			args[temp] = NULL;
			i--;
		}
		i++;
	}
	return (args);
}
