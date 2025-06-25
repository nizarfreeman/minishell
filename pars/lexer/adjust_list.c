/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:29:43 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:44:54 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

t_token	*copy_token(t_token *head)
{
	t_token	*copy;

	if (!head)
		return (NULL);
	copy = gc_malloc(sizeof(t_token));
	copy->token = gc_strdup(head->token);
	copy->type = head->type;
	copy->quoted = head->quoted;
	copy->space = head->space;
	copy->next = NULL;
	copy->prev = NULL;
	copy->file = gc_strdup(head->file);
	return (copy);
}

void	adjust_list(t_token **head)
{
	t_token	*copy;

	copy = copy_token((*head)->next->next);
	copy->next = *head;
	(*head)->prev = copy;
}
