#include "s.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 && *s2 && (char)*s1 == (char)*s2)
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return ((char)*s1 - (char)*s2);
}

char	*ft_strrchr(const char *s, int c)
{
	char	*tmp;

	tmp = NULL;
	while (*s)
	{
		if (*s == (char)c)
			tmp = (char *)s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (tmp);
}

// size_t	ft_strlen(const char *s)
// {
// 	const char	*t;

// 	if (!s)
// 		return (0);
// 	t = s;
// 	while (*t)
// 		t++;
// 	return ((size_t)(t - s));
// }

// char	*ft_strjoin(char *s1, char *s2)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*ret;

// 	if (s1 == NULL && s2 == NULL)
// 		return (NULL);
// 	i = ft_strlen(s1);
// 	j = ft_strlen(s2);
// 	ret = malloc(j + i + 1);
// 	if (ret == NULL)
// 	{
// 		free(s1);
// 		free(s2);
// 		return (NULL);
// 	}
// 	ft_memcpy(ret, s1, i);
// 	ft_memcpy(ret + i, s2, j);
// 	ret[i + j] = 0;
// 	free(s1);
// 	free(s2);
// 	return (ret);
// }

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*s;
	unsigned char	*d;

	if (dst == (void *)0 && src == (void *)0)
		return (dst);
	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	while (n)
	{
		*(d++) = *(s++);
		n--;
	}
	return (dst);
}
