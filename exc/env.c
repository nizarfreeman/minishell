#include "s.h"

env *no_env(void)
{
	char *s;
	env *ret;
	s = ft_strjoin(ft_strdup("PWD="), getcwd(NULL, 0));
	ft_lstnew(&ret, s, 1);
	ft_lstnew(&ret, "OLDPWD", 0);
	ft_lstnew(&ret, "SHLVL=1", 1);
	free(s);
	s = NULL;
	ft_lstnew(&ret, "_=/usr/bin/env", 1);
	return ret;
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
	if (!*envr)
		return no_env();
	while (*envr)
	{
		ft_lstnew(&ret, *envr, check_value(*envr));
		envr++;
	}
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

int unset(env **env, char **str)
{
	if (!str)
		return 0;
	while (*str)
	{
		remove_node(env, *str);
		str++;
	}
	return 0;
}