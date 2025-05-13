#include "s.h"
char *get_value(env *env, char *key)
{
	while (env && ft_strcmp(env->value, key) && ft_strncmp(env->value, key, ft_strlen(key)))
		env = env->next;
	if (!env)
		return (NULL);
	return (ft_strdup(env->value + ft_strlen(key)));
}
int pwd(env *env)
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
	return 0;
}
int echo_option(char *str)
{
	if (!str)
		return 1;
	if (*str != '-')
		return 1;
	str++;
	while (*str && *str == 'n')
		str++;
	if (!*str)
		return 0;
	return 1;
}

int echo(char **str)
{
	int opt = echo_option(*str);
	if (!opt)
		str++;
	while (*str)
	{
		write(1, *str, ft_strlen(*str));
		// printf("%s", *str);
		if (*(str + 1))
			printf(" ");
	// if(*str && !echo_option(*str))
	// {
	// 	if(str[1])
	// 		printf("%s", str[1]);
	// }
	// else
	// {
	// 	if (*str)
	// 		printf("%s", *str);
	// 	else
	// 		printf("\n");
	// }
	str++;	
	}
	if(opt)
		printf("\n");
	return 0;
}
int envr(env* env)
{
	while (env)
	{
		if (env->f)
		printf("%s\n", env->value);
		env = env->next;
	}
	return 0;
}

int	cd(env *env, char *path)
{
	char *tmp;

	if (!path)
	{
		tmp = get_value(env, "HOME=");
		if(!tmp)
		{
			free(tmp);
			tmp = NULL;
			printf("cd: HOME not set\n");
			return 1;
		}
		if (!*tmp)
		{
			free(tmp);
			tmp = NULL;
			return 0;
		}
		return cd2(env, tmp);
	}
	else
		return cd2(env, path);
}

int	cd2(env *env, char *path)
{
	char *pwd;
	char *oldpwd;
	char *tmp;
	oldpwd = get_value(env, "PWD=");
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
			printf("cd: Permission denied.\n");
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