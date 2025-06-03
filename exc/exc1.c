/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:45:07 by aayache           #+#    #+#             */
/*   Updated: 2025/06/03 22:02:34 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	**pre_expand(char **args, env *envr, int *ex)
{
	env		*list;
	char	*tmp;

	list = NULL;
	while (*args)
	{
		tmp = *args;
		if (is_wildcard(tmp) && expand_wildcard(tmp, &list))
			*ex = *ex;
		else if (expand5(&list, tmp, envr))
			*ex = *ex;
		args++;
	}
	return (lst_to_arr2(list));
}

int	excute2(char **cmd, env **env, int fd_in, int *ex)
{
	if (!ft_strcmp(*cmd, "echo"))
		return (echo(&cmd[1], ex));
	else if (!ft_strcmp(*cmd, "env"))
		return (envr(*env, ex));
	else if (!ft_strcmp(*cmd, "exit"))
		(my_exit(&cmd[1], ex));
	else if (cmd && *cmd && !**cmd)
	{
		*ex = 0;
		if (cmd[1])
			return (excute(&cmd[1], env, fd_in, ex));
		return (0);
	}
	else
		return (excute_cmd(cmd, env, fd_in, ex));
	return (1);
}

int	excute(char **cmd, env **env, int fd_in, int *ex)
{
	if (is_all_empty(cmd))
	{
		write(2, ": command not found\n", 21);
		*ex = 127;
		return (1);
	}
	if (ft_strcmp(*cmd, "export") || ft_strcmp(*(pre_expand(cmd, *env, ex)),
			"export"))
		cmd = pre_expand(cmd, *env, ex);
	if (!cmd || !*cmd)
		return (1);
	else if (!ft_strcmp(*cmd, "cd"))
		return (cd(*env, &cmd[1], ex));
	else if (!ft_strcmp(*cmd, "export"))
		return (export(env, &cmd[1], ex));
	else if (!ft_strcmp(*cmd, "unset"))
		return (unset(env, &cmd[1], ex));
	else if (!ft_strcmp(*cmd, "pwd"))
		return (pwd(*env, ex));
	else
		return (excute2(cmd, env, fd_in, ex));
}

void	handle_int(int sig)
{
	if (g_han == 1)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		*(get_exit_status(NULL)) = 130;
	}
	else
	{
		(void)(sig);
	}
}
