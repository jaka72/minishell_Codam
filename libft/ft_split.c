/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: kito <kito@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/10 14:39:50 by kito          #+#    #+#                 */
/*   Updated: 2020/11/29 17:47:04 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	st_countd(const char *s, char c)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	if (c == '\0')
		return (1);
	if (s[0] != c)
		j++;
	if (s[0] == '\0')
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			j++;
		i++;
	}
	return (j);
}

static char	**st_putstr(char **da, char const *s, int d1, int d2)
{
	int	i;

	i = 0;
	da[d1] = malloc(sizeof(char) * (d2 + 2));
	if (!da[d1])
	{
		while (d1 > 0)
		{
			free(da[d1 - 1]);
			d1--;
		}
		free(da);
		return (NULL);
	}
	da[d1][d2 + 1] = '\0';
	i = 0;
	while (i <= d2)
	{
		da[d1][i] = s[i];
		i++;
	}
	return (da);
}

static char	**st_allocate(char const *s, char c, int j)
{
	char	**da;
	int		d1;
	int		d2;

	d1 = 0;
	d2 = 0;
	da = malloc(sizeof(da) * (j + 1));
	if (!da)
		return (NULL);
	while (s[d2] != '\0')
	{
		if (s[d2 + 1] == c || s[d2 + 1] == '\0')
		{
			da = st_putstr(da, s, d1, d2);
			d1++;
			while (s[d2 + 1] == c && c != '\0')
				d2++;
			s = &s[d2 + 1];
			d2 = 0;
		}
		else
			d2++;
	}
	da[d1] = NULL;
	return (da);
}

char		**ft_split(char const *s, char c)
{
	char	**src;
	int		i;
	int		j;

	if (s == NULL)
		return (NULL);
	j = st_countd(s, c);
	if (j == 0)
	{
		src = (char **)malloc(sizeof(src) * 1);
		if (!src)
			return (NULL);
		src[0] = NULL;
		return (src);
	}
	i = 0;
	if (s[0] == c)
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
	}
	s = &s[i];
	src = st_allocate(s, c, j);
	return (src);
}
