#ifndef MINISHELL_H
# define MINISHELL_H

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
extern t_global				g_gl;

struct	s_global
{
	int				g_status;
	t_env			*start_env;
	t_cmd			*start_cmd;
	char			prompt[13];
	int				ini_fd[2];
	struct termios	termios_save;
	struct termios	termios_new;
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
	char			**heredoc;
	char			**files;
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
	pid_t	cu_pid;
	int		newpipe[3];
	int		status;
	pid_t	last_pid;
	t_cmd	*temp_cmd;
};

# include "jaka_utils/utils.h"
# include "check_syntax/check_syntax.h"
# include "parsing/make_commands.h"
# include "builtins/builtins.h"

// util/error.c
int		return_perr(int i, char *tx);
int		return_errtx(int i, char *tx);
void	err_free_env_exit(char *text);
int		err_all_free_exit(int exitnum);
int		errtx_all_free_exit(int exitnum, char *tx);

// util/free.c
void	free_envlist(void);
int		free_strings(char **strs);
void	free_tcmd(void);

// util/util.c
char	*free_return_null(char *text);
char	*ft_add_c_free(char *s1, char c);
void	clean_fd(void);
int		clean_data(int status, char *text);

// init/signal_handle.c
void	handle_sigint(int num);
void	handle_sigquit(int num);
void	handle_sigint_p(int num);
void	handle_sigquit_p(int num);
void	handle_sigint_hd(int num);

// init/init.c
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
char	*check_expand_file(char *tx);
char	*check_expand(char *tx);
char	*check_expand_hd(char *tx);
char	**expand_array(char **args);

// heredoc/heredoc_util.c
void	init_pid_sig_hd(t_pid *pidinfo);
void	reset_termios(void);
int		reset_fd_sig_close(int fd);

// heredoc/heredoc.c
int		read_heredoc(char *limiter, int fd_out);
void	set_status(int status);
int		make_heredoc(char *limiter);
int		open_heredoc(t_pid *pid);

// exec/open_fd.c
int		set_infile(t_cmd *str, int i);
int		set_outfile(t_cmd *str, int i);
int		set_outfile_ap(t_cmd *str, int i);
int		open_file_fd(t_cmd *str);

// exec/connect_fd.c
int		connect_fd(t_cmd *current);
void	check_close_fd(int fdin, int fdout);
void	reset_fd_sig(void);

// exec/find_path.c
char	*ft_find_env_pathnum(char *envp[]);
char	*ft_make_binpass(int i, char *pass, char *cmd);
char	*ft_findshell_path(char *cmd, char *envp[]);

// exec/check_files.c
int		check_file_access(t_cmd	*current);

// exec/exec.c


int		run_cmd(void);

// exec/ms_execve.c
int		ms_execve(t_cmd *str);
int		cmd_is_custom(char **args, char **envs);
int		cmd_with_slash(char *path, char **args, char **envs);
int		cmd_no_slash(char *path, char **args, char **envs);


// added jaka: to count the array of arguments:
int		count_elems(char **arr);




// main.c


#endif