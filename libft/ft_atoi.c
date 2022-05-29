#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

static int	overflow(int sign, long long num, const char *str, int i)
{
	if (sign == -1 && str[i] > '8')
		return (0);
	if (sign == -1 && num > 922337203685477580)
		return (0);
	if (sign == 1 && str[i] > '7')
		return (-1);
	if (sign == 1 && num > 922337203685477580)
		return (-1);
	else
	{
		num = (num * 10) + (str[i] - '0');
		return (num * sign);
	}
}

int			ft_atoi(const char *str)
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
			return (overflow(sign, num, str, i));
		num = num * 10 + str[i] - '0';
		i++;
	}
	return (num * sign);
}