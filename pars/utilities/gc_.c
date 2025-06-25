/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:09 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/21 15:37:54 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utilities/utilities.h"
#include "../../exc/s.h"

void	*gc_malloc(int size)
{
	static t_gc	*head = NULL;
	void		*ptr;
	t_gc		*new_node;

	if (size == -1)
		return ((void *)&head);
	ptr = malloc(size);
	if (!ptr)
	{
		ft_free();
		gc_free();
		exit(1);
	}
	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		free(ptr);
		ft_free();
		gc_free();
		exit(1);
	}
	new_node->ptr = ptr;
	new_node->next = head;
	head = new_node;
	return (ptr);
}

void	gc_free(void)
{
	t_gc	**head_ptr;
	t_gc	*curr;
	t_gc	*tmp;

	head_ptr = (t_gc **)gc_malloc(-1);
	curr = *head_ptr;
	while (curr)
	{
		tmp = curr->next;
		free(curr->ptr);
		free(curr);
		curr = tmp;
	}
	*head_ptr = NULL;
}

void	set_variables(t_token **new_node, t_arg *arg)
{
	(*new_node)->file = arg->file;
	(*new_node)->token = arg->token;
	(*new_node)->type = arg->type;
	(*new_node)->space = arg->space;
	if (arg->quoted)
		(*new_node)->quoted = 1;
}

int	add_token(t_token **head, t_arg *arg)
{
	t_token	*new_node;
	t_token	*tmp;

	new_node = (t_token *)gc_malloc(sizeof(t_token));
	if (!new_node)
	{
		gc_free();
		exit(1);
	}
	ft_memset(new_node, 0, sizeof(t_token));
	set_variables(&new_node, arg);
	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while ((*head)->next)
			*head = (*head)->next;
		(*head)->next = new_node;
		new_node->prev = *head;
		*head = tmp;
	}
	return (1);
}
