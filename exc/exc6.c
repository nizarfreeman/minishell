/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:11:23 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 16:28:05 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	filter_mid2(env **files, int found, env **tmp, env **prev)
{
	if (!found)
	{
		if (*prev)
			(*prev)->next = (*tmp)->next;
		else
			*files = (*tmp)->next;
		*tmp = (*tmp)->next;
	}
	else
	{
		*prev = *tmp;
		*tmp = (*tmp)->next;
	}
}

void	filter_mid(env **files, env *arg)
{
	env		*tmp;
	env		*prev;
	int		found;
	char	*search_pos;

	tmp = *files;
	prev = NULL;
	while (tmp)
	{
		found = 0;
		search_pos = tmp->tmp;
		while (*search_pos)
		{
			if (!ft_strncmp(search_pos, arg->value, ft_strlen(arg->value)))
			{
				tmp->tmp = search_pos + ft_strlen(arg->value);
				found = 1;
				break ;
			}
			search_pos++;
		}
		filter_mid2(files, found, &tmp, prev);
	}
}

void	filter_last(env **files, env *arg)
{
	env	*tmp;
	env	*next;

	tmp = *files;
	if (!ft_strcmp(arg->value, ""))
		return ;
	while (tmp)
	{
		if (ft_strlen(arg->value) <= ft_strlen(tmp->tmp))
		{
			tmp->tmp += ft_strlen(tmp->tmp) - ft_strlen(arg->value);
			if (ft_strncmp(tmp->tmp, arg->value, ft_strlen(arg->value)))
			{
				(1) && (next = tmp, tmp = tmp->next);
				remove_node(files, next->value);
			}
			else
				(1) && (tmp->tmp += ft_strlen(arg->value), tmp = tmp->next);
		}
		else
		{
			remove_node(files, tmp->value);
			tmp = tmp->next;
		}
	}
}

int	exec_tree2(t_tree *root, env **env, int *ex)
{
	int	status;

	if (root->type == PIPE)
		return (exce_pipe(root, env, ex));
	if (root->type >= 6 && root->type <= 9)
	{
		status = fork();
		if (status == 0)
		{
			redirections(root, env, ex, root->left);
			if (!*ex && root->left)
				exec_tree(root->left, env, ex);
			exit(*ex);
		}
		waitpid(status, &status, 0);
		*ex = WEXITSTATUS(status);
	}
	return (*ex);
}

int	exec_tree(t_tree *root, env **env, int *ex)
{
	int	status;

	if (root->type == WORD || root->type >= 18)
		return (excute(root->args, env, root->fd, ex));
	if (root->type == AND_IF)
	{
		status = exec_tree(root->left, env, ex);
		if (!status)
			return (exec_tree(root->right, env, ex));
		return (status);
	}
	if (root->type == OR_IF)
	{
		status = exec_tree(root->left, env, ex);
		if (status)
			return (exec_tree(root->right, env, ex));
		return (status);
	}
	else
		return (exec_tree2(root, env, ex));
	return (*ex);
}
