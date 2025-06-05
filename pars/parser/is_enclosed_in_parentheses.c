/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_enclosed_in_parentheses.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:02:27 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:02:43 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

int	is_enclosed_in_parentheses(t_token *head)
{
	t_token	*first;
	t_token	*last;

	if (!head || !head->next)
		return (0);
	first = head;
	last = head;
	while (last->next)
		last = last->next;
	if (first->type != OPEN_PER || last->type != CLOSE_PER)
		return (0);
	return (check_paren_balance(head, last));
}
