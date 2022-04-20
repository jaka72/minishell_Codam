#ifndef BUILTINS_H
#define BUILTINS_H

# include <sys/param.h>	// FOR PATH_MAX, JUST FOR LINUX
# include <limits.h>	// FOR PATH_MAX, JUST FOR MAC

//#include "../main.h"

# include "../jaka_utils/utils.h"

# include "../minishell.h"


int	run_echo_builtin(t_cmd *cmd);
//int	echo_builtin(t_source *src);


#endif
