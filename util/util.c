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
