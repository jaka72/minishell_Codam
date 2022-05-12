#include "../minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	int	i = 0;
	int	f = 0;
	pid_t	pid;
	char  **cmd;

	(void) argc;
	(void) argv;
	cmd = ft_split("cat", ' ');
	printf("%d\n", STDIN_FILENO);
	pid = fork();
	if (pid == 0)
	{
		i = open("test", O_RDONLY, 0666);
		f = open("outputfile",
						O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(i, 0);
		dup2(f, 1);
		close(i);
		close(f);
		printf("%d\n", STDIN_FILENO);
		execve("/bin/cat", cmd, envp);
	}
	printf("end of grm\n");
	free(cmd);
	return (0);
}