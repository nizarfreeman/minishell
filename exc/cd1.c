/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:27:55 by aayache           #+#    #+#             */
/*   Updated: 2025/06/01 12:41:06 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	pwd(env *env, int *ex)
{
	char	*s;
	char *tmp;
	s = getcwd(NULL, 0);
	if (s)
		printf("%s\n", s);
	else
	{
		tmp = get_value(env, "PWD=");
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
	int	opt;

	opt = echo_option(*str);
	while (!echo_option(*str))
		str++;
	while (*str)
	{
		if (**str)
		{
			write(1, *str, ft_strlen(*str));
			if (*(str + 1))
				write(1, " ", 1);
		}
		str++;
	}
	if (opt)
		printf("\n");
	*ex = 0;
	return (0);
}

int	envr(env *env, int *ex)
{
	while (env)
	{
		if ((env->f && env->f != 2) || !ft_strcmp("_=/usr/bin/env", env->value))
			printf("%s\n", env->value);
		env = env->next;
	}
	*ex = 0;
	return (0);
}

void	replace_create(env *env, char *key, char *rep)
{
	char	*tmp;

	tmp = ft_strjoin(ft_strdup(key), ft_strdup("="));
	while (env && ft_strcmp(key, env->value) && ft_strncmp(env->value, key,
			ft_strlen(key)))
		env = env->next;
	if (!env)
	{
		free(tmp);
		return ;
	}
	free(env->value);
	env->value = NULL;
	env->value = ft_strjoin(tmp, rep);
	env->f = 1;
}
