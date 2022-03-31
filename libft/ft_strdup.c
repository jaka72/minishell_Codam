/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 13:18:53 by kito          #+#    #+#                 */
/*   Updated: 2020/11/17 19:50:46 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*newstr;
	int		size;
	int		i;

	size = ft_strlen(s1);
	i = 0;
	newstr = (char *)malloc(size + 1);
	if (!newstr)
		return (NULL);
	while (i < size)
	{
		newstr[i] = s1[i];
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}
