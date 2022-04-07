/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/01 14:34:26 by kito          #+#    #+#                 */
/*   Updated: 2020/11/24 14:18:58 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	char	*newdist;
	size_t	i;
	size_t	j;

	newdist = dst;
	i = 0;
	j = ft_strlen(src);
	if (dstsize == 0 || dst == NULL)
		return (j);
	while (i < (dstsize - 1))
	{
		newdist[i] = src[i];
		if (src[i] == '\0')
			return (j);
		i++;
	}
	newdist[i] = '\0';
	return (j);
}
