#include "../minishell.h"

int	run_env_builtin(t_infos *info)
{
	print_env(info);
	return (0);
}
