/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buil_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:06:00 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:06:07 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_tree	*build_tree(t_token *head)
{
	t_tree	*node;
	int		root_pos;

	if (!head)
		return (NULL);
	node = handle_parentheses_(head);
	if (node)
		return (node);
	if (is_simple(head))
		return (insert_command(head));
	root_pos = get_root_pos(head);
	if (root_pos == -1)
		return (NULL);
	return (build_complex_tree(head, root_pos));
}
