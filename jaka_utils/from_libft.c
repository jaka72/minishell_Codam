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
