/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 04:43:04 by aayache           #+#    #+#             */
/*   Updated: 2025/06/05 14:36:01 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	is_space_tab(char c)
{
	return (c == ' ' || c == '\t');
}

int	count_words_with_charset(char *str, char charset)
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

int	count_words_with_spaces(char *str)
{
	int	count;
	int	flag;

	flag = 1;
	count = 0;
	while (*str)
	{
		if (!is_space_tab(*str) && flag)
		{
			count++;
			flag = 0;
		}
		else if (is_space_tab(*str))
			flag = 1;
		str++;
	}
	return (count);
}

int	c_word(char *str, char charset)
{
	if (charset != 0)
		return (count_words_with_charset(str, charset));
	else
		return (count_words_with_spaces(str));
}

char	**ft_split(const char *s, char c)
{
	char	**split;
	int		a;
	char	*s1;

	if (!s)
		return (NULL);
	if (!*s)
	{
		split = gc_malloc(sizeof(char *) * 2);
		split[0] = ft_strdup("");
		split[1] = NULL;
		return (split);
	}
	s1 = ft_strdup(s);
	a = c_word(s1, c);
	split = gc_malloc((a + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split2(split, s1, c);
	return (split);
}
