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
		// if (env->f)
		// {
			ret[i] = ft_strdup(env->value);
			i++;
		// }
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
			return 131;
		}
		else if (WTERMSIG(*status) == SIGINT)
		{
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
	if (*str != '$')
	{
		s = str;
		while (*str && *(str + 1) != '$') //(*str == '$' && expand_valid(*(str +1 )))
			str++;
		// printf("|%c|\n", *str);
		if (*(str + 1) == '$')
			ret = ft_strjoin(ret , ft_strjoin(word(s, str + 1), expand2(str + 1, envr, ex)));
		else
			ret = word(s, str);
		return ret;
	}
	if (*str == '$')
	{
		str++;
		if(*str == '?')
		{
			ret = ft_strjoin(ret , ft_strjoin(ft_itoa(*ex), expand2(str + 1, envr, ex)));
			return ret;
		}
		else
		{
			s = str;
			while (*str && !expand_valid(*(str)))
				str++;
			ret = ft_strjoin(ret , ft_strjoin(get_value(envr, ft_strjoin(word(s, str), "=")), expand2(str, envr, ex)));
			return ret;
		}
	}
	
	// while(*str)
	// {
	// 	if (!s && *str != '$')
	// 		s = str;
	// 	if(*str == '$' && !expand_valid(*str))
	// 	{
	// 		 ret = ft_strjoin(ret, word(s, str));
	// 	}
	// }
}
void expand(char **args, env *envr, int *ex)
{
	char *s;
	while (*args)
	{
		// printf("%s\n", *args);
		*args = expand2(*args, envr, ex);
		if (!*args || !**args)
		{
			// printf("%sssss\n", *args);
			*args = ft_strdup("");
		}
		args++;
	}
}

int excute(char **cmd, env **env, int fd_in, int *ex)
{
	// int ex = 0;
	expand(cmd, *env, ex);
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
	}
	else{
		close(130);
		sig_han = 0;
	}
}