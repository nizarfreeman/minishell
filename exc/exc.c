#include "s.h"
int sig_han = 0;
char **lst_to_arr2(env *env)
{
	int size;
	char **ret;
	int i;
	size = ft_lstsize(env) + 1;
	ret = malloc(sizeof(char *) * size);
	i = 0;
	while (env)
	{
			ret[i] = ft_strdup(env->value);
			i++;
		env = env->next;
	}
	ret[i] = NULL;
	return ret;
}
char **lst_to_arr(env *env)
{
	int size;
	char **ret;
	int i;
	size = ft_lstsize(env) + 1;
	ret = malloc(sizeof(char *) * size);
	i = 0;
	while (env)
	{
		if (env->f)
		{
			ret[i] = ft_strdup(env->value);
			i++;
		}
		env = env->next;
	}
	ret[i] = NULL;
	return ret;
}

int excute_cmd(char **cmd, env **env, int fd_in, int *status)
{
	char **envr;
	char *path;
	pid_t pid;
	// int status;
	envr = lst_to_arr(*env);
	path = get_path(cmd, envr);
	if (!path)
		printerr(cmd[0], 0);
	pid = fork();
	if (pid == 0)
	{
		if(fd_in != -1)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		sig_han = 0;
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (execve(path, cmd, envr) == -1)
			exit(1);
	}
	close(fd_in);
	wait(status);
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 20);
			*status = 131;
			return 131;
		}
		else if (WTERMSIG(*status) == SIGINT)
		{
			*status = 130;
			write(1, "\n", 1);
			return 130;
		}
	}
	*status = WEXITSTATUS(*status);
	// printf("%d\n", *status);
	return (*status);
}

int expand_valid(char c)
{
	return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && c != '_' && c != '?');
}

char *expand2(char *str, env *envr, int *ex)
{
	char *ret;
	char *s = NULL;
	ret = NULL;
	if (!str || !*str)
        return ft_strdup("");
	if (*str != '$')
	{
		s = str;
		while (*str && *(str + 1) != '$') //(*str == '$' && expand_valid(*(str +1 )))
			str++;
		// printf("|%c|\n", *str);
		if (*(str + 1) == '$')
			ret = ft_strjoin(word(s, str + 1), expand2(str + 1, envr, ex));
		else
			ret = word(s, str);
		return ret;
	}
	if (*str == '$')
	{
		if (expand_valid(*(str + 1)))
		{
			ret = ft_strjoin(ft_strdup("$"), expand2(str + 1, envr, ex));
			return ret;
		}
			str++;
		if(*str == '?')
		{
			ret = ft_strjoin(ft_itoa(*ex), expand2(str + 1, envr, ex));
			return ret;
		}
		else
		{
			s = str;
			while (*str && !expand_valid(*(str)))
				str++;
			ret = ft_strjoin(get_value(envr, ft_strjoin(word(s, str), "=")), expand2(str, envr, ex));
			return ret;
		}
	}
	return NULL;
}
char **expand_split(char **args)
{
	env *ret;
	char **tmp;
	ret = NULL;
	while (*args)
	{
		tmp = ft_split(*args, ' ');
		while (tmp && *tmp)
		{
			// printf("%s\n", *tmp);
			ft_lstnew(&ret, *tmp, 0);
			tmp++;
		}
		args++;
	}
	return (lst_to_arr2(ret));
}

