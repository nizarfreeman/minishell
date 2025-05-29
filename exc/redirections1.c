/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:12:56 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 13:13:28 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	redirections_expand2(t_tree *root, env **env, int *ex)
{
	char	**tmp;

	tmp = pre_expand(root->right->args, *env, ex);
	if (tmp && *tmp && (tmp[1] || !**tmp))
	{
		write(2, "ambiguous redirect\n", 20);
		*ex = 1;
		return ;
	}
	else
	{
		root->right->cmd = *tmp;
		*ex = 0;
	}
}

void	redirections_expand(t_tree *root, env **env, int *ex)
{
	char	**tmp;

	if (root->right->type >= 6 && root->right->type <= 9)
	{
		tmp = pre_expand(root->right->left->args, *env, ex);
		if (tmp && *tmp && tmp[1])
		{
			write(2, "ambiguous redirect\n", 20);
			*ex = 1;
			return ;
		}
		else
		{
			root->right->left->cmd = *tmp;
			*ex = 0;
		}
	}
	else
		redirections_expand2(root, env, ex);
}

void	red_out(t_tree *root, env **env, int *ex, t_tree *left)
{
	int	fd;

	if (root->right->type >= 6 && root->right->type <= 9)
		fd = open(root->right->left->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else
		fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd != -1)
	{
		*ex = 0;
		dup2(fd, STDOUT_FILENO);
		if (root->right->type >= 6 && root->right->type <= 9)
			redirections(root->right, env, ex, left);
		close(fd);
	}
	else
	{
		perror("open");
		*ex = 1;
	}
}

void	red_in(t_tree *root, env **env, int *ex, t_tree *left)
{
	int	fd;

	if (root->right->type >= 6 && root->right->type <= 9)
		fd = open(root->right->left->cmd, O_RDONLY);
	else
		fd = open(root->right->cmd, O_RDONLY);
	if (fd != -1)
	{
		*ex = 0;
		if (left->fd != -1)
			close(left->fd);
		left->fd = fd;
		if (root->right->type >= 6 && root->right->type <= 9)
			redirections(root->right, env, ex, left);
	}
	else
	{
		perror("open");
		*ex = 1;
	}
}

void	red_app(t_tree *root, env **env, int *ex, t_tree *left)
{
	int	fd;

	if (root->right->type >= 6 && root->type <= 9)
		fd = open(root->right->left->cmd, O_WRONLY | O_CREAT | O_APPEND);
	else
		fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_APPEND);
	if (fd != -1)
	{
		*ex = 0;
		dup2(fd, STDOUT_FILENO);
		if (root->right->type >= 6 && root->type <= 9)
			redirections(root->right, env, ex, left);
		close(fd);
	}
	else
	{
		perror("open");
		*ex = 1;
	}
}
