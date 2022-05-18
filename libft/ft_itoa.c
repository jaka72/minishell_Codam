/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/11 13:41:48 by kito          #+#    #+#                 */
/*   Updated: 2020/11/11 13:41:48 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	st_sign(int n)
{
	if (n < 0)
		return (-1);
	else
		return (1);
}

static int	st_min(int n)
{
	if (n == -2147483648)
		return (1);
	else
		return (0);
}

static int	st_digit(int n)
{
	int	i;
	int	j;

	if (n == 0 || n == -0)
		return (1);
	i = (n + st_min(n)) * st_sign(n);
	j = 0;
	while (i > 0)
	{
		i = i / 10;
		j++;
	}
	j = j + ((st_sign(n) - 1) * -1 / 2);
	return (j);
}

char	*ft_itoa(int n)
{
	int		i;
	int		j;
	char	*str;

	i = (n + st_min(n)) * st_sign(n);
	j = st_digit(n);
	str = malloc(sizeof(char) * (j + 1));
	if (!str)
		return (NULL);
	str[j] = '\0';
	j--;
	while (j >= ((st_sign(n) - 1) * -1 / 2))
	{
		if (str[j + 1] == '\0' && st_min(n) == 1)
			str[j] = '0' + 8;
		else
			str[j] = '0' + (i % 10);
		i = i / 10;
		j--;
	}
	if (st_sign(n) == -1)
		str[0] = '-';
	return (str);
}
