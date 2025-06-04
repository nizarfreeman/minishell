/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:01:23 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/05 00:24:47 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax_check.h"

int	syntax_check(t_token *head)
{
	if (!check_start_end(head))
		return (0);
	if (!check_parentheses(head))
		return (0);
	if (!check_redirections(head))
		return (0);
	if (!check_operators(head))
		return (0);
	if (!check_heredoc(head))
		return (0);
	return (1);
}

static int	check_partner(char *s, char c, int i)
{
	i++;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	check_quotes(char *s)
{
	int	i;
	int	partner;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			partner = check_partner(s, s[i], i);
			if (partner == -1)
				return (0);
			i = partner + 1;
		}
		else
			i++;
	}
	return (1);
}
