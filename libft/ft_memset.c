/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/27 15:59:51 by kito          #+#    #+#                 */
/*   Updated: 2020/11/24 14:34:52 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*newb;
	unsigned char	newc;
	size_t			i;

	newb = (unsigned char *)b;
	newc = (unsigned char)c;
	i = 0;
	while (i < len)
	{
		newb[i] = newc;
		i++;
	}
	return (b);
}
