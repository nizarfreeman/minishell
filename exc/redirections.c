/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:30:57 by aayache           #+#    #+#             */
/*   Updated: 2025/05/30 15:44:37 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	here_doc2(t_tree *root, env **env, int *ex, t_tree *left)
{
	if (ft_strrchr(root->right->cmd, '\'') || ft_strrchr(root->right->cmd, '"'))
	{
		if (left->fd != -1)
			close(left->fd);
		left->fd = root->fd;
	}
	else
	{
		if (left->fd != -1)
		{
			close(left->fd);
		}
		left->fd = read_and_expand(root->fd, *env, ex);
	}
}

void	here_doc3(t_tree *root, env **env, int *ex, t_tree *left)
{
	if (ft_strrchr(root->right->left->cmd, '\'')
		|| ft_strrchr(root->right->left->cmd, '"'))
	{
		if (left->fd != -1)
			close(left->fd);
		left->fd = root->fd;
		redirections(root->right, env, ex, left);
	}
	else
	{
		if (left->fd != -1)
			close(left->fd);
		left->fd = read_and_expand(root->fd, *env, ex);
		redirections(root->right, env, ex, left);
	}
}

void	here_doc(t_tree *root, env **env, int *ex, t_tree *left)
{
	int	fd;

	*ex = 0;
	fd = open(root->file, O_RDONLY, 0644);
	if (fd == -1)
	{
		perror("open");
		*ex = 1;
		return ;
	}
	else
		root->fd = fd;
	if (root->right->type >= 6 && root->type <= 9)
		here_doc3(root, env, ex, left);
	else
		here_doc2(root, env, ex, left);
}

void	redirections(t_tree *root, env **env, int *ex, t_tree *left)
{
	int		fd;
	char	**tmp;

	if (root->type != HERE_ODC)
		redirections_expand(root, env, ex);
	if (root->type == REDIRECTION_OUT)
		red_out(root, env, ex, left);
	else if (root->type == REDIRECTION_IN)
		red_in(root, env, ex, left);
	else if (root->type == APPEND)
		red_app(root, env, ex, left);
	else
		here_doc(root, env, ex, left);
	return ;
}

int	read_and_expand(int fd, env *envr, int *ex)
{
	char	*s;
	int		fd_ret;
	env		*ret;

	s = get_next_line(fd);
	ret = NULL;
	while (s)
	{
		ft_lstnew(&ret, expand2(s, envr, ex), 0);
		s = get_next_line(fd);
	}
	s = ft_strjoin("/tmp/", ft_itoa((uintptr_t)s));
	close(fd);
	fd = open(s, O_CREAT | O_RDWR | O_TRUNC, 0644);
	fd_ret = open(s, O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (ret)
	{
		write(fd, ret->value, ft_strlen(ret->value));
		ret = ret->next;
	}
	return (fd_ret);
}
