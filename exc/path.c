#include "s.h"
char	*creat_path(char *cmd, char **path)
{
	int		i;
	char	*s;
	char	*tmp;

	(1) && (i = 0, s = NULL);
	if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	if (ft_strncmp(cmd, "./", 2) == 0 || cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	if (!path)
		return (NULL);
	tmp = ft_strjoin(ft_strdup("/"), ft_strdup(cmd));
	while (path[i])
	{
		s = ft_strjoin(ft_strdup(path[i]), ft_strdup(tmp));
		if (access(s, X_OK) == 0)
			break ;
		free(s);
		(1) && (s = NULL, i++);
	}
	free(tmp);
	return (s);
}

char	*get_path(char **cmd, char **env)
{
	char	**tmp;
	char	*path;

	tmp = NULL;
	while (*env && ft_strncmp(*env, "PATH=", 5))
		env++;
	if (*env)
		tmp = ft_split(*env + 5, ':');
	path = creat_path(*cmd, tmp);
	// free2arr(tmp, -1);
	return (path);
}
void	printerr(char *cmd, int i)
{
	char	*shell;
	char	*s;

	if (i || ft_strrchr(cmd, '/'))
	{
		s = ": No such file or directory";
		shell = "bash: ";
		write(STDERR_FILENO, shell, ft_strlen(shell));
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, s, ft_strlen(s));
		write(STDERR_FILENO, "\n", 1);
	}
	else
	{
		s = ": command not found";
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, s, ft_strlen(s));
		write(STDERR_FILENO, "\n", 1);
	}
}