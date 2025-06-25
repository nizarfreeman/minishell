/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:23:15 by aayache           #+#    #+#             */
/*   Updated: 2025/06/12 15:11:00 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	free_split(char **split, int i)
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

static void	split_with_charset(char **split, char *str, char charset)
{
	char	*start;
	char	*end;
	int		i;

	start = NULL;
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

static void	split_with_spaces(char **split, char *str)
{
	char	*start;
	char	*end;
	int		i;

	start = NULL;
	i = 0;
	while (*str)
	{
		if (!is_space_tab(*str) && !start)
			start = str;
		if ((is_space_tab(*str) || *(str + 1) == '\0') && start)
		{
			if (is_space_tab(*str))
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

void	split2(char **split, char *str, char charset)
{
	if (charset != 0)
		split_with_charset(split, str, charset);
	else
		split_with_spaces(split, str);
}
