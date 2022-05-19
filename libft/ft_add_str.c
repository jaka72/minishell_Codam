#include "libft.h"

char	**ft_add_str(char **str)
{
	char	**temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
		i++;
	temp = malloc(sizeof(char **) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while (str[j])
	{
		temp[j] = str[j];
		j++;
	}
	temp[i] = NULL;
	temp[i + 1] = NULL;
	free(str);
	return (temp);
}
