#ifndef MINISHELL_H
# define MINISHELL_H
# define ERROR_RETURN 1
# define ERROR_RETURN_MIN -1

# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termcap.h>
# include <termios.h>

typedef struct s_env		t_env;
typedef struct s_infos		t_infos;

struct s_env
{
	char	*name;
	char	*value;
	t_env	*next;
};

struct	s_infos
{
	t_env			*start_env;
	struct termios	termios_save;
	struct termios	termios_new;
	char			prompt[13];
};

typedef struct s_cmd
{
	t_env			*start_env;
	char			**args;
	int				fd_in;
	int				fd_out;
	char			*infile;
	char			*outfile;
	struct s_cmd	*next;
}	t_cmd;

#endif

// Added Jaka: Testing branches
