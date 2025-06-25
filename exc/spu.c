/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:23:42 by aayache           #+#    #+#             */
/*   Updated: 2025/06/25 17:12:55 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*word(char *s, char *e)
{
	int		l;
	int		i;
	char	*p;

	l = e - s;
	p = gc_malloc((l + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (i < l)
	{
		p[i] = s[i];
		i++;
	}
	p[l] = '\0';
	return (p);
}

void	repl1(char c, char b, char *s)
{
	char	t;

	t = '\t';
	while (s && *s)
	{
		if (*s == c)
			*s = b;
		else if (c == ' ' && *s == t)
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
