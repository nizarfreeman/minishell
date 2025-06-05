/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:02:53 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:03:21 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_tree	*root(t_token *head, int root_pos)
{
	t_tree	*new_node;
	int		i;

	i = 0;
	while (i != root_pos)
	{
		head = head->next;
		i++;
	}
	new_node = gc_malloc(sizeof(t_tree));
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = head->type;
	new_node->cmd = gc_strdup(head->token);
	new_node->args = NULL;
	new_node->file = NULL;
	if (head->file)
		new_node->file = gc_strdup(head->file);
	new_node->fd = -1;
	return (new_node);
}
