#include "../minishell.h"
#define SYNTAX_ERROR	258
#define MSG_SYNTAX		"Syntax error"

int	check_syntax_errors(t_source *src)
{
	src->currpos = 0;
	src->inputline_size = ft_strlen(src->inputline);
	if (src->inputline_size == 0)
		return (0);
	if (check_quotes(src) != 0)
	{
		print_err_msg(MSG_SYNTAX);
		return (SYNTAX_ERROR);
	}
	if (check_pipes(src) != 0)
	{
		print_err_msg(MSG_SYNTAX);
		return (SYNTAX_ERROR);
	}
	if (check_redirects(src) != 0)
	{
		print_err_msg(MSG_SYNTAX);
		return (SYNTAX_ERROR);
	}
	return (0);
}
