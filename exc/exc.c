/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:38:28 by aayache           #+#    #+#             */
/*   Updated: 2025/06/04 14:20:12 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int		g_han = 0;

char	*creat_word(char *str, int f, char c, int *p)
{
	char	*s;

	s = str;
	if (f)
	{
		if (*str == c)
		{
			*p = 1;
			return (ft_strdup(""));
		}
		while (*(s) != c)
		{
			s++;
			(*p)++;
		}
		(*p) = s - str + 1;
		return (word(str, s));
	}
	while (*(str) && *(str) != '\'' && *(str) != '"')
		str++;
	(*p) = str - s;
	return (word(s, str));
}

int	is_wildcard(char *s)
{
	char	tmp;

	while (*s)
	{
		if (*s == '\'' || *s == '"')
		{
			tmp = *s;
			s++;
			s = ft_strchr(s, tmp);
		}
		else if (*s == '*')
			return (1);
		s++;
	}
	return (0);
}

void	filter_first(t_env **files, t_env *arg)
{
	t_env	*tmp;
	t_env	*next;

	tmp = *files;
	if (!ft_strcmp(arg->value, ""))
		return ;
	while (tmp)
	{
		if (ft_strncmp(tmp->tmp, arg->value, ft_strlen(arg->value)))
		{
			next = tmp->next;
			remove_node(files, tmp->value);
			tmp = next;
		}
		else
		{
			tmp->tmp += ft_strlen(arg->value);
			tmp = tmp->next;
		}
	}
}

void	get_dir(t_env **ret)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	(entry = readdir(dir));
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
			ft_lstnew(ret, ft_strdup(entry->d_name), -1);
		(entry = readdir(dir));
	}
	closedir(dir);
}
