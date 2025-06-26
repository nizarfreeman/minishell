/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:05:53 by aayache           #+#    #+#             */
/*   Updated: 2025/06/26 21:51:36 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	check_value_exit(char *cmd)
{
	if (!*cmd)
		return (1);
	if (*cmd == '-' || *cmd == '+')
		cmd++;
	if (!*cmd)
		return (1);
	while (*cmd == ' ')
		cmd++;
	while (*cmd && *cmd >= '0' && *cmd <= '9')
		cmd++;
	while (*cmd == ' ')
		cmd++;
	if (!*cmd)
		return (0);
	return (1);
}

void	my_exit(char **cmd, int *ex)
{
	if (cmd && !*cmd)
	{
		write(1, "exit\n", 5);
		exit(*ex);
	}
	if (!check_value_exit(*cmd) && cmd[1])
	{
		write(2, "exit: too many arguments\n", 26);
		*ex = 1;
		return ;
	}
	if (check_value_exit(*cmd) || ft_atoi2(*cmd))
	{
		(write(1, "exit\n", 5), write(2, "exit: ", 7) && (1));
		write(2, *cmd, ft_strlen(*cmd));
		write(2, ": numeric argument required\n", 29);
		*ex = 2;
		exit(2);
	}
	if (!check_value_exit(*cmd) && !cmd[1])
	{
		*ex = ft_atoi(*cmd) % 256;
		write(1, "exit\n", 5);
		exit(ft_atoi(*cmd) % 256);
	}
}

char	*ft_strchr(char *s, char c)
{
	if (!s)
		return (NULL);
	while (*s && *s != c)
		s++;
	if (*s != c)
		return (NULL);
	return (s);
}

int	is_all_num(char *s)
{
	if (!s)
		return (0);
	while (*s >= '0' && *s <= '9')
		s++;
	if (!*s)
		return (1);
	return (0);
}

void	no_t_env2(t_env **ret)
{
	char	*s;

	s = get_value(*ret, "OLDPWD", 1);
	if (!s)
		ft_lstnew1(ret, "OLDPWD", 0);
	s = get_value(*ret, "PATH=", 0);
	if (!s)
		ft_lstnew1(ret,
			"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
			2);
	s = get_value(*ret, "PWD", 1);
	if (!s)
	{
		s = getcwd(NULL, 0);
		ft_lstnew1(ret, ft_strjoin(ft_strdup("PWD="), s), 1);
		free(s);
		s = NULL;
	}
	else
	{
		s = getcwd(NULL, 0);
		search_replace1(*ret, "PWD", s);
		free(s);
		s = NULL;
	}
}
