/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:27:36 by aayache           #+#    #+#             */
/*   Updated: 2025/05/30 15:01:41 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	no_env(env **ret)
{
	char	*s;

	no_env2(ret);
	s = get_value(*ret, "OLDPWD");
	if (!s)
		ft_lstnew1(ret, "OLDPWD", 0);
	s = get_value(*ret, "_");
	if (!s)
		ft_lstnew1(ret, "_=/usr/bin/env", 2);
	else
		search_replace1(*ret, "_", NULL);
	s = get_value(*ret, "SHLVL");
	if (!s)
		ft_lstnew1(ret, "SHLVL=1", 1);
	else if (is_all_num(s + 1) && (int)ft_atoi(s + 1) >= 999)
	{
		printf("bash: warning: shell level (%d) too high, resetting to 1\n",
			(int)ft_atoi(s + 1)); // write err;
		search_replace1(*ret, "SHLVL", "1");
	}
	else
		search_replace1(*ret, "SHLVL", ft_itoa(ft_atoi(s + 1) + 1));
}

int	check_value(char *str)
{
	while (*str && *str != '=')
		str++;
	if (!*str)
		return (0);
	return (1);
}

env	*creat_env(char **envr)
{
	env	*tmp;
	env	*ret;

	ret = NULL;
	while (*envr)
	{
		ft_lstnew1(&ret, *envr, check_value(*envr));
		envr++;
	}
	no_env(&ret);
	return (ret);
}

void	remove_node(env **envrr, char *str)
{
	char	*tmp;
	env		*envr;
	env		*prev;

	envr = *envrr;
	tmp = ft_strjoin(ft_strdup(str), ft_strdup("="));
	if (envr && (!ft_strcmp(str, envr->value) || !ft_strncmp(envr->value, tmp,
				ft_strlen(tmp))))
	{
		*envrr = envr->next;
		return ;
	}
	while (envr && (ft_strncmp(envr->value, tmp, ft_strlen(tmp))
			&& ft_strcmp(envr->value, str)))
	{
		prev = envr;
		envr = envr->next;
	}
	if (!envr)
		return ;
	prev->next = envr->next;
}

int	unset(env **env, char **str, int *ex)
{
	if (!str)
	{
		*ex = 0;
		return (0);
	}
	while (*str)
	{
		if (ft_strcmp("_", *str))
			remove_node(env, *str);
		str++;
	}
	*ex = 0;
	return (0);
}
