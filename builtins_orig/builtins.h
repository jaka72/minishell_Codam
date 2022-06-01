#ifndef BUILTINS_H
#define BUILTINS_H

# include <sys/param.h>	// FOR PATH_MAX, JUST FOR LINUX
# include <limits.h>	// FOR PATH_MAX, JUST FOR MAC

# include "../minishell.h"

int	run_echo_builtin(t_cmd *cmd);
int	run_pwd_builtin(void);
int	run_cd_builtin(t_cmd *cmd, t_infos *info);
int	run_exit_builtin(t_cmd *cmd);
int	run_export_builtin(t_cmd *cmd, t_infos *info);
int	run_unset_builtin(t_cmd *cmd, t_infos *info);
int	run_env_builtin(t_infos *info);
int	find_name_delate(char *targetname, t_infos *info);

#endif