char **expand(char **args, env *envr, int *ex)
{
	char *s;
	char **tmp = args;
	while (tmp && *tmp)
	{
		repl1(' ', '\2', *tmp);
		tmp++;
	}
	tmp = args;
	while (*args)
	{
		*args = expand2(*args, envr, ex);
		if (!*args || !**args)
			*args = ft_strdup("");
		args++;
	}
	args = expand_split(tmp);
	tmp = args;
	while (tmp && *tmp)
	{
		repl1('\2', ' ', *tmp);
		tmp++;
	}
	return args;
}
char *creat_word(char *str, int f, char c, int *p)
{
	char *s = str;
	if (f)
	{
		if (*str == c)
		{
			*p = 1;
			return ft_strdup("");
		}
		while (*(s) != c)
		{
			s++;
			(*p)++;
		}
		(*p) = s - str + 1;
		// printf("%d\n", str - s);
		return word(str, s);
	}
	while (*(str) && *(str) != '\'' && *(str) != '"')
		str++;
		(*p) = str - s;

		// printf("%s\n", str);
	return word(s, str);
}
char **pre_expand(char **args, env *envr, int *ex)
{
	env *list = NULL;
	char *tmp = NULL;
	char *ret = NULL;
	char **tmp1 = NULL;
	int p = 0;
	while(*args)
	{
		tmp = *args;
		ret = NULL;
		while(*tmp)
		{
			// if (tmp1 && *tmp1)
			// {
			// 	ret = ft_strjoin(ret, *tmp1);
			// 	tmp1 = NULL;
			// }
			if(*tmp == '"')
			{
				ret = ft_strjoin(ret, expand2(creat_word(++tmp, 1, '"', &p), envr, ex));
				tmp += p;
			}
			if(*tmp == '\'')
			{
				ret = ft_strjoin(ret, creat_word(++tmp, 1, '\'', &p));
				tmp += p;
			}
			if (*tmp)
			{
				tmp1 = malloc(sizeof(char *) * 3);
				tmp1[0] = creat_word(tmp, 0, 0, &p);
				tmp1[1] = NULL;
				tmp1 = expand(tmp1, envr, ex);
				if(tmp1[0] && !tmp1[1])
				{
					// 
					// printf("|%s|\n", tmp1[0]);
					ret = ft_strjoin(ret , *tmp1);
					tmp1 = NULL;
				}
				else
				{
					ret = ft_strjoin(ret , *tmp1);
					ft_lstnew(&list, ret, 0);
					tmp1++;
					p = 0;
					ret = expand2(creat_word(tmp, 0, 0, &p), envr, ex);
					if (*(ret + ft_strlen(ret) - 1) == ' ' || *(ret + ft_strlen(ret) - 1) == '"')
					{
						while(*tmp1)
							ft_lstnew(&list, *tmp1++, 0);
						tmp1 = NULL;
						ret = NULL;
					}
					else
					{
						while (*tmp1 && *(tmp1 + 1))
							ft_lstnew(&list, *tmp1++, 0);
						ret = *tmp1;

					}
				}
				tmp += p;
			}
			if (!*tmp)
			{
				ft_lstnew(&list, ret, 0);
				ret = NULL;
			}
		}
		args++;
	}
	return lst_to_arr2(list);
}

int excute(char **cmd, env **env, int fd_in, int *ex)
{
	// if(ft_strcmp(*cmd, "export") || ft_strcmp(expand2(*cmd, *env, ex), "export"))
		cmd = pre_expand(cmd, *env, ex);
	if (!cmd)
		return 1;
	else if (!ft_strcmp(*cmd, "cd"))
		return cd(*env, cmd[1], ex);
	else if (!ft_strcmp(*cmd, "export"))
		return export(env, &cmd[1], ex);
	else if (!ft_strcmp(*cmd, "unset"))
		return unset(env, &cmd[1], ex);
	else if (!ft_strcmp(*cmd, "pwd"))
		return pwd(*env, ex);
	else if (!ft_strcmp(*cmd, "echo"))
		return echo(&cmd[1], ex);
	else if (!ft_strcmp(*cmd, "env"))
		return envr(*env, ex);
	else if (!ft_strcmp(*cmd, "exit"))
		my_exit(&cmd[1], ex);
	else if(!**cmd)
	{
		*ex = 0;
		if (cmd[1])
			return excute(&cmd[1], env, fd_in, ex);
		return (0);
	}
	else
		return excute_cmd(cmd, env, fd_in, ex);
	return 1;
}

void handle_int(int sig)
{
	if(sig_han == 1){
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		// *(get_exit_status(NULL)) = 130;
	}
	else{
		exit(130);
		sig_han = 0;
	}
}