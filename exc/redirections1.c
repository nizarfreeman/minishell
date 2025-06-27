/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:12:56 by aayache           #+#    #+#             */
/*   Updated: 2025/06/23 15:58:30 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	redirections_expand2(t_tree *root, t_env **t_env, int *ex)
{
	char	**tmp;

	tmp = pre_expand(root->right->args, *t_env, ex);
	if (!ft_strcmp(root->right->cmd, "''") || !ft_strcmp(root->right->cmd,
			"\"\""))
	{
		root->right->cmd = *tmp;
		*ex = 0;
		return (0);
	}
	if (tmp && *tmp && (tmp[1] || !**tmp))
	{
		write(2, "ambiguous redirect\n", 20);
		*ex = 1;
		return (1);
	}
	else
	{
		root->right->cmd = *tmp;
		*ex = 0;
		return (0);
	}
}

int	redirections_expand(t_tree *root, t_env **t_env, int *ex)
{
	char	**tmp;

	if (root->right->type >= 6 && root->right->type <= 9)
	{
		tmp = pre_expand(root->right->left->args, *t_env, ex);
		if (tmp && *tmp && tmp[1] && ft_strcmp(*(root->right->left->args), ""))
		{
			write(2, "ambiguous redirect\n", 20);
			*ex = 1;
			return (1);
		}
		else
		{
			root->right->left->cmd = *tmp;
			*ex = 0;
			return (0);
		}
	}
	else
		return (redirections_expand2(root, t_env, ex));
}

void	red_out(t_tree *root, t_env **t_env, int *ex, t_tree *left)
{
	int	fd;

	if (root->right->type >= 6 && root->right->type <= 9)
		fd = open(root->right->left->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1)
	{
		*ex = 0;
		ft_dup2(fd, STDOUT_FILENO);
		close(fd);
		if (root->right->type >= 6 && root->right->type <= 9)
		{
			redirections(root->right, t_env, ex, left);
		}
	}
	else
	{
		perror("open");
		*ex = 1;
	}
}

void	red_in(t_tree *root, t_env **t_env, int *ex, t_tree *left)
{
	int	fd;

	if (root->right->type >= 6 && root->right->type <= 9)
		fd = open(root->right->left->cmd, O_RDONLY);
	else
		fd = open(root->right->cmd, O_RDONLY);
	if (fd != -1)
	{
		ft_dup2(fd, STDIN_FILENO);
		close(fd);
		*ex = 0;
		if (root->right->type >= 6 && root->right->type <= 9)
			redirections(root->right, t_env, ex, left);
	}
	else
	{
		perror("open");
		*ex = 1;
	}
}

void	red_app(t_tree *root, t_env **t_env, int *ex, t_tree *left)
{
	int	fd;

	if (root->right->type >= 6 && root->right->type <= 9)
		fd = open(root->right->left->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(root->right->cmd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd != -1)
	{
		*ex = 0;
		ft_dup2(fd, STDOUT_FILENO);
		close(fd);
		if (root->right->type >= 6 && root->right->type <= 9)
		{
			redirections(root->right, t_env, ex, left);
		}
	}
	else
	{
		perror("open");
		*ex = 1;
	}
}
