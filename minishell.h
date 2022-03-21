#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env		t_env;
typedef struct s_variable	t_variable;

struct s_env
{
	char	*name;
	char	*value;
	t_env	*next;
};

struct	s_variable
{
	char		*name;
	char		*value;
	t_variable	*next;
};

struct	s_infos
{
	t_env		*start_env;
	t_variable	*start_v;
};

typedef struct s_cmd
{
	t_env			*start_env;
	char			**args;
	int				fd_in;
	int				d_out;
	char			*infile;
	char			*outfile;
	struct s_cmd	*next;
}	t_cmd;

extern int	laststatus; //maybe the last cmd exit status for $?

#endif
