/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:27:52 by aayache           #+#    #+#             */
/*   Updated: 2025/06/15 17:35:05 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*creat_path2(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else if (!access(cmd, F_OK) && access(cmd, X_OK))
	{
		*(get_exit_status(NULL)) = 126;
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Permission denied\n", 21);
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*creat_path3(char **path, char *tmp)
{
	char	*s;
	int		i;

	i = 0;
	while (path[i])
	{
		s = ft_strjoin(ft_strdup(path[i]), ft_strdup(tmp));
		if (!access(s, F_OK) && access(s, X_OK))
		{
			*(get_exit_status(NULL)) = 126;
			write(2, s, ft_strlen(s));
			write(2, ": Permission denied\n", 21);
			return (ft_strdup(""));
		}
		if (access(s, X_OK) == 0)
			break ;
		(1) && (s = NULL, i++);
	}
	return (s);
}

char	*creat_path(char *cmd, char **path)
{
	char		*tmp;
	struct stat	*sb;

	sb = gc_malloc(sizeof(struct stat));
	ft_memset(sb, 0, sizeof(struct stat));
	stat(cmd, sb);
	if (sb && S_ISDIR(sb->st_mode))
		return (NULL);
	if ((!path || !*path || !**path) && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (ft_strncmp(cmd, "./", 2) == 0 || ft_strchr(cmd, '/'))
		return (creat_path2(cmd));
	if (!path)
		return (NULL);
				
	tmp = ft_strjoin(ft_strdup("/"), ft_strdup(cmd));
	return (creat_path3(path, tmp));
}

char	*get_path(char **cmd, t_env **t_env1)
{
	char	**tmp;
	char	*path;
	char	*t_env;

	t_env = get_value(*t_env1, "PATH=");
	tmp = NULL;
	if (t_env)
		tmp = ft_split(t_env, ':');
	path = creat_path(*cmd, tmp);
	return (path);
}
