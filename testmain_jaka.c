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
	printf("\n");
	t_source	src;
	t_cmd		*cmd_list;
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
		cmd_list = make_commands(&src);

		gl.g_status = run_cmd();
		free_commands_list(cmd_list);
		clean_data(gl.g_status, NULL);
		//printf(GRN"\nexit! (tester mode)\n\n"RES);
		return (0);
	}
	else
	{
		// printf(GRN"Real mode:\n"RES);
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
	}

	//system("leaks minishell");
	return (clean_data(gl.g_status, "exit\n"));
}

/*
/// CURRENT ISSUES: ////////////////////////////////////////////////////////////////////////

- What if lib command has no permissions? Now we say no such file.
	It has to say Bash: ls: No permission, and exit code 126

- ./builtins/$ABCa  ==> existing folder, but nonexisiting variable, It shows access() as it is X_OK ????


- in Bash, it gives 2 different err messages in separate terminals ??? in case traying ti execute a folder
		./libft  or  ./libft/    or libft
	msg1: no such file, msg2: it is a folder ????

- In Bash, if you try execit a text file, it will try to execute a word from each line
	and write Command not found.


- check ls | exit qwe wer , exit 255 ! Now 139 ????
	Check always exit code, after exit and other commands, because it is sometimes some hidden error, and it 
		prints 139, not 255


- heredoc alway exits,	- with normal input: << here cat ... $HOME ... here ... EXITS!
					- in case nonexisting variable: ... $xxx
					- with no input, just here

- Should minishell be able to create var $ABC ,without doing export

- in case <qwewer ,could add in front of error msg: minishell: qwewer: No such file....

- <nopermiss_file or >nopermiss_file ,writes 2x no permission.

- inside heredoc, if pressed ctrl-c, will go back to promt, then if again ctrl-C, prints black lines
- in heredoc, if line not empty, first ctrl-D nothing, seconmd ctrl-D exits


- In case of nonexisting command, still reachable: run_cmd, store_into command ... 
		Try if it is better after Keiko added 2 lines.

- There is forbidden printf in clean_data, also in expand and export ...


- MALLOC  -  still reachable:  Looks like with builtins this happens
	no problem: ls | wc
	a  problem: ls | echo aaa    or    echo aaa | ls   or   pwd | ls 


- If malloc fails inside make_commands(), it must also free mallocs from ms_init, get_env,
		It looks like it's now fixed with calling clean_data

- If malloc fails in my make_commands(), then it has to free and exit. But there are still mallocs present from 
	ms_init, get_env. lost 24 bytes in 1 block


- Segfault or hanging, if the env variable does not exist. This was not happening in version 30 may
		echo $OUTFILE  (if nonexistant, should print new line)
		cat $OUTFILE

- File fd.c: For <infiles and >outfiles: in check_infile_fd(): errtext_exit() 
		in case file has no permission
		or it does not exitst
			-  now it exits minishell
			-  also, it prints 2 messages: 1 from check_infile_fd  and 1 original from ????
		
	This function should not exit, but return,


		Something like this:
			int	errtext_return(char *text)	// added jaka, 1 jun
			{								// 		in cases like: < asdqwe
				write(2, "minishell: ", 11);//		it should not exit, but return (like   < asdqwe    )
				perror(text);
				return (ERROR_RETURN);
			}


- There is 2x clear_history in clean_data()

- In case of ctrl_C, main() reaches the end and prints "exit" ????



Things to check:
   Heredoc and ctrl-C: If followed by other commands, heredoc should execute first and other commands must not execute

	Maybe always after ft_strdup, needs to check if NULL

////////////////////////////////////////////////

- How is ctrl-\ behaving? First nothihng, after 1 command says quite 3 ???

- How is ctrl-\ behaving? First nothihng, after 1 command says quite 3 ???

- Maybe all the libft files in main Makefile are not needed listed, because they come from own libft file?


- What must happen if any malloc fails? Message or exit or both?

- After exit 123 qwe, in Bash, $? or echo $? sometimes gives 127 or 1 ???


- random stream has to work


- exit 32 | exit w3e2 | exit   => should not print "exit"
- ls | ls | exit   ==> it should not exit
    
- Such arguments: 2> or 1> ... should be recognized? They would change the stdout:
        cd "doesntexist" 2>/dev/null	 				KO  Sends error msg to bin
        cd "doesntexist" "blabla" 2>/dev/null 			KO  Sends error msg to bin
        cd "builtins" 2>/dev/null 						KO  Moves to bultins, no msg Too many args
        cd '/////' 2>/dev/null 							KO	Moves to root, no msg Too many args



- cd and pwd: Shall I always at start of minishell erase the OLDPWD, even if it already exists in Bash?
- cd: is such case necessary?
        cd "blabla" "blabla" 2>/dev/null        (IT SENDS ERROR MESSAGE TO BIN)

- Redirects: This is allowed in bash:
    aaa<<<asdf     just prints aaa
    aaa <<< asdf     just prints aaa


- Add such cases to tester:
    <" "
    <""
    <  ""
    <""  | 
    <"" | echo | | 
    ...etc

- Syntax error must continue, not exit. Internal Exit code must be set to 258, so " echo $?" must be 258 

- !!! Handeling double quotes ""in all three syntax check, is now messing checking double pipes inside "aaa | | bbb"
    LOOK INSIDE redirect_utils

- In case < ls  ,message twice: infile not exist + no such file ...
        Then it exists minishell

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


// READLINE LIBRARY - get familiar

// INFO ABOUT KEY BINDING SETTINGS
	stty -a
	Case readline:
	    interupt    ctrl-C      SIGINT		makes newline, always. Interupts the readline? Replaces line with "" ???
		quit        ctrl-\      SIGQUIT		prints quit 3 ???
		send oef    ctrl-D		eof			- if readline empty: exits program
			(Sends eof to whom?)			- if readline busy:  nothing
*/


//		If slash at start
//			Extract word after last slash. 
//			Run ft_findshell_pass() to get the correct path.
//			Compare both pass if they match, it can go to access and execve
//		If ./   or   ../ at start,
//			Skip ft_findshell_pass
//			Check if command is X_OK, with acces()
//			save the correct path: it has to be without ./ ,if it is in the same folder 
//								 : if it is in above folder, the execve recognises the ../
// if (str->args[0][0] == '.' || str->args[0][0] == '/' || h