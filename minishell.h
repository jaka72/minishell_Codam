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
# include <limits.h>	// FOR PATH_MAX, JUST FOR MAC
# include "libft/libft.h"
# include "parsing_util/colors.h"

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
	int				fd_in;
	int				fd_out;
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

// init/env_util.c
t_env	*last_env(void);
// int		count_env(void);
// void	free_errtx_all_free_exit(t_env *env, char *envname);
t_env	*get_name_value(t_env *env, char *envtext);
t_env	*init_tempenv(void);

// init/env.c
t_env	*get_env(char *envp[]);
// char	**ft_realloc_i(char **str, int i);
// char	**make_envstr(t_env *current, char **envs, int i);
char	**get_env_array(void);

// init/expand_util_1.c
char	*name_expand(char *tx);
char	*add_singlequote(char *dst, char *src);
int		count_single_length(char *src);
int		count_expand_length(char *src);
// int		count_expand_length_hd(char *src);

// init/expand_util_2.c
char	*add_expanded(char *dst, char *src);
char	*add_laststatus(char *dst, int g_status);
char	*ini_expanded(void);
char	*check_expand_hd(char *tx);

// init/expand.c
// char	*check_quote_expand(char *expanded, char *tx, int *i, int *d_flag);
// char	*check_doller_expand(char *expanded, char *tx, int *i, int *d_flag);
char	*check_expand_file(char *tx);
char	*check_expand(char *tx);
char	**expand_array(char **args);

// heredoc/heredoc_util.c
void	init_pid_sig_hd(t_pid *pidinfo);
void	reset_termios(void);
int		reset_fd_sig_close(int fd);

// heredoc/heredoc.c
// int		read_heredoc(char *limiter, int fd_out);
// void	set_status(int status);
// int		make_heredoc(char *limiter);
int		open_heredoc(t_pid *pid);

// exec/open_fd.c
// int		set_infile(t_cmd *str, int i);
// int		set_outfile(t_cmd *str, int i);
// int		set_outfile_ap(t_cmd *str, int i);
int		open_file_fd(t_cmd *str);

// exec/connect_fd.c
int		connect_fd(t_cmd *current);
void	check_close_fd(int fdin, int fdout);
void	reset_fd_sig(void);

// exec/find_path.c
// char	*ft_find_env_pathnum(char *envp[]);
// char	*ft_make_binpath(int i, char *pass, char *cmd);
char	*ft_findshell_path(char *cmd, char *envp[]);

// exec/check_files.c
int		check_file_access(t_cmd	*current);

// exec/exec.c
int		run_cmd(void);

// exec/ms_execve.c
int		ms_execve(t_cmd *str);
// int		cmd_is_custom(char **args, char **envs);
// int		cmd_with_slash(char *path, char **args, char **envs);
// int		cmd_no_slash(char *path, char **args, char **envs);

// added jaka: to count the array of arguments:
int		count_elems(char **arr);

// main.c
int		main(int argc, char *argv[], char *envp[]);



int	run_echo_builtin(t_cmd *cmd);
int	run_pwd_builtin(void);
int	run_cd_builtin(t_cmd *cmd);
int	print_msg_var_not_set(char *name);
int	print_error_too_many_args(void);
int	insert_oldpwd_into_list(t_env *env, const char *value);
// int	run_exit_builtin(t_cmd *cmd, t_cmd *list);
int	run_exit_builtin(t_cmd *cmd, t_cmd *list);



int	find_name_delate(char *targetname);

//env
// void	print_env(void);
int	print_env_export(void);
int	run_env_builtin(void);

//export
// t_env	*find_and_split(const char *s, char c, t_env *env);
// int	check_valid_identifier(char *text);
// int	write_identifier_error(char *text);
// void	add_env_tolast(t_env *temp_env);
int	run_export_builtin(t_cmd *cmd);

//unset
int	find_name_delate(char *targetname);
int	run_unset_builtin(t_cmd *cmd);



typedef struct s_temporary_array
{
	int		arrow;
	int		len;
	int		start;
	char	**temp_arr;
	int		count;
} t_tmp;

// MAKE COMMANDS
// t_cmd	*make_commands(t_source *src);
t_cmd	*make_commands(t_source *src);
// int		free_commands_list(t_cmd *first_cmd);
void	print_command_info(t_cmd *cmd);
void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t);
int		store_to_redirect_arr(t_source *src, t_cmd *cmd);
char	**realloc_array(char **arr, int count);

// UTILS
void	init_values(t_cmd *cmd);
int		check_if_builtin(t_cmd *cmd);
int		exec_builtin(t_cmd *cmd, t_cmd *list);
// int		exec_builtin(t_cmd *cmd, t_infos *info);
void	ft_lstadd_back(t_cmd **list, t_cmd *newnode);
int		get_length_of_word(t_source *src);


#define	MSG_SYNTAX "Syntax error"

// INITIAL CHECK
int	check_syntax_errors(t_source *src);
int	check_unsuported_chars(t_source *src);
int	check_pipes(t_source *src);
int	check_redirects(t_source *src);
int	check_quotes(t_source *src);
void skip_alphas_spaces_pipes(t_source *src);
int	is_space_alpha_or_pipe(t_source *src, int *c);
int	check_char_after_space(t_source *src, int *c);



# define SYNTAX_ERROR 	258
# define ENDOFLINE		(-1)
# define NOINPUT		(-1)

// UTILS //////////////////////////
int		is_allowed_char(int c);
int		is_valid_filename_char(int c);
void	print_err_msg(char *err_msg);
char	peek_next_char(t_source *src);
char	get_next_char(t_source *src);
void	skip_white_spaces(t_source *src);
void	print_err_msg(char *err_msg);
void	msg_and_exit(char *err_msg, int exit_code);



#endif