/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:31:51 by aayache           #+#    #+#             */
/*   Updated: 2025/06/04 14:20:12 by aayache          ###   ########.fr       */
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

char	*expand2_1(char *str, t_env *t_envr, int *ex)
{
	char	*ret;
	char	*s;

	s = NULL;
	ret = NULL;
	s = str;
	while (*str && *(str + 1) != '$')
		str++;
	if (*str && *(str + 1) == '$')
		ret = ft_strjoin(word(s, str + 1), expand2(str + 1, t_envr, ex));
	else
		ret = word(s, str);
	return (ret);
}

char	*expand2_2(char *str, t_env *t_envr, int *ex)
{
	char	*ret;
	char	*s;

	ret = NULL;
	if (expand_valid(*(str + 1)))
	{
		ret = ft_strjoin(ft_strdup("$"), expand2(str + 1, t_envr, ex));
		return (ret);
	}
	str++;
	if (*str == '?')
	{
		ret = ft_strjoin(ft_itoa(*ex), expand2(str + 1, t_envr, ex));
		return (ret);
	}
	else
	{
		s = str;
		while (*str && !expand_valid(*(str)))
			str++;
		ret = ft_strjoin(get_value(t_envr, ft_strjoin(word(s, str), "=")),
				expand2(str, t_envr, ex));
		return (ret);
	}
	return (NULL);
}

char	*expand2(char *str, t_env *t_envr, int *ex)
{
	if (*str != '$')
		return (expand2_1(str, t_envr, ex));
	if (*str == '$')
		return (expand2_2(str, t_envr, ex));
	return (NULL);
}
