#include "../minishell.h"

char	*ft_find_env_passnum(char *envp[])
{
	int		i;
	char	*pass;

	i = 0;
	pass = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			pass = &envp[i][5];
			return (pass);
		}
		i++;
	}
	return (NULL);
}

char	*ft_make_binpass(int i, char *pass, char *cmd)
{
	int		j;
	int		k;
	char	*bin;

	j = 0;
	k = 0;
	bin = malloc(i + 2 + ft_strlen(cmd));
	if (bin == NULL)
		return (NULL);
	while (j < i)
	{
		bin[j] = pass[j];
		j++;
	}
	bin[j] = '/';
	j++;
	while (k < (int)ft_strlen(cmd))
	{
		bin[j + k] = cmd[k];
		k++;
	}
	bin[j + k] = '\0';
	return (bin);
}

char	*ft_findshell_pass(char *cmd, char *envp[])
{
	int		i;
	char	*pass;
	char	*bin;

	i = 0;
	pass = ft_find_env_passnum(envp);
	while (pass != NULL && pass[i] != '\0')
	{
		while (pass[i] != ':' && pass[i] != '\0')
			i++;
		if (pass[i] == ':' || pass[i] == '\0')
		{
			bin = ft_make_binpass(i, pass, cmd);
			if (bin == NULL)
				return (NULL);
			if (access(bin, X_OK) == 0)
				return (bin);
			free(bin);
			if (pass[i] == '\0')
				return (NULL);
			pass = &pass[i + 1];
			i = 0;
		}
	}
	return (NULL);
}
