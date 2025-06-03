/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:07:02 by aayache           #+#    #+#             */
/*   Updated: 2025/06/03 15:44:09 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	**lst_to_arr2(env *env)
{
	int		size;
	char	**ret;
	int		i;

	size = ft_lstsize(env) + 1;
	ret = gc_malloc(sizeof(char *) * size);
	i = 0;
	while (env)
	{
		if (env->f != 2)
		{
			ret[i] = ft_strdup(env->value);
			i++;
		}
		env = env->next;
	}
	ret[i] = NULL;
	return (ret);
}

char	**lst_to_arr(env *env)
{
	int		size;
	char	**ret;
	int		i;

	size = ft_lstsize(env) + 1;
	ret = gc_malloc(sizeof(char *) * size);
	i = 0;
	while (env)
	{
		if (env->f)
		{
			ret[i] = ft_strdup(env->value);
			i++;
		}
		env = env->next;
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
	env		*ret;
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

char	**expand(char **args, env *envr, int *ex)
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
		*args = expand2(*args, envr, ex);
		if (!*args || !**args)
			*args = ft_strdup("");
		args++;
	}
	args = expand_split(tmp);
	tmp = args;
	while (tmp && *tmp)
	{
		repl1('\2', ' ', *tmp);
		tmp++;
	}
	return (args);
}
