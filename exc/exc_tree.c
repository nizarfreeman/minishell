/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:38:36 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 16:28:13 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	pipe_left(t_tree *root, env **env, int *fds, int *exi)
{
	pid_t	pid;
	int		ex;

	ex = 1;
	pid = fork();
	if (pid == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
		ex = exec_tree(root, env, exi);
		exit(ex);
	}
	close(fds[1]);
	return (pid);
}

int	exce_pipe(t_tree *root, env **env, int *exi)
{
	int	fds[2];
	int	ex;
	int	left_pid;
	int	pid;

	pipe(fds);
	left_pid = pipe_left(root->left, env, fds, exi);
	pid = fork();
	if (pid == 0)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		exec_tree(root->right, env, exi);
		exit(*exi);
	}
	close(fds[0]);
	waitpid(left_pid, &ex, 0);
	waitpid(pid, &ex, 0);
	*exi = WEXITSTATUS(ex);
	return (ex);
}
