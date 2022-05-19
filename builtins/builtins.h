#ifndef BUILTINS_H
#define BUILTINS_H

# include <sys/param.h>	// FOR PATH_MAX, JUST FOR LINUX
# include <limits.h>	// FOR PATH_MAX, JUST FOR MAC

//#include "../main.h"

//# include "../jaka_utils/utils.h"

# include "../minishell.h"


int	run_echo_builtin(t_cmd *cmd /*, t_infos *info */);

int	run_pwd_builtin(void);
int	run_cd_builtin(t_cmd *cmd, t_infos *info);
int	run_exit_builtin(t_cmd *cmd /*, t_infos *info */ , t_cmd *list);
int	run_export_builtin(t_cmd *cmd, t_infos *info);
int	find_name_delate(char *targetname, t_infos *info);
int	run_unset_builtin(t_cmd *cmd, t_infos *info);
int	run_env_builtin(t_infos *info);

// CD , can be removed
//int	store_old_pwd(void);




#endif
