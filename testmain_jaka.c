#include "minishell.h"

/*
// NOTES, ISSUES ////////////////////////////////////////////////////
***************************************************************************

SAMPLE TEST:
	make && valgrind ./minishell "<< here cat | < infile < main.c cat | grep a > outfile | ls -l -a | wc -c  > outfile > out1 > out2"
<< here cat | < infile < main.c cat | grep a > outfile | ls -l -a | wc -c  > outfile > out1 > out2
make && valgrind valgrind --leak-check=full --show-reachable=yes   ./minishell 2> outfile

*/

void	free_and_read(t_source *src, int history)
{
	if (history == 1)
		add_history(src->inputline);	
	if (src->inputline != NULL)
		free(src->inputline);
	src->inputline = readline(gl.prompt);
}

// ORIGINAL FROM testmain.c
int	main(int argc, char *argv[], char *envp[])
{
//	printf("\n");
	t_source	src;
//	t_cmd		*cmd_list;
	//char		*line;

	(void) argc;
	(void) argv;
	// src.inputline = NULL;
	ms_init(envp);
	//free_and_read(&src, 0);

	//line = readline(gl.prompt);

	if (argc == 2)	// JUST FOR TESTING ////////////////////////
	{
		//printf(GRN"tester mode:\n"RES);
		src.inputline = argv[1];
		src.inputline_size = strlen(src.inputline);
		if (check_syntax_errors(&src) != 0)
		{
			return (SYNTAX_ERROR);
		}
		// cmd_list = make_commands(&src);
		gl.start_cmd = make_commands(&src);

		gl.g_status = run_cmd();
		free_tcmd();
//		free_commands_list(cmd_list);
		clean_data(gl.g_status, NULL);
		//printf(GRN"\nexit! (tester mode)\n\n"RES);
		return (0);
	}
	else
	{
		printf(GRN"Tester mode, needs 1 argument!\n\n"RES);
		exit (0);
/*		// printf(GRN"Real mode:\n"RES);
		//line = readline(gl.prompt);
		src.inputline = NULL;
		free_and_read(&src, 0);
		while (src.inputline)
		{
			if (ft_strlen(src.inputline) > 0)
			{
				if (check_syntax_errors(&src) != 0)
				{
					//add_history(line);				// ADDED JAKA: IN CASE OF ERROR MUST NOT EXIT, BUT LOOP AGAIN
					//free(line);
					//line = readline(gl.prompt);
					free_and_read(&src, 1);
					continue ;
				}
				add_history(src.inputline);
				// cmd_list = make_commands(&src);
				cmd_list = make_commands(&src);

				gl.g_status = run_cmd();
				free_commands_list(cmd_list);	
			}
			free_and_read(&src, 0);
			//free(line);
			//line = readline(gl.prompt);
		}
*/
	}

	//system("leaks minishell");
	return (clean_data(gl.g_status, "exit\n"));
}

/*
/// CURRENT ISSUES: ////////////////////////////////////////////////////////////////////////



- Should minishell be able to create var $ABC ,without doing export

- in case <qwewer ,could add in front of error msg: minishell: qwewer: No such file....


- MALLOC  -  still reachable:  Looks like with builtins this happens
	no problem: ls | wc
	a  problem: ls | echo aaa    or    echo aaa | ls   or   pwd | ls 


- If malloc fails inside make_commands(), it must also free mallocs from ms_init, get_env,
		It looks like it's now fixed with calling clean_data

- If malloc fails in my make_commands(), then it has to free and exit. But there are still mallocs present from 
	ms_init, get_env. lost 24 bytes in 1 block



-	Maybe always after ft_strdup, needs to check if NULL

////////////////////////////////////////////////

- How is ctrl-\ behaving? First nothihng, after 1 command says quite 3 ???

- How is ctrl-\ behaving? First nothihng, after 1 command says quite 3 ???

- Maybe all the libft files in main Makefile are not needed listed, because they come from own libft file?


- What must happen if any malloc fails? Message or exit or both?



- cd and pwd: Shall I always at start of minishell erase the OLDPWD, even if it already exists in Bash?
- cd: is such case necessary?
        cd "blabla" "blabla" 2>/dev/null        (IT SENDS ERROR MESSAGE TO BIN)



- Add such cases to tester:
    <" "
    <""
    <  ""
    <""  | 
    <"" | echo | | 
    ...etc



- !!! Handeling double quotes ""in all three syntax check, is now messing checking double pipes inside "aaa | | bbb"
    LOOK INSIDE redirect_utils



- single . or double dot ..  does not report 'command not found'
    should be:   .   filename argument required     exit code 2
                ..   command not found              exit code 127


- echo tilda, should display home path
- echo "a"  now working, now the other issue is back:   "aaa"b  KO
                                                        aaa"b"  ok

- syntax error with pipes, exits, instead of going back to minishell

- checking between quotes, for double || or  > >  =>> probably also needs to check between single quotes





- If pipe or redir is inside quotes, I dont ignore the pipe and redir!!

- Strange behaviour of Bash:
    lsx              command not found           code 127
    /bin/lsx         no such file                code 127        right code but wrong message ???
    cat infilexx     no such file                code 1

- If first cmd includes slash: (xxx/xxx), it will give error: No such file
    lsxxx       "command not found"
    /bin/lsxxx  "no such file"
    So, with slash, it needs to extract the last word before the slash /


- If just pressed tab twice, it prints content from ls ???


- Echo:  escape character, ie:   "  \"    "
    In bash it prints just   "  

- exit: it will probably need some return value to the ms_execve, so it must know, 
    that it has to exit the whole program, not just the child process.

- case: wc | ls    or     cat | ls
    does not give ls, just waits.
    then try ctrl-d, it gives error: wc: stdin: read: input/output error

- No error when command is wrong, ie: catxxx
    Also, no error when wrong option, ie:  cat -xxx

// looking at runcmd ///////////////////////////////////////////////

// looking at runcmd ///////////////////////////////////////////////
- function count_env , seems unnused
- what is rl_catch_signals = 0 ?

-------------------------------------------------------------------------


*/


//		If slash at start
//			Extract word after last slash. 
//			Run ft_findshell_path() to get the correct path.
//			Compare both pass if they match, it can go to access and execve
//		If ./   or   ../ at start,
//			Skip ft_findshell_path
//			Check if command is X_OK, with acces()
//			save the correct path: it has to be without ./ ,if it is in the same folder 
//								 : if it is in above folder, the execve recognises the ../
// if (str->args[0][0] == '.' || str->args[0][0] == '/' || h
//