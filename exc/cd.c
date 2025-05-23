#include "s.h"
char *get_value(env *env, char *key)
{
	while (env && ft_strcmp(env->value, key) && ft_strncmp(env->value, key, ft_strlen(key)))
		env = env->next;
	if (!env)
		return (NULL);
	return (ft_strdup(env->value + ft_strlen(key)));
}
int pwd(env *env, int *ex)
{
	char *s;

	s = getcwd(NULL, 0);
	if (s)
		printf("%s\n", s);
	else
	
	{
		s = get_value(env, "PWD=");
		printf("%s\n", s);
	}

	free(s);
	s = NULL;
	*ex = 0;
	return 0;
}
int echo_option(char *str)
{
	if (!str)
		return 1;
	if (*str != '-')
		return 1;
	str++;
	if (!*str)
		return 1;
	while (*str && *str == 'n')
		str++;
	if (!*str)
		return 0;
	return 1;
}

int echo(char **str, int *ex)
{

	int opt = echo_option(*str);
	while(!echo_option(*str))
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
	if(opt)
		printf("\n");
	*ex = 0;
	return 0;
}
int envr(env* env, int *ex)
{
	while (env)
	{
		if ((env->f && env->f != 2) || !ft_strcmp("_=/usr/bin/env", env->value))
			printf("%s\n", env->value);
		env = env->next;
	}
	*ex = 0;
	return 0;
}

int	cd(env *env, char *path, int *ex)
{
	char *tmp;
	// printf("%s\n", path);
	if (!path)
	{
		tmp = get_value(env, "HOME=");
		if(!tmp)
		{
			free(tmp);
			tmp = NULL;
			printf("cd: HOME not set\n");
			*ex = 1;
			return 1;
		}
		if (!*tmp)
		{
			free(tmp);
			tmp = NULL;
			*ex = 0;
			return 0;
		}
		*ex = cd2(env, tmp);
		return *ex;
	}
	else
		*ex = cd2(env, path);
	return *ex;
}
char	*trim_last_dir(char *path)
{
	char	*last_slash;

	if (!ft_strcmp("/", path))
		return ft_strdup("/");
	last_slash = ft_strrchr(path, '/');
	return (word((char *)path, last_slash));
}

int	cd2(env *env, char *path)
{
	char *pwd;
	char *oldpwd;
	char *tmp;
	oldpwd = get_value(env, "PWD=");
	// printf("|%s|\n", path);
	if (chdir(path) == 0)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			tmp  = ft_strjoin(ft_strdup("/"), ft_strdup(path));
			printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			pwd = ft_strjoin(ft_strdup(oldpwd), tmp);
		}
		search_replace(env, "PWD", pwd);
		if (oldpwd)
			search_replace(env, "OLDPWD", oldpwd);
	// printf("%s\n", path);
		return 0;
	}
	switch (errno)
	{
		case EACCES:
		{
			if (!ft_strcmp(path, ".."))
				cd2(env, trim_last_dir(getcwd(NULL, 0)));
			else
				printf("cd: Permission denied.\n");
		}
			break;
		case ENOENT:
			printf("cd: Directory does not exist.\n");
			break;
		case ENOTDIR:    
			printf("cd: is not a directory.\n");
			break;
		default:
			printf("cd: Failed to change directory\n");
			break;
        }
	free(oldpwd);
	oldpwd = NULL;
	return 1;
}

void search_replace(env *env, char *key, char *rep)
{
	char *tmp;
	tmp = ft_strjoin(ft_strdup(key), ft_strdup("="));
	while (env && ft_strcmp(key, env->value) && ft_strncmp(env->value, tmp, ft_strlen(tmp)))
		env = env->next;
	// printf("%s\n", env->value);
	if (!env)
	{
		// free(tmp);
		return ;
	}
	// free(env->value);
	if(rep)
	{
	env->value = NULL;
	env->value = ft_strjoin(tmp, rep);
	}
	env->f = 1;
	// printf("%s\n", env->value);

}
void replace_create(env *env, char *key, char *rep)
{
	char *tmp;

	tmp = ft_strjoin(ft_strdup(key), ft_strdup("="));
	while (env && ft_strcmp(key, env->value) && ft_strncmp(env->value, key, ft_strlen(key)))
		env = env->next;
	// printf("%s\n", env->value);
	if (!env)
	{

		free(tmp);
		return ;
	}
	free(env->value);
	env->value = NULL;
	env->value = ft_strjoin(tmp, rep);
	env->f = 1;
	// printf("%s\n", env->value);

}