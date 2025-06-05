/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:27:52 by aayache           #+#    #+#             */
/*   Updated: 2025/06/05 01:15:33 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*creat_path2(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else if (ft_strncmp(cmd, "./", 2) == 0 && access(cmd, F_OK) == 0)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Permission denied\n", 21);
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*creat_path(char *cmd, char **path)
{
	int			i;
	char		*s;
	char		*tmp;
	struct stat	*sb;

	sb = gc_malloc(sizeof(struct stat));
	(1) && (ft_memset(sb, 0, sizeof(struct stat)), stat(cmd, sb));
	if (sb && S_ISDIR(sb->st_mode))
		return (NULL);
	(1) && (i = 0, s = NULL);
	if (!path && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (ft_strncmp(cmd, "./", 2) == 0 || cmd[0] == '/')
		return (creat_path2(cmd));
	if (!path)
		return (NULL);
	tmp = ft_strjoin(ft_strdup("/"), ft_strdup(cmd));
	while (path[i])
	{
		s = ft_strjoin(ft_strdup(path[i]), ft_strdup(tmp));
		if (access(s, X_OK) == 0)
			break ;
		(1) && (s = NULL, i++);
	}
	return (s);
}

char	*get_path(char **cmd, t_env **t_env1)
{
	char	**tmp;
	char	*path;
	char	*t_env;

	t_env = get_value(*t_env1, "PATH=");
	tmp = NULL;
	if (t_env)
		tmp = ft_split(t_env + 5, ':');
	path = creat_path(*cmd, tmp);
	return (path);
}

void	printerr2(char *cmd, int *ex)
{
	char		*s;
	struct stat	sb;

	*ex = 127;
	s = ": No such file or directory";
	if (stat(cmd, &sb) != -1 && S_ISDIR(sb.st_mode))
	{
		*ex = 126;
		s = ": Is a directory";

	}
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, s, ft_strlen(s));
	write(STDERR_FILENO, "\n", 1);
}

void	printerr(char *cmd, int *ex)
{
	char	*s;

	*ex = 127;
	if (ft_strrchr(cmd, '/'))
		printerr2(cmd, ex);
	else
	{
		s = ": command not found";
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, s, ft_strlen(s));
		write(STDERR_FILENO, "\n", 1);
	}
}
