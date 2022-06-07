#include "minishell.h"

/*
// NOTES, ISSUES ////////////////////////////////////////////////////

***************************************************************************
*** A temp Line to see if this will dissapear after merging from Kitos'main
*** Because I merged my branch to main, then I started changing this branch
*** Then Kito merged her branch to Main.
*** So now some files on main are older than the ones on my branch.
*** If I now pull from main, will my new files be overwritten by my old files?



Cases like:  < asdqwe   or    < nopermissionfile
	It should not exit, just print error
	I added a similar function like errtext_exit. But it probably needs to 
		return to all the previous calls.


SAMPLE TEST:
	make && valgrind ./minishell "<< here cat | < infile < main.c cat | grep a > outfile | ls -l -a | wc -c  > outfile > out1 > out2"
<< here cat | < infile < main.c cat | grep a > outfile | ls -l -a | wc -c  > outfile > out1 > out2

*/

void	free_and_read(t_source *src, t_infos *info, int history)
{
	if (history == 1)
		add_history(src->inputline);	
	if (src->inputline != NULL)
		free(src->inputline);
	src->inputline = readline(info->prompt);
}



int	g_status;
// ORIGINAL FROM testmain.c
int	main(int argc, char *argv[], char *envp[])
{
	printf("\n");
	t_source	src;
	t_cmd		*cmd_list;
	t_infos		info;
	//char		*line;

	(void) argc;
	(void) argv;
	// src.inputline = NULL;
	ms_init(&info, envp);
	//free_and_read(&src, &info, 0);

	//line = readline(info.prompt);

	if (argc == 2)	// JUST FOR TESTING ////////////////////////
	{
		//printf(GRN"tester mode:\n"RES);
		src.inputline = argv[1];
		src.inputline_size = strlen(src.inputline);
		if (check_syntax_errors(&src) != 0)
			return (SYNTAX_ERROR);
		cmd_list = make_commands(&src  /*, &info  */);
		g_status = run_cmd(&info, cmd_list);
		free_commands_list(cmd_list);
		clean_data(g_status, &info, NULL);
		printf(GRN"\nexit!!! (tester mode)\n\n"RES);
		// return (0);
		exit(0);
	}
	else
	{
		printf(GRN"Real mode:\n"RES);
		//line = readline(info.prompt);
		src.inputline = NULL;
		free_and_read(&src, &info, 0);
		while (src.inputline)
		{
			if (ft_strlen(src.inputline) > 0)
			{
				if (check_syntax_errors(&src) != 0)
				{
					//add_history(line);				// ADDED JAKA: INCASE OF ERROR MUST NOT EXIT, BUT LOOP AGAIN
					//free(line);
					//line = readline(info.prompt);
					free_and_read(&src, &info, 1);
					continue ;
				}
				add_history(src.inputline);
				cmd_list = make_commands(&src);
				g_status = run_cmd(&info, cmd_list);
				free_commands_list(cmd_list);	
			}
			free_and_read(&src, &info, 0);
			//free(line);
			//line = readline(info.prompt);
		}
	}

	//system("leaks minishell");
	return (clean_data(g_status, &info, "exit\n"));
}

/*
/// CURRENT ISSUES: //////////////////////////

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




- In case of ctrl_C, main() reaches the end and prints "exit" ????



Things to check:
   Heredoc and ctrl-C: If followed by other commands, heredoc should execute first and other commands must not execute

	Maybe always after ft_strdup, needs to check if NULL

////////////////////////////////////////////////


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



-------------------------------------------------------------------------



// INFO ABOUT KEY BINDING SETTINGS
stty -a
    interupt    ctrl-C      SIGINT
    quit        ctrl-\      SIGQUIT
    send oef    ctrl-D

*/