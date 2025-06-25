/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:38:36 by aayache           #+#    #+#             */
/*   Updated: 2025/06/22 14:52:32 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	pipe_left(t_tree *root, t_env **t_env, int *fds, int *exi)
{
	pid_t	pid;
	int		ex;

	ex = 1;
	pid = ft_fork(fds[0], fds[1]);
	if (pid == 0)
	{
		ft_dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
		ex = exec_tree(root, t_env, exi);
		exit(ex);
	}
	close(fds[1]);
	return (pid);
}

int	exce_pipe(t_tree *root, t_env **t_env, int *exi)
{
	int	fds[2];
	int	ex;
	int	left_pid;
	int	pid;

	if (pipe(fds) == -1)
	{
		gc_free();
		ft_free();
		exit(1);
	}
	left_pid = pipe_left(root->left, t_env, fds, exi);
	pid = ft_fork(fds[0], -1);
	if (pid == 0)
	{
		ft_dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		exec_tree(root->right, t_env, exi);
		exit(*exi);
	}
	close(fds[0]);
	waitpid(left_pid, &ex, 0);
	waitpid(pid, &ex, 0);
	*exi = WEXITSTATUS(ex);
	return (*exi);
}
