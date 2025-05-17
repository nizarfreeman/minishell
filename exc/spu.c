#include "s.h"
void	repl1(char c, char b, char *s)
{
	while (*s)
	{
		if (*s == c)
			*s = b;
		s++;
	}
}

void	repl(char c, char b, char *s)
{
	int	f;

	while (*s && *s == c)
	{
		*s = b;
		s++;
	}
	f = ft_strlen(s) - 1;
	while (s[f] && s[f] == c)
	{
		s[f] = b;
		f--;
	}
	f = 0;
	while (*s)
	{
		if (*s == '\'')
			f++;
		if (*s == c && f != 0 && f % 2)
			*s = b;
		s++;
	}
}