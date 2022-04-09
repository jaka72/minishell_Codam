# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "check_syntax/syntax_header.h"


int	 main(int argc, char **argv) // ADDED JAKA, NEED argv FOR THE TESTER
{
	char		*prompt;
	char		*line = NULL;


	// ADDED JAKA: INITIAL CHECKING FOR SYNTAX ERRORS/////////////// 
	t_source	src;
	int			is_tester; // just for tester.sh
	src.inputline = NULL;
	is_tester = 0;
	if (argc == 2)
		is_tester = 1;
	/////////////////////////////////////////////////////////////////


//	prompt = getenv("PS1");
//	if (prompt == NULL)
		prompt = "minishell > ";
	// before while loop, get env
	while ((line = readline(prompt)))
	{

		// ADDED JAKA: INITIAL CHECKING FOR SYNTAX ERRORS/////////////
		src.inputline = line;
		check_syntax_errors(&src, is_tester, argv);
		if (is_tester == 1)
			exit(0);
		else
			continue ;
		/////////////////////////////////////////////////////////////


		printf("%s\n", line);
		add_history(line);

		// here parsing and make a linkedlist of t_cmd
		// after makint t_cmd list, fork and execute
		free(line);
	}
	rl_clear_history();
	return (0);
}
