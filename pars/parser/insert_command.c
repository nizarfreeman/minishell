/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:58:59 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 12:59:27 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_tree	*insert_command(t_token *head)
{
	t_tree	*new_node;

	new_node = gc_malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = head->type;
	new_node->cmd = gc_strdup(head->token);
	new_node->args = get_args(head);
	new_node->file = NULL;
	if (head->file)
		new_node->file = gc_strdup(head->file);
	new_node->fd = -1;
	return (new_node);
}
