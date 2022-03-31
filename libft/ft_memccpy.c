/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memccpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/31 06:13:25 by kito          #+#    #+#                 */
/*   Updated: 2020/12/01 20:56:37 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*newdist;
	unsigned char	*newsrc;
	unsigned char	newc;
	size_t			i;

	newdist = (unsigned char *)dst;
	newsrc = (unsigned char *)src;
	newc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		newdist[i] = newsrc[i];
		if (newsrc[i] == newc)
			return (dst + (unsigned char)(i + 1));
		i++;
	}
	return (NULL);
}
