/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/02 12:49:35 by kito          #+#    #+#                 */
/*   Updated: 2020/11/24 16:04:09 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	cc;
	int		i;

	cc = (char)c;
	i = 0;
	while (s[i] != '\0' || (c == '\0' && s[i] == '\0'))
	{
		if (s[i] == cc)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}
