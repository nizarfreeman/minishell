/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 04:43:04 by aayache           #+#    #+#             */
/*   Updated: 2025/04/17 17:26:36 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

static int	c_word(char *str, char charset)
{
	int	count;
	int	flag;

	flag = 1;
	count = 0;
	while (*str)
	{
		if (*str != charset && flag)
		{
			count++;
			flag = 0;
		}
		else if (*str == charset)
			flag = 1;
		str++;
	}
	return (count);
}

char	*word(char *s, char *e)
{
	int		l;
	int		i;
	char	*p;

	l = e - s;
	p = malloc((l + 1) * sizeof(char));
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

static void	free_split(char **split, int i)
{
	int	j;

	j = i;
	while (i >= 0)
	{
		if (!split[i])
		{
			while (j >= 0)
			{
				free(split[j]);
				j--;
			}
			free(split);
			split = NULL;
			return ;
		}
		i--;
	}
}

static void	split2(char **split, char *str, char charset)
{
	char	*start;
	char	*end;
	int		i;

	start = NULL ;
	i = 0;
	while (*str)
	{
		if (*str != charset && !start)
			start = str;
		if ((*str == charset || *(str + 1) == '\0') && start)
		{
			if (*str == charset)
				end = str;
			else
				end = (str + 1);
			split[i++] = word(start, end);
			start = NULL;
		}
		str++;
	}
	split[i] = 0;
}

char	**ft_split(const char *s, char c)
{
	char	**split;
	int		a;
	char	*s1;

	if (!s)
		return (NULL);
	if (!*s)
		s = "\5";
	s1 = ft_strdup(s);
	// repl(' ', '\2', s1);
	// repl1('\'', ' ', s1);
	a = c_word(s1, c);
	split = malloc((a + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split2(split, s1, c);
	free_split(split, a - 1);
	// while (split && a--)
	// 	repl1('\2', ' ', split[a]);
	free(s1);
	return (split);
}
