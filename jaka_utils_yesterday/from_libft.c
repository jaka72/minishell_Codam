//#include "make_commands.h"
#include "utils.h"



// size_t	ft_strlcpy(char *dest, const char *src, size_t size)
// {
// 	size_t	i;

// 	if (!dest || !src)
// 		return (0);
// 	if (!size)
// 		return (strlen(src));
// 	i = 0;
// 	while (i < size - 1 && src[i] != '\0')
// 	{
// 		dest[i] = src[i];
// 		//printf(YEL"%zu[%c] "RES, i, dest[i]);
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	//printf(YEL"   i%zu[%d] END\n"RES, i, dest[i]);
// 	// LOOKS LIKE IT IS FINE TO MALLOC LEN + 1
// 	return (strlen(src));
// }



// int	ft_isalpha(int c)
// {
// 	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
// 		return (1);
// 	else
// 		return (0);
// }


// int	ft_isspace(int c)
// {
// 	c = (unsigned char)c;
// 	if (c == '\t' || c == '\n' || c == '\v'
// 		|| c == '\f' || c == '\r' || c == ' ')
// 		return (1);
// 	return (0);
// }


int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}


//////////////////

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