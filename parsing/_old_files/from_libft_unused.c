#include "make_commands.h"


size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (!dest || !src)
		return (0);
	if (!size)
		return (strlen(src));
	i = 0;
	while (i < size - 1 && src[i] != '\0')
	{
		dest[i] = src[i];
		//printf(YEL"%zu[%c] "RES, i, dest[i]);
		i++;
	}
	dest[i] = '\0';
	//printf(YEL"   i%zu[%d] END\n"RES, i, dest[i]);
	// LOOKS LIKE IT IS FINE TO MALLOC LEN + 1
	return (strlen(src));
}
