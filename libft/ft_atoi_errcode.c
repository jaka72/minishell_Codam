#include "libft.h"

int	ft_atoi_errcode(const char *str)
{
	int			i;
	int			sign;
	long long	num;

	i = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	num = 0;
	while (str[i] && ft_isdigit(str[i]))
	{
		if (num >= 922337203685477580)
			return (255);
		num = num * 10 + str[i] - '0';
		i++;
	}
	return (num * sign);
}
