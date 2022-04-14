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

# include "check_syntax/syntax_header.h"

typedef struct s_env		t_env;
typedef struct s_infos		t_infos;

sig_atomic_t	g_flag;

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
	int				ini_fd[2];
};

typedef struct s_cmd
{
	t_env			*start_env;
	char			**args;
	int				fd_in;		// fd_in  : default is 0, if "<" -2, if "<<" -3.
	int				fd_out;		// fd_out : default is 1, if ">" -2, if ">>" -3.
	char			**infile;
	char			**outfile;
	char			**heredoc;
	int				count_args;			// NEEDED TO KEEP TRACK OF NR OF ELEMENTS,
	int				count_infiles;		// TO BE ABLE TO realloc(), EACH TIME: nr + 1
	int				count_outfiles;
	int				count_heredocs;
	struct s_cmd	*next;
}	t_cmd;

// util/error.c
void	errtext_exit(char *text);
void	free_envlist(t_infos *infos);
int		free_strings(char **strs);
void	free_tcmd(t_cmd *cmds);
void	err_free_env_exit(t_infos *infos, char *text);

// util/util.c
char	*make_malloc_str(char *text);
char	*free_return_null(char *text);

// init/init.c
void	handle_sigint(int num);
void	ms_init(t_infos *info, char *envp[]);

// init/env.c
t_env	*last_env(t_env *start_env);
int		count_env(t_env *start_env);
t_env	*get_name_value(t_env *env, char *envtext);
t_env	*get_env(t_infos *infos, char *envp[]);
char	**get_env_array(t_env *start_env);

// init/expand.c
void	print_env(t_infos *info);
char	*name_expand(t_infos *info, char *tx);
char	*check_expand(t_infos *info, char *tx);

// file/heredoc.c
char	*addtext_free(char *s1, char *s2, int *num);
char	*check_limiter(char *buff, char *limiter);
char	*write_free(int fd, char *checklimit);
int		get_heredoc(char *limiter, int fd_out);
int		make_heredoc(char *limiter);

// exec/exec.c
char	*ft_find_env_passnum(char *envp[]);
char	*ft_make_binpass(int i, char *pass, char *cmd);
char	*ft_findshell_pass(char *cmd, char *envp[]);

#endif