/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:52:27 by aayache           #+#    #+#             */
/*   Updated: 2025/06/21 19:42:14 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*get_value(t_env *t_env, char *key, int f)
{
	char	*t;

	if (f)
	{
		t = ft_strjoin(key, "=");
		while (t_env && (ft_strcmp(t_env->value, key)
				&& ft_strncmp(t_env->value, t, ft_strlen(t))))
			t_env = t_env->next;
		if (!t_env)
			return (NULL);
		if (!ft_strncmp(t_env->value, t, ft_strlen(t)))
			return (ft_strdup(t_env->value + ft_strlen(t)));
		return (ft_strdup(t_env->value + ft_strlen(key)));
	}
	while (t_env && ft_strcmp(t_env->value, key) && ft_strncmp(t_env->value,
			key, ft_strlen(key)))
		t_env = t_env->next;
	if (!t_env)
		return (NULL);
	return (ft_strdup(t_env->value + ft_strlen(key)));
}

int	cd_home(t_env *t_env, int *ex)
{
	char	*tmp;

	tmp = get_value(t_env, "HOME=", 0);
	if (!tmp)
	{
		tmp = NULL;
		write(2, "cd: HOME not set\n", 18);
		*ex = 1;
		return (1);
	}
	if (!*tmp)
	{
		tmp = NULL;
		*ex = 0;
		return (0);
	}
	*ex = cd2(t_env, tmp);
	return (*ex);
}

int	cd(t_env *t_env, char **path, int *ex)
{
	if (*path && path[1])
	{
		write(2, "cd: too many arguments\n", 24);
		*ex = 1;
		return (1);
	}
	if (!*path || !**path)
		return (cd_home(t_env, ex));
	else
		*ex = cd2(t_env, *path);
	return (*ex);
}

int	cd_error(char *path, t_env *t_env)
{
	if (errno == EACCES)
	{
		if (!ft_strcmp(path, ".."))
			return (cd2(t_env, trim_last_dir(getcwd(NULL, 0))));
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

int	cd2(t_env *t_env, char *path)
{
	char	*pwd;
	char	*oldpwd;
	char	*tmp;
	char	*oldpwd1;

	oldpwd1 = NULL;
	oldpwd = get_value(t_env, "PWD=", 0);
	if (!oldpwd)
		oldpwd1 = getcwd(NULL, 0);
	if (chdir(path) == 0)
	{
		cd3(path, &tmp, &pwd, oldpwd);
		search_replace1(t_env, "PWD", pwd);
		if (oldpwd)
			search_replace1(t_env, "OLDPWD", oldpwd);
		else
			search_replace1(t_env, "OLDPWD", oldpwd1);
		free(oldpwd1);
		oldpwd1 = NULL;
		return (0);
	}
	free(oldpwd1);
	return (cd_error(path, t_env));
}
