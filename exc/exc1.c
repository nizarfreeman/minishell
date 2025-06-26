/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:45:07 by aayache           #+#    #+#             */
/*   Updated: 2025/06/26 22:45:41 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	**pre_expand(char **args, t_env *t_envr, int *ex)
{
	t_env	*list;
	char	*tmp;
	t_env	*ret;

	list = NULL;
	ret = NULL;
	while (*args)
	{
		tmp = *args;
		if (expand5(&list, tmp, t_envr))
			*ex = *ex;
		args++;
	}
	while (list)
	{
		tmp = list->value;
		if (tmp && *tmp && is_wildcard(tmp) && expand_wildcard(tmp, &ret))
			*ex = *ex;
		else
			ft_lstnew(&ret, rm_q(list->value), 0);
		list = list->next;
	}
	return (lst_to_arr2(ret));
}

int	excute2(char **cmd, t_env **t_env, int fd_in, int *ex)
{
	if (!ft_strcmp(*cmd, "echo"))
		return (echo(&cmd[1], ex));
	else if (!ft_strcmp(*cmd, "env"))
		return (t_envr(*t_env, ex));
	else if (!ft_strcmp(*cmd, "exit"))
		(my_exit(&cmd[1], ex));
	else if (cmd && *cmd && !**cmd)
	{
		*ex = 0;
		if (cmd[1])
			return (excute(&cmd[1], t_env, fd_in, ex));
		return (0);
	}
	else
		return (excute_cmd(cmd, t_env, fd_in, ex));
	return (1);
}

int	excute(char **cmd, t_env **t_env, int fd_in, int *ex)
{
	if (is_all_empty(cmd))
	{
		write(2, ": command not found\n", 21);
		*ex = 127;
		return (1);
	}
	if (ft_strcmp(*cmd, "export") || ft_strcmp(*(pre_expand(cmd, *t_env, ex)),
			"export"))
		cmd = pre_expand(cmd, *t_env, ex);
	if (!cmd || !*cmd)
		return (1);
	else if (!ft_strcmp(*cmd, "cd"))
		return (cd(*t_env, &cmd[1], ex));
	else if (!ft_strcmp(*cmd, "export"))
		return (export(t_env, &cmd[1], ex));
	else if (!ft_strcmp(*cmd, "unset"))
		return (unset(t_env, &cmd[1], ex));
	else if (!ft_strcmp(*cmd, "pwd"))
		return (pwd(*t_env, ex));
	else
		return (excute2(cmd, t_env, fd_in, ex));
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
