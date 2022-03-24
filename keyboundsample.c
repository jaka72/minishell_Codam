#include <stdio.h>
#include <stdlib.h>
#define _SVID_SOURCE 1
#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

struct termios termios_save;

void reset_the_terminal(void)
{
	tcsetattr(0, 0, &termios_save );
}

sig_atomic_t the_flag = 0;

void handle_the_stuff(int num)
{
	// rl_redisplay();
	// char buff[4];
	// buff[0] = ' ';
	// buff[2] = ' ' ;
	// num /= 10;
	// buff[1] = ' ' ;
	// buff[3] = ' ';
	// write(0, buff, sizeof buff);
	// the_flag = 1;
	// rl_on_new_line();
	// printf("\n");
}

int main (void)
{
	int rc;
	int ch;
	struct termios termios_new;
	char		*line = NULL;

	rc = tcgetattr(0, &termios_save );
	if (rc) {perror("tcgetattr"); exit(1); }

	rc = atexit(reset_the_terminal);
	if (rc) {perror("atexit"); exit(1); }

	termios_new = termios_save;
	// termios_new.c_iflag &= ~(BRKINT);
	termios_new.c_iflag &= BRKINT;
	termios_new.c_cc[VQUIT] = 0;
	termios_new.c_cc[VEOF] = 4;
	termios_new.c_cc[VINTR] = 3;		
	rc = tcsetattr(0, 0, &termios_new);
	if (rc) {perror("tcsetattr"); exit(1); }

	signal(SIGINT, handle_the_stuff);

	printf("(pseudoshell)Start typing:\n" );

	// while(1) {
	// 		ch = getc(stdin);
	// 		if (the_flag) {
	// 				printf("Saw the signal, last character was %02x\n", (unsigned) ch);
	// 				break;
	// 				}
	// 		}

	if (termios_new.c_iflag & IGNBRK)
      puts("IGNBRK is set");
    else
      puts("IGNBRK is not set");
	if (termios_new.c_iflag & BRKINT)
      puts("BRKINT is set");
    else
      puts("BRKINT is not set");
    if (termios_new.c_cflag & PARODD)
      puts("Odd parity is used");
    else
      puts("Even parity is used");
    if (termios_new.c_lflag & ISIG)
      puts("ISIG is set");
    else
      puts("ISIG is not set");
    printf("The end-of-file character is x'%02x'\n", termios_new.c_cc[VEOF]);


	while ((line = readline("->")))
	{
		// printf("line is %s\n", line);
		add_history(line);
		// here parsing and make a linkedlist of t_cmd
		// after makint t_cmd list, fork and execute
		free(line);
	}
	rl_clear_history();
	rc = tcsetattr(0, 0, &termios_save);
	return (0);
}