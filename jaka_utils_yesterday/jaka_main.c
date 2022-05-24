#include "utils.h"


// THIS MAIN IS FROM SYNTAX CHECK
int	main(int argc, char **argv)
{
	// ADDED BY JAKA //////////////////////////////////////////////////
	// INITIAL CHECKING FOR SYNTAX ERRORS ///////////////////////////// 

	//int			i;
	//int			ret;
	t_source	src;
	t_cmd		*cmd_list;
	// t_cmd		*new_cmd;
	// t_cmd		*temp;	// just for printing 
	

	src.inputline = NULL;
	if (argc == 2)
	{
		src.inputline = argv[1];
		//printf(CYN"line len: %ld\n"RES, src.inputline_size);
	}
	else
	{
		// CALL READLINE() ETC ...
		src.inputline = readline("minishell: ");
		//printf("\nPls provide the line!\n");
		// return (0);
	}
	src.inputline_size = strlen(src.inputline);

	printf(DYEL"A) [%s] len: %ld\n"RES, src.inputline, src.inputline_size);

	// STOP IN CASE NO INPUT
	if (src.inputline == NULL || src.inputline[0] == '\0')
		return (0);




	// INSERT THIS AFTER KEIKO'S readline() AND BEFORE history() ////////////////	
	
	if (check_syntax_errors(&src) != 0)
		return (SYNTAX_ERROR);


	cmd_list = make_commands(&src);

	// FREE AFTER EXECUTION /////////////////////////////////////////////////////
	free_commands_list(cmd_list);



	//if (argc == 2)
	//	exit(0);
	// else
	// 	continue ;

	// ABOVE IS JAKA'S CODE //////////////////////////////////////

	// add_history(src->inputline);


	return (0);
}
