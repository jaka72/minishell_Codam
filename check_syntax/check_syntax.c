#include "check_syntax.h"

int	check_syntax_errors(char *line, t_source *src, t_infos *info)
{
	int	ret;
	src->inputline = line;
	src->currpos = 0;
	src->inputline_size = ft_strlen(src->inputline);
	if (src->inputline_size == 0)
		return (0);
	ret = check_quotes(src);
	if (ret == 1) // MAYBE THIS IS NOT USED ANYMORE, ALL CHARS CAN BE VALID
	{
		// printf("\n"BLU"        unsuported char\n"RES);
		// print_err_msg(MSG_UNSUPPORTED);
		// info->exit_code = 258;
		// return (SYNTAX_ERROR);
	}
	else if (ret == 2)
	{
		print_err_msg(MSG_QUOTES);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}
	if (check_pipes(src) != 0)
	{
		print_err_msg(MSG_PIPES);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}
	if (check_redirects(src) != 0)
	{
		print_err_msg(MSG_REDIRECTS);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}
	//printf("\n"BLU"After check redirects\n"RES);
	return (0);
}
