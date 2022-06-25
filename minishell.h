/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 17:09:21 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <termcap.h>
# include <termios.h>
# include "libft/libft.h"
# include "typedef.h"

// util/error.c
int		return_perr(int i, char *tx);
int		return_errtx(int i, char *tx);
int		err_return_num(int exit_code, char *tx);
void	print_err_msg(char *err_msg);
void	msg_and_exit(char *err_msg, int exit_code);
// void	err_free_env_exit(char *text);
// int		err_all_free_exit(int exitnum);
// int		errtx_all_free_exit(int exitnum, char *tx);

// util/free.c
void	free_envlist(t_util *st_base);
int		free_strings(char **strs);
void	free_tcmd(t_util *st_base);

// util/util.c
char	*free_return_null(char *text);
char	*ft_add_c_free(char *s1, char c);
void	clean_fd(t_util *st_base);
int		clean_data(int status, char *text, t_util *st_base);
int		wait_return(void);

//check_syntax/check_syntax_redirects_utils.c
int		is_space_alpha_or_pipe(t_source *src, int *c);
int		check_char_after_space(t_source *src, int *c);

//check_syntax/check_syntax_redirects.c
int		check_redirects(t_source *src);

//check_syntax/check_syntax_quotes.c
int		check_quotes(t_source *src);

//check_syntax/check_syntax_pipes.c
int		check_pipes(t_source *src);

//check_syntax/check_syntax.c
int		check_syntax_errors(t_source *src, int *stat);

// parsing_util/parsing_utils_err.c
void	ft_lstadd_back(t_cmd **list, t_cmd *newnode);
char	**realloc_array(char **arr, int count);

// parsing_util/parsing_utils.c
int		count_elems(char **arr);
int		is_allowed_char(int c);
int		is_valid_filename_char(int c);
char	peek_next_char(t_source *src);
void	skip_white_spaces(t_source *src);

// parsing/choose_correct_array.c
void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t);

// parsing/store_to_redirect_arr.c
int		store_to_redirect_arr(t_source *src, t_cmd *cmd);

// parsing/make_commands_utils.c
void	init_values(t_cmd *cmd);
int		check_if_builtin(t_cmd *cmd);
int		exec_builtin(t_cmd *cmd, t_cmd *list, t_util *st_base);
int		get_length_of_word(t_source *src);

// parsing/make_commands.c
t_cmd	*make_commands(t_source *src);

// init/signal_handle.c
void	handle_sigint(int num);
void	handle_sigquit(int num);
void	handle_sigint_p(int num);
void	handle_sigquit_p(int num);
void	handle_sigint_hd(int num);

// init/init.c
void	ms_init(char *envp[], int *ex_stat, t_util *st_base, t_source *src);

// init/env_util.c
t_env	*last_env(t_util *st_base);
t_env	*get_name_value(t_env *env, char *envtext);
t_env	*init_tempenv(void);

// init/env.c
t_env	*get_env(char *envp[], t_util *st_base);
char	**get_env_array(t_util *st_base);

// init/expand_util_1.c
char	*name_expand(char *tx, t_util *st_base);
char	*add_singlequote(char *dst, char *src);
int		count_single_length(char *src);
int		count_expand_length(char *src);
char	*show_last_status(char *expanded, int *ex_stat, int *i);

// init/expand_util_2.c
char	*add_expanded(char *dst, char *src, t_util *st_base);
char	*add_two_cha(char *dst, char a, char b, int *i);
char	*add_laststatus(char *dst, int *ex_stat);
char	*ini_expanded(void);
char	*check_expand_hd(char *tx, t_util *st_base);

// init/expand.c
char	*check_expand_file(char *tx, int *ex_stat, t_util *st_base);
char	*check_expand(char *tx, int *ex_stat, t_util *st_base);
char	**expand_array(char **args, int *ex_stat, t_util *st_base);

// heredoc/heredoc_util.c
void	init_pid_sig_hd(t_pid *pidinfo, t_util *st_base);
int		reset_fd_sig_close(int fd, t_util *st_base);

// heredoc/heredoc.c
int		open_heredoc(t_pid *pid, int *ex_stat, t_util *st_base);

// exec/open_fd.c
int		open_file_fd(t_cmd *str);

// exec/connect_fd.c
int		connect_fd(t_cmd *current);
void	check_close_fd(int fdin, int fdout);
int		reset_fd_sig(t_util *st_base);

// exec/find_path.c
char	*ft_findshell_path(char *cmd, char *envp[]);

// exec/check_files.c
int		check_file_access(t_cmd	*current, int *ex_stat, t_util *st_base);

// exec/exec.c
int		run_cmd(int *ex_stat, t_util *st_base);

// exec/ms_execve.c
int		ms_execve(t_cmd *str, t_util *st_base);

// main.c
int		main(int argc, char *argv[], char *envp[]);

// builtin/cd_util.c
char	*get_path(char *name, int *ret, int n, t_util *st_base);
int		print_msg_var_not_set(char *name);
int		insert_oldpwd_into_list(t_env *env, const char *value);

// builtin/cd.c
int		run_cd_builtin(t_cmd *cmd, t_util *st_base);

// builtin/echo.c
int		run_echo_builtin(t_cmd *cmd);

// builtin/env.c
int		print_env_export(t_util *st_base);
int		run_env_builtin(t_util *st_base);
void	add_env_tolast(t_env *temp_env, t_util *st_base);

// builtin/exit.c
int		run_exit_builtin(t_cmd *cmd, t_cmd *list);

// builtin/export.c
int		run_export_builtin(t_cmd *cmd, t_util *st_base);

// builtin/pwd.c
int		run_pwd_builtin(t_util *st_base);

// builtin/unset.c
int		find_name_delate(char *targetname, t_util *st_base);
int		run_unset_builtin(t_cmd *cmd, t_util *st_base);

#endif