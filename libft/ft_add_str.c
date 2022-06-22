/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_add_str.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:14:22 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
