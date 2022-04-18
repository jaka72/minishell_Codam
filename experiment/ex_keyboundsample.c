# include "../minishell.h"

sig_atomic_t the_flag = 0;

void handle_c()
{
	the_flag = 1;
	write(1, "\n", 1);
//	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*newb;
	unsigned char	newc;
	size_t			i;

	newb = (unsigned char *)b;
	newc = (unsigned char)c;
	i = 0;
	while (i < len)
	{
		newb[i] = newc;
		i++;
	}
	return (b);
}

int	main(void)
{
	int				rc;
	char			*line;
	t_infos		init;
	struct sigaction	sig_act;

	line = NULL;
	rc = tcgetattr(0, &init.termios_save);
	if (rc)
	{
		perror("tcgetattr");
		exit(1);
	}
	init.termios_new = init.termios_save;
	init.termios_new.c_iflag &= BRKINT;
	init.termios_new.c_lflag &= ~(ECHOCTL); 	// deactivate printing "^C" (Ctr+C)
	init.termios_new.c_cc[VQUIT] = 0;  		// deactivate sending "SIGQUIT" with ^/ (Ctr+/)
	init.termios_new.c_cc[VEOF] = 4; 		// activate "EOF" with ^D (Ctr+D)
	init.termios_new.c_cc[VINTR] = 3;		// activate "SIGINT" with ^C (Ctr+C)		
	rc = tcsetattr(0, 0, &init.termios_new);
	if (rc)
	{
		perror("tcsetattr");
		exit(1);
	}
	// signal(SIGINT, handle_the_stuff);
	ft_memset(&sig_act, 0, sizeof(sig_act));
	sig_act.sa_handler = &handle_c;
	sig_act.sa_flags = 0;
	sig_act.sa_flags |= SA_RESTART;
	if (sigaction(SIGINT, &sig_act, NULL) != 0)
        perror("sigaction");
	if (sig_act.sa_flags & SA_RESETHAND)
		puts("SA_RESETHAND is set");
	printf("Start typing:\n" );

	if (init.termios_new.c_iflag & IGNBRK)
		puts("IGNBRK is set");
	else
		puts("IGNBRK is not set");
	if (init.termios_new.c_iflag & BRKINT)
		puts("BRKINT is set");
	else
		puts("BRKINT is not set");
	if (init.termios_new.c_cflag & PARODD)
		puts("Odd parity is used");
	else
		puts("Even parity is used");
	if (init.termios_new.c_lflag & ECHOCTL)
		puts("ECHOCTL is set");
	else
		puts("ECHOCTL is not set");
	printf("The end-of-file character is x'%02x'\n", init.termios_new.c_cc[VEOF]);

	while ((line = readline("->")))
	{
		printf("line is %s\n", line);
		if(the_flag == 1)
			printf("interupted!\n");
		if (strlen(line) > 0)
			add_history(line);
		// here parsing and make a linkedlist of t_cmd
		// after makint t_cmd list, fork and execute
		free(line);
		the_flag = 0;
	}
	rl_clear_history();
	rc = tcsetattr(0, 0, &init.termios_save);
	return (0);
}
