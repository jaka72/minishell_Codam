#ifndef BUILTINS_H
#define BUILTINS_H

# include <sys/param.h>	// FOR PATH_MAX, JUST FOR LINUX
# include <limits.h>	// FOR PATH_MAX, JUST FOR MAC
# include "../minishell.h"

int	run_echo_builtin(t_cmd *cmd);
int	run_pwd_builtin(void);
int	run_cd_builtin(t_cmd *cmd);
int	print_msg_var_not_set(char *name);
int	print_error_too_many_args(void);
int	insert_oldpwd_into_list(t_env *env, const char *value);
// int	run_exit_builtin(t_cmd *cmd, t_cmd *list);
int	run_exit_builtin(t_cmd *cmd, t_cmd *list);
int	run_export_builtin(t_cmd *cmd);
int	run_unset_builtin(t_cmd *cmd);
int	run_env_builtin(void);
int	find_name_delate(char *targetname);

//env
int	print_env_export(void);
int	run_env_builtin();

//export
t_env	*find_and_split(const char *s, char c, t_env *env);
int	check_valid_identifier(char *text);
int	write_identifier_error(char *text);
void	add_env_tolast(t_env *temp_env);
int	run_export_builtin(t_cmd *cmd);

//unset
int	find_name_delate(char *targetname);
int	run_unset_builtin(t_cmd *cmd);

#endif
