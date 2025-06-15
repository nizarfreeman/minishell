/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_paren_balance.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:01:45 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:02:03 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

int	check_paren_balance(t_token *head, t_token *last)
{
	t_token	*curr;
	int		level;

	level = 0;
	curr = head;
	while (curr)
	{
		if (curr->type == OPEN_PER)
			level++;
		else if (curr->type == CLOSE_PER)
		{
			level--;
			if (level == 0 && curr != last)
				return (0);
		}
		curr = curr->next;
	}
	return (level == 0);
}
