/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcpy.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/30 09:09:17 by kito          #+#    #+#                 */
/*   Updated: 2020/11/24 14:42:03 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*newdist;
	unsigned char	*newsrc;
	size_t			i;

	newdist = (unsigned char *)dst;
	newsrc = (unsigned char *)src;
	i = 0;
	if (n == 0 || dst == src)
		return (dst);
	while (i < n)
	{
		newdist[i] = newsrc[i];
		i++;
	}
	return (dst);
}
