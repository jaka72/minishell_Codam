#ifndef BUILTINS_H
#define BUILTINS_H

# include <sys/param.h>	// FOR PATH_MAX, JUST FOR LINUX
# include <limits.h>	// FOR PATH_MAX, JUST FOR MAC
# include "../minishell.h"

int	run_echo_builtin(t_cmd *cmd);
int	run_pwd_builtin(void);
int	run_cd_builtin(t_cmd *cmd, t_infos *info);
int	print_error_too_many_args(void);
int	insert_into_list(t_env *env, const char *value);
int	run_exit_builtin(t_cmd *cmd, t_cmd *list);
int	run_export_builtin(t_cmd *cmd, t_infos *info);
int	run_unset_builtin(t_cmd *cmd, t_infos *info);
int	run_env_builtin(t_infos *info);
int	find_name_delate(char *targetname, t_infos *info);

#endif
