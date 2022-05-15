#include "check_syntax.h"

//int	start_checking(t_source *src)
int	check_syntax_errors(t_source *src, t_infos *info)
{
	//printf("\n"BLU"TEEST\n"RES);

	int	ret;

	if (src->inputline_size == 0)
		return (0);

	src->currpos = 0;
	ret = check_quotes(src);
	if (ret == 1) // MAYBE THIS IS NOT USED ANYMORE, ALL CHARS CAN BE VALID
	{
		printf("\n"BLU"        unsuported char\n"RES);
		print_err_msg(MSG_UNSUPPORTED);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}
	else if (ret == 2)
	{
		printf("\n"BLU"        bad quotes\n"RES);
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
	return (0);
}
