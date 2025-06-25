/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:07:02 by aayache           #+#    #+#             */
/*   Updated: 2025/06/25 15:15:57 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	**lst_to_arr2(t_env *t_env)
{
	int		size;
	char	**ret;
	int		i;

	size = ft_lstsize(t_env) + 1;
	ret = gc_malloc(sizeof(char *) * size);
	i = 0;
	while (t_env)
	{
		if (t_env->f != 2)
		{
			ret[i] = ft_strdup(t_env->value);
			i++;
		}
		t_env = t_env->next;
	}
	ret[i] = NULL;
	return (ret);
}

char	**lst_to_arr(t_env *t_env)
{
	int		size;
	char	**ret;
	int		i;

	size = ft_lstsize(t_env) + 1;
	ret = gc_malloc(sizeof(char *) * size);
	i = 0;
	while (t_env)
	{
		if (t_env->f && t_env->f != 2)
		{
			ret[i] = ft_strdup(t_env->value);
			i++;
		}
		t_env = t_env->next;
	}
	ret[i] = NULL;
	return (ret);
}

int	expand_valid(char c)
{
	return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && c != '_'
		&& c != '?');
}

char	**expand_split(char **args)
{
	t_env	*ret;
	char	**tmp;

	ret = NULL;
	while (*args)
	{
		tmp = ft_split(*args, 0);
		while (tmp && *tmp)
		{
			ft_lstnew(&ret, *tmp, 0);
			tmp++;
		}
		args++;
	}
	return (lst_to_arr2(ret));
}

char	**expand(char **args, t_env *t_envr, int *ex)
{
	char	**tmp;

	tmp = args;
	while (tmp && *tmp)
	{
		repl1(' ', '\2', *tmp);
		tmp++;
	}
	tmp = args;
	while (*args)
	{
		*args = expand2(*args, t_envr, ex);
		if (!*args || !**args)
			*args = ft_strdup("");
		args++;
	}
	args = expand_split(tmp);
	tmp = args;
	while (tmp && *tmp)
	{
		repl1('\2', ' ', *tmp);
		repl1('\2', '\t', *tmp);
		tmp++;
	}
	return (args);
}
