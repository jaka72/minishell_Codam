#include "check_syntax.h"

//int	start_checking(t_source *src)
int	check_syntax_errors(t_source *src)
{
	int	ret;

	src->currpos = 0;
	ret = check_quotes(src);
	if (ret == 1)
	{
		print_err_msg(MSG_UNSUPPORTED);
		return (SYNTAX_ERROR);
	}
	else if (ret == 2)
	{
		print_err_msg(MSG_QUOTES);
		return (SYNTAX_ERROR);
	}
	if (check_pipes(src) != 0)
	{
		print_err_msg(MSG_PIPES);
		return (SYNTAX_ERROR);
	}
	if (check_redirects(src) != 0)
	{
		print_err_msg(MSG_REDIRECTS);
		return (SYNTAX_ERROR);
	}
	return (0);
}
