/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:08:47 by aayache           #+#    #+#             */
/*   Updated: 2025/06/20 16:15:43 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	check_signal(int *status)
{
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
		{
			if (isatty(STDIN_FILENO))
				write(2, "Quit (core dumped)\n", 20);
			*status = 131;
			return (*status);
		}
		else if (WTERMSIG(*status) == SIGINT)
		{
			if (isatty(STDIN_FILENO))
				write(2, "\n", 1);
			*status = 130;
			return (*status);
		}
	}
	*status = WEXITSTATUS(*status);
	return (*status);
}

int	excute_cmd(char **cmd, t_env **t_env, int fd_in, int *status)
{
	char	**t_envr;
	char	*path;
	pid_t	pid;

	(1) && (t_envr = lst_to_arr(*t_env), path = get_path(cmd, t_env));
	if (!path)
		printerr(cmd[0], status);
	pid = ft_fork(fd_in, -1);
	if (pid == 0)
	{
		if (fd_in != -1)
		{
			ft_dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		g_han = 0;
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (!path || execve(path, cmd, t_envr) == -1)
			exit(*status);
	}
	if (fd_in != -1)
		close(fd_in);
	wait(status);
	return (check_signal(status));
}

void	wildcar_split2(char *s, char **src, t_env **ret)
{
	char	*tmp;
	int		p;

	if (*s == '\'' || *s == '"')
	{
		tmp = creat_word(s + 1, 1, *s, &p);
		s++;
		if (ft_strcmp("", tmp))
			ft_lstnew(ret, tmp, 0);
		s += p;
	}
	else if (*s && *s != '*')
	{
		tmp = s;
		while (*s && *s != '*')
			s++;
		ft_lstnew(ret, word(tmp, s), 0);
	}
	*src = s;
}
