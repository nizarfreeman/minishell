/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:29:41 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 16:48:23 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	search_replace(env *env, char *key, char *rep)
{
	char	*tmp;

	tmp = ft_strjoin(ft_strdup(key), ft_strdup("="));
	while (env && ft_strcmp(key, env->value) && ft_strncmp(env->value, tmp,
			ft_strlen(tmp)))
		env = env->next;
	if (!env)
		return ;
	if (rep)
	{
		env->value = NULL;
		env->value = ft_strjoin(tmp, rep);
	}
	env->f = 1;
}

char	*trim_last_dir(char *path)
{
	char	*last_slash;

	if (!ft_strcmp("/", path))
		return (ft_strdup("/"));
	last_slash = ft_strrchr(path, '/');
	free(path);
	return (word((char *)path, last_slash));
}

void	cd3(char *path, char **tmp, char **pwd, char *oldpwd)
{
	char	*pwdtmp;

	pwdtmp = getcwd(NULL, 0);
	*pwd = ft_strdup(pwdtmp);
	if (!*pwd)
	{
		*tmp = ft_strjoin(ft_strdup("/"), ft_strdup(path));
		write(2, "cd: error retrieving current directory: getcwd: ", 49);
		write(2, "cannot access parent ", 22);
		write(2, "directories: No such file or directory\n", 40);
		pwd = ft_strjoin(ft_strdup(oldpwd), *tmp);
	}
	free(pwdtmp);
	pwdtmp = NULL;
}
