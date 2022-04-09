#include "syntax_header.h"

/* 
gcc syntax_main.c ft_utils.c syntax_check_pipes.c syntax_check_quotes.c syntax_check_redirects.c libft_functions.c  -lreadline -o minishell && ./minishell "ls||wc"
*/

// NEED TO FREE BEFORE EXIT AND BEFORE REPEATING LOOP
// MAYBE NOT NEEDED TO EXIT BECAUSE IT EXITS ALREADY BEFORE
int	start_checking(t_source *src, int is_tester)
{
	if (check_quotes(src) != 0)
	{
		print_err_msg(src, MSG_QUOTES);
		return (1);
	}
	if (check_pipes(src) != 0)
	{
		print_err_msg(src, MSG_PIPES);
		return (1);
	}
	if (check_redirects(src) != 0)
	{
		print_err_msg(src, MSG_REDIRECTS);
		return (1);
	}
	return (0);
}



int	check_syntax_errors(t_source *src, int is_tester, char **argv)
{
	src->currpos = 0;
	if (is_tester == 0)
		; //src->inputline = readline("minishell: ");  // IT IS IN MAIN STORED FROM 'line'
	else if (is_tester == 1) // just for the tester
		src->inputline = argv[1];
	if ((src->inputline == NULL || src->inputline[0] == '\0') && is_tester == 1)
		exit(EXIT_SUCCESS);
	src->inputline_size = strlen(src->inputline);
	if (start_checking(src, is_tester) != 0)
		return (1);
	return (0);
}


// int main(int argc, char **argv)
// {
// 	// ADDED BY JAKA //////////////////////////////////////////////////
// 	// INITIAL CHECKING FOR SYNTAX ERRORS ///////////////////////////// 

// 	t_source	src;
// 	int			is_tester; // just for tester.sh
// 	src.inputline = NULL;
// 	is_tester = 0;
// 	if (argc == 2)
// 		is_tester = 1;
		
// 	while (1)
// 	{
// 		// INSERT THIS AFTER KEIKO'S readline() ///////////////////////
// 		check_syntax_errors(&src, is_tester, argv);
	
// 		if (is_tester == 1)
// 			exit(0);
// 		else
// 			continue ;





// 		// add_history(src->inputline);
// 	}
// 	return (0);
// }



