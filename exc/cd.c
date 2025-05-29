/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:52:27 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 16:47:52 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*get_value(env *env, char *key)
{
	while (env && ft_strcmp(env->value, key) && ft_strncmp(env->value, key,
			ft_strlen(key)))
		env = env->next;
	if (!env)
		return (NULL);
	return (ft_strdup(env->value + ft_strlen(key)));
}

int	cd_home(env *env, char **path, int *ex)
{
	char	*tmp;

	tmp = get_value(env, "HOME=");
	if (!tmp)
	{
		free(tmp);
		tmp = NULL;
		printf("cd: HOME not set\n");
		*ex = 1;
		return (1);
	}
	if (!*tmp)
	{
		free(tmp);
		tmp = NULL;
		*ex = 0;
		return (0);
	}
	*ex = cd2(env, tmp);
	return (*ex);
}

int	cd(env *env, char **path, int *ex)
{
	char	*tmp;

	if (*path && path[1])
	{
		write(2, "cd: too many arguments\n", 24);
		*ex = 1;
		return (1);
	}
	if (!*path || !**path)
		return (cd_home(env, path, ex));
	else
		*ex = cd2(env, *path);
	return (*ex);
}

int	cd_error(char *path, env *env)
{
	if (errno == EACCES)
	{
		if (!ft_strcmp(path, ".."))
			return (cd2(env, trim_last_dir(getcwd(NULL, 0))));
		else
			write(2, "cd: Permission denied.\n", 24);
	}
	else if (errno == ENOENT)
	{
		write(2, "cd: ", 4);
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 29);
	}
	else if (errno == ENOTDIR)
	{
		write(2, "cd: ", 4);
		write(2, path, ft_strlen(path));
		write(2, ": is not a directory.\n", 23);
	}
	else
		write(2, "cd: Failed to change directory\n", 31);
	return (1);
}

int	cd2(env *env, char *path)
{
	char	*pwd;
	char	*oldpwd;
	char	*tmp;
	char	*oldpwd1;

	oldpwd = get_value(env, "PWD=");
	if (!oldpwd)
		oldpwd1 = getcwd(NULL, 0);
	if (chdir(path) == 0)
	{
		cd3(path, &tmp, &pwd, oldpwd);
		search_replace(env, "PWD", pwd);
		if (oldpwd)
			search_replace(env, "OLDPWD", oldpwd);
		else
			search_replace(env, "OLDPWD", oldpwd1);
		free(oldpwd1);
		oldpwd1 = NULL;
		return (0);
	}
	return (cd_error(path, env));
}
