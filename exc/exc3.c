/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:31:51 by aayache           #+#    #+#             */
/*   Updated: 2025/06/28 17:13:10 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

t_env	*wildcar_split(char *s)
{
	t_env	*ret;

	ret = NULL;
	if (*s == '*')
	{
		ft_lstnew(&ret, ft_strdup(""), 0);
		while (*s && *s == '*')
			s++;
	}
	while (*s)
	{
		if (*s && (*s == '\'' || *s == '"' || *s != '*'))
			wildcar_split2(s, &s, &ret);
		else if (*s == '*' && !*(s + 1))
		{
			ft_lstnew(&ret, ft_strdup(""), 0);
			s++;
		}
		else
			s++;
	}
	return (ret);
}

int	expand_wildcard(char *s, t_env **ret)
{
	t_env	*args;
	t_env	*dir;
	t_env	*last;

	(1) && (args = wildcar_split(s), dir = NULL);
	get_dir(&dir);
	filter_first(&dir, args);
	(1) && (last = ft_lstlast(args), args = args->next);
	while (args && args != last)
	{
		filter_mid(&dir, args);
		args = args->next;
	}
	filter_last(&dir, last);
	if (!dir)
		return (0);
	else
	{
		while (dir)
		{
			ft_lstnew(ret, dir->value, 0);
			dir = dir->next;
		}
	}
	return (1);
}
