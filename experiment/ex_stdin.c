#include "../minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	int	i = 0;
	// int	pid = 0;
	char  **cmd;

	(void) argc;
	(void) argv;
	cmd = ft_split("cat", ' ');
	printf("%d\n", STDIN_FILENO);
	i = open("file8", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	// dup2(i, 0);
	printf("%d\n", STDIN_FILENO);
	execve("/bin/cat", cmd, envp);
	free(cmd);
	return (0);
}