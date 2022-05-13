#include "builtins.h"

int	run_pwd_builtin(void)
{
	char buff[PATH_MAX];
	char *current_path;

	current_path = getcwd(buff, PATH_MAX);
	if (current_path == NULL)
		return (1);
	else
	{
		printf("%s\n", current_path);
		return (0);
	}
}
