#include "../minishell.h"

char	*make_malloc_str(char *text)
{
	int		i;
	char	*str;

	if (text == NULL)
		return (NULL);
	i = ft_strlen(text);
	str = malloc(i + 1);
	if (str == NULL)
		errtext_exit("malloc failed\n");
	str = ft_memcpy(str, text, i);
	str[i] = '\0';
	return (str);
}

char	*free_return_null(char *text)
{
	free(text);
	return (NULL);
}

char	*ft_add_c_free(char *s1, char c)
{
	char	*str;
	int		i;
	int		j;

	if (s1 == NULL)
		return (NULL);
	i = ft_strlen(s1);
	str = malloc(sizeof(char) * (i + 1 + 1));
	j = 0;
	if (!str)
		return (NULL);
	while (j < i)
	{
		str[j] = s1[j];
		j++;
	}
	str[j] = c;
	j++;
	str[j] = '\0';
	free(s1);
	return (str);
}

void	cleandata(t_infos *info)
{
	printf("exit\n");
	rl_clear_history();
	free_envlist(info);
	tcsetattr(0, 0, &info->termios_save);
}
