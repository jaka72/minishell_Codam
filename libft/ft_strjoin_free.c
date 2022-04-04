/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin_free.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 12:26:18 by kito          #+#    #+#                 */
/*   Updated: 2022/04/04 12:48:13 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;
	int		k;

	i = ft_strlen(s1);
	j = ft_strlen(s2);
	str = malloc(sizeof(char) * (i + j + 1));
	k = 0;
	if (!str)
		return (NULL);
	while (k < i)
	{
		str[k] = s1[k];
		k++;
	}
	while (k < (i + j))
	{
		str[k] = s2[k - i];
		k++;
	}
	str[k] = '\0';
	// if (s1 != NULL)
	// 	free(s1);
	// if (s2 != NULL)
	// 	free(s2);
	return (str);
}
