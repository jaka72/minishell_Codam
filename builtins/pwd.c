#include "../minishell.h"
#include <limits.h>

int	run_pwd_builtin(void)
{
	char	buff[PATH_MAX];
	char	*current_path;

	current_path = getcwd(buff, PATH_MAX);
	if (current_path == NULL)
		return (1);
	else
	{
		write(1, current_path, ft_strlen(current_path));
		write(1, "\n", 1);
		return (0);
	}
}
