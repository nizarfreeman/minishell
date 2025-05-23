#include "s.h"

int is_all_num(char *s)
{
	if (!s)
		return 0;
	while (*s >= '0' && *s <= '9')
		s++;
	if (!*s)
		return 1;
	return 0;
}

void no_env(env **ret)
{
	char *s;

	s = get_value(*ret, "PATH=");
	if (!s)
		ft_lstnew(ret, "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", 2);
	s = get_value(*ret, "PWD");
	if (!s)
	{
		// s = ft_strjoin(ft_strdup("PWD="), getcwd(NULL, 0));
		ft_lstnew(ret, ft_strjoin(ft_strdup("PWD="), getcwd(NULL, 0)), 1);	
	}
	else
		search_replace(*ret, "PWD", getcwd(NULL, 0));
	s = get_value(*ret, "OLDPWD");
	if (!s)
		ft_lstnew(ret, "OLDPWD", 0);
	s = get_value(*ret, "_");
	if (!s)
		ft_lstnew(ret, "_=/usr/bin/env", 2);
	else
		search_replace(*ret, "_", NULL);
	
	s = get_value(*ret, "SHLVL");
	if (!s)
		ft_lstnew(ret, "SHLVL=1", 1);
	else if(is_all_num(s + 1) && (int)ft_atoi(s + 1) >= 999)
	{	
		printf("bash: warning: shell level (%d) too high, resetting to 1\n", (int)ft_atoi(s + 1));
		search_replace(*ret, "SHLVL", "1");
	}
	else 
		search_replace(*ret, "SHLVL", ft_itoa(ft_atoi(s + 1) + 1));
}

int check_value(char *str)
{
	while (*str && *str != '=')
		str++;
	if (!*str)
		return 0;
	return 1;
}

env *creat_env(char **envr)
{
	env *tmp;
	env *ret;
	ret = NULL;
	while (*envr)
	{
		ft_lstnew(&ret, *envr, check_value(*envr));
		envr++;
	}
	// if (!*envr)
	no_env(&ret);
	// handle_shlvl(ret);
	return ret;
}

void remove_node(env **envrr, char *str)
{
	char *tmp;
	env *envr;
	env *prev;

	envr = *envrr;
	tmp = ft_strjoin(ft_strdup(str), ft_strdup("="));
	if (envr && (!ft_strcmp(str, envr->value) || !ft_strncmp(envr->value, tmp, ft_strlen(tmp))))
	{
		*envrr = envr->next;
		return ;
	}
	while (envr && (ft_strncmp(envr->value, tmp, ft_strlen(tmp)) && ft_strcmp(envr->value, str)))
	{
		prev = envr;
		envr = envr->next;
	}
	if (!envr)
		return ;
	prev->next = envr->next;
	free(envr->value);
	free(envr);
}

int unset(env **env, char **str, int *ex)
{
	if (!str)
	{
		*ex = 0;
		return 0;
	}
	while (*str)
	{
		if (ft_strcmp("_", *str))
			remove_node(env, *str);
		str++;
	}
	*ex = 0;
	return 0;
}