/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:27:52 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 20:59:49 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*creat_path2(char *cmd, char **path)
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
	int		i;
	char	*s;
	char	*tmp;

	(1) && (i = 0, s = NULL);
	if (!path && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (ft_strncmp(cmd, "./", 2) == 0 || cmd[0] == '/')
		return (creat_path2(cmd, path));
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

char	*get_path(char **cmd, char **env)
{
	char	**tmp;
	char	*path;

	tmp = NULL;
	while (*env && ft_strncmp(*env, "PATH=", 5))
		env++;
	if (*env)
		tmp = ft_split(*env + 5, ':');
	path = creat_path(*cmd, tmp);
	return (path);
}

void	printerr(char *cmd, int i)
{
	char	*shell;
	char	*s;

	if (i || ft_strrchr(cmd, '/'))
	{
		s = ": No such file or directory";
		shell = "bash: ";
		write(STDERR_FILENO, shell, ft_strlen(shell));
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, s, ft_strlen(s));
		write(STDERR_FILENO, "\n", 1);
	}
	else
	{
		s = ": command not found";
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, s, ft_strlen(s));
		write(STDERR_FILENO, "\n", 1);
	}
}
