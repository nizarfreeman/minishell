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

int excute_cmd(char **cmd, env **env, int fd_in)
{
	char **envr;
	char *path;
	pid_t pid;
	int status;
	envr = lst_to_arr(*env);
	path = get_path(cmd, envr);
	if (!path)
		printerr(cmd[0], 0);
	pid = fork();
	if (pid == 0)
	{
		if(fd_in != -1)
		{
			// write(2, "here\n",5);
			// fprintf(stderr, "%d\n", fd_in);
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
	wait(&status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 20);
			return 131;
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			return 130;
		}
	}
	return (status);
}

void expand(char **args, env *envr)
{
	char *s;
	while (*args)
	{
		s = *args;
		if (*s == '$' && *(s + 1))
			*args = get_value(envr, s + 1);
		args++;
	}
}

int excute(char **cmd, env **env, int fd_in)
{
	expand(cmd, *env);
	if (!cmd)
		return 1;
	else if (!ft_strcmp(*cmd, "cd"))
		return cd(*env, cmd[1]);
	else if (!ft_strcmp(*cmd, "export"))
		return export(env, &cmd[1]);
	else if (!ft_strcmp(*cmd, "unset"))
		return unset(env, &cmd[1]);
	else if (!ft_strcmp(*cmd, "pwd"))
		return pwd(*env);
	else if (!ft_strcmp(*cmd, "echo"))
		return echo(&cmd[1]);
	else if (!ft_strcmp(*cmd, "env"))
		return envr(*env);
	// else if (!ft_strcmp(*cmd, "exit"))
	// my_exit(&cmd[1]);
	else
		return excute_cmd(cmd, env, fd_in);
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