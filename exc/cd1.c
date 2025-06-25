/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:27:55 by aayache           #+#    #+#             */
/*   Updated: 2025/06/21 14:30:35 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	pwd(t_env *t_env, int *ex)
{
	char	*s;
	char	*tmp;

	s = getcwd(NULL, 0);
	if (s)
		printf("%s\n", s);
	else
	{
		tmp = get_value(t_env, "PWD=", 0);
		if (tmp)
			printf("%s\n", tmp);
	}
	free(s);
	s = NULL;
	*ex = 0;
	return (0);
}

int	echo_option(char *str)
{
	if (!str)
		return (1);
	if (*str != '-')
		return (1);
	str++;
	if (!*str)
		return (1);
	while (*str && *str == 'n')
		str++;
	if (!*str)
		return (0);
	return (1);
}

int	echo(char **str, int *ex)
{
	int		opt;
	char	*p;

	opt = echo_option(*str);
	while (!echo_option(*str))
		str++;
	p = NULL;
	while (*str)
	{
		if (**str)
		{
			p = ft_strjoin(p, *str);
			if (*(str + 1))
				p = ft_strjoin(p, " ");
		}
		str++;
	}
	if (opt)
		p = ft_strjoin(p, "\n");
	write(1, p, ft_strlen(p));
	*ex = 0;
	return (0);
}

int	t_envr(t_env *t_env, int *ex)
{
	while (t_env)
	{
		if ((t_env->f && t_env->f != 2) || !ft_strcmp("_=/usr/bin/env",
				t_env->value))
			printf("%s\n", t_env->value);
		t_env = t_env->next;
	}
	*ex = 0;
	return (0);
}

void	replace_create(t_env *t_env, char *key, char *rep)
{
	char	*tmp;

	tmp = ft_strjoin(ft_strdup(key), ft_strdup("="));
	while (t_env && ft_strcmp(key, t_env->value) && ft_strncmp(t_env->value,
			key, ft_strlen(key)))
		t_env = t_env->next;
	if (!t_env)
	{
		free(tmp);
		return ;
	}
	free(t_env->value);
	t_env->value = NULL;
	t_env->value = ft_strjoin(tmp, rep);
	t_env->f = 1;
}
