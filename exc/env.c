/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:22:00 by aayache           #+#    #+#             */
/*   Updated: 2025/06/15 16:52:55 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	no_t_env(t_env **ret)
{
	char	*s;

	no_t_env2(ret);
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
		write(2, "bash: warning: shell level (", 29);
		write(2, ft_itoa(ft_atoi(s + 1) + 1), ft_strlen(ft_itoa(ft_atoi(s + 1)
					+ 1)));
		write(2, ") too high, resetting to 1\n", 28);
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

t_env	*creat_env(char **t_envr)
{
	t_env	*ret;

	ret = NULL;
	while (*t_envr)
	{
		ft_lstnew1(&ret, *t_envr, check_value(*t_envr));
		t_envr++;
	}
	no_t_env(&ret);
	return (ret);
}

void	remove_node(t_env **t_envrr, char *str)
{
	char	*tmp;
	t_env	*t_envr;
	t_env	*prev;

	t_envr = *t_envrr;
	tmp = ft_strjoin(str, "=");
	if (t_envr && t_envr->value && (!ft_strcmp(str, t_envr->value)
			|| !ft_strncmp(t_envr->value, tmp, ft_strlen(tmp))))
	{
		*t_envrr = t_envr->next;
		return ;
	}
	while (t_envr && (ft_strncmp(t_envr->value, tmp, ft_strlen(tmp))
			&& ft_strcmp(t_envr->value, str)))
	{
		prev = t_envr;
		t_envr = t_envr->next;
	}
	if (!t_envr)
		return ;
	prev->next = t_envr->next;
}

int	unset(t_env **t_env, char **str, int *ex)
{
	if (!str)
	{
		*ex = 0;
		return (0);
	}
	while (*str)
	{
		if (ft_strcmp("_", *str))
			remove_node(t_env, *str);
		str++;
	}
	*ex = 0;
	return (0);
}
