#ifndef MINISHELL_H
# define MINISHELL_H
# define ERROR_RETURN 1
# define ERROR_RETURN_MIN -1

# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termcap.h>
# include <termios.h>
# include "libft/libft.h"

typedef struct s_global		t_global;
typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_source		t_source;
typedef struct s_pid		t_pid;

struct	s_global
{
	int				g_status;
	t_env			*start_env;
	t_cmd			*start_cmd;
	char			prompt[13];
	int				ini_fd[2];
	struct termios	termios_save;
};

struct s_env
{
	char	*name;
	char	*value;
	t_env	*next;
};

struct s_cmd
{
	char			**args;
	int				fd_in;		// fd_in  : default is 0, if "<" -2, if "<<" -3.
	int				fd_out;		// fd_out : default is 1, if ">" -2, if ">>" -3.
	char			**infile;
	char			**outfile;
	char			**heredoc;
	struct s_cmd	*next;
};

struct s_source
{
	char	*inputline;
	long	inputline_size;
	long	currpos;
};

struct s_pid
{
	pid_t	pid;
	int		newpipe[3];
	int		status;
	pid_t	last_pid;
};


extern t_global	gl;

# include "jaka_utils/utils.h"
# include "check_syntax/check_syntax.h"
# include "parsing/make_commands.h"
# include "builtins/builtins.h"

// util/error.c
void	errtext_exit(char *text);
int		errtext_return(char *text);
void	free_envlist(void);
int		free_strings(char **strs);
void	free_tcmd(void);
void	err_free_env_exit(char *text);
int		err_all_free_exit(int exitnum);
int		errtx_all_free_exit(int exitnum, char *tx);

// util/util.c
char	*free_return_null(char *text);
char	*ft_add_c_free(char *s1, char c);
void	clean_fd(void);
int		clean_data(int status, char *text);

// init/init.c
void	handle_sigint(int num);
void	handle_sigquit(int num);
void	handle_sigint_p(int num);
void	handle_sigquit_p(int num);
void	handle_sigquit_hd(int num);
void	handle_sigint_hd(int num);
void	ms_init( char *envp[]);

// init/env.c
t_env	*last_env(void);
int		count_env(void);
t_env	*get_name_value(t_env *env, char *envtext);
t_env	*get_env(char *envp[]);
char	**get_env_array(void);

// init/expand.c
void	print_env(void);
char	*name_expand(char *tx);
char	*check_expand(char *tx);
char	*check_expand_hd(char *tx);
char	**expand_array(char **args);

// file/heredoc.c
int		get_heredoc(char *limiter, int fd_out);
int		make_heredoc(char *limiter);

// exec/fd.c
int		check_infile_fd(t_cmd *str);
int		check_outfile_fd(t_cmd *str);
int		connect_fd(t_cmd *current);
void	reset_fd_sig(void);

// exec/exec.c
char	*ft_find_env_passnum(char *envp[]);
char	*ft_make_binpass(int i, char *pass, char *cmd);
char	*ft_findshell_pass(char *cmd, char *envp[]);
int		ms_execve(t_cmd *str);
int		run_cmd(void);


// added jaka: to count the array of arguments:
int		count_elems(char **arr);




// main.c


#endif