#include "check_syntax.h"

//int	start_checking(t_source *src)
int	check_syntax_errors(char *line, t_source *src, t_infos *info)
{
	// printf("\n"BLU"TEEST\n"RES);

	int	ret;
	src->inputline = line;
	src->currpos = 0;
	src->inputline_size = ft_strlen(src->inputline);

	if (src->inputline_size == 0)
		return (0);


	ret = check_quotes(src);
	// printf("\n"BLU"After check quotes\n"RES);

	if (ret == 1) // MAYBE THIS IS NOT USED ANYMORE, ALL CHARS CAN BE VALID
	{
		// printf("\n"BLU"        unsuported char\n"RES);
		// print_err_msg(MSG_UNSUPPORTED);
		// info->exit_code = 258;
		// return (SYNTAX_ERROR);
	}
	else if (ret == 2)
	{
		//printf("\n"BLU"        bad quotes\n"RES);
		print_err_msg(MSG_QUOTES);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}
	
	//printf("\n"BLU"Before check pipes\n"RES);

	if (check_pipes(src) != 0)
	{
		print_err_msg(MSG_PIPES);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}

	//printf("\n"BLU"After check pipes\n"RES);
	//check_redirects(src);
	if (check_redirects(src) != 0)
	{
		print_err_msg(MSG_REDIRECTS);
		info->exit_code = 258;
		return (SYNTAX_ERROR);
	}
	//printf("\n"BLU"After check redirects\n"RES);
	return (0);
}
