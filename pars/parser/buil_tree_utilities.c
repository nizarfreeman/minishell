/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buil_tree_utilities.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:06:24 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:06:32 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_token	*build_left_tokens(t_token *head, int root_pos)
{
	t_token	*left_side;
	t_token	*curr;
	t_arg	*arg;
	int		i;

	left_side = NULL;
	curr = head;
	i = 0;
	while (i < root_pos && curr)
	{
		arg = create_token_arg(curr);
		if (!arg || !add_token(&left_side, arg))
			return (NULL);
		curr = curr->next;
		i++;
	}
	return (left_side);
}

t_token	*build_right_tokens(t_token *head, int root_pos)
{
	t_token	*right_side;
	t_token	*curr;
	t_arg	*arg;
	int		i;

	right_side = NULL;
	curr = head;
	i = 0;
	while (i <= root_pos && curr)
	{
		curr = curr->next;
		i++;
	}
	while (curr)
	{
		arg = create_token_arg(curr);
		if (!arg || !add_token(&right_side, arg))
			return (NULL);
		curr = curr->next;
	}
	return (right_side);
}

int	validate_tree_node(t_tree *node)
{
	if (!node)
		return (0);
	if (node->type == PIPE && (!node->left || !node->right))
		return (0);
	if ((node->type == AND_IF || node->type == OR_IF) && !node->right)
		return (0);
	return (1);
}

t_tree	*build_complex_tree(t_token *head, int root_pos)
{
	t_tree	*node;
	t_token	*left_side;
	t_token	*right_side;

	node = root(head, root_pos);
	if (!node)
		return (NULL);
	left_side = build_left_tokens(head, root_pos);
	right_side = build_right_tokens(head, root_pos);
	if (left_side)
		node->left = build_tree(left_side);
	if (right_side)
		node->right = build_tree(right_side);
	if (!validate_tree_node(node))
		return (NULL);
	return (node);
}

t_tree	*handle_parentheses_(t_token *head)
{
	t_token	*inner_tokens;
	t_tree	*node;

	if (!is_enclosed_in_parentheses(head))
		return (NULL);
	inner_tokens = extract_paren_content(head);
	if (!inner_tokens)
		return (NULL);
	node = build_tree(inner_tokens);
	return (node);
}
