/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_paren_content.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:05:08 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:05:32 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_token	*extract_paren_content(t_token *head)
{
	t_token	*start;
	t_token	*end;
	int		paren_level;

	start = head->next;
	end = head;
	paren_level = 1;
	if (!head || head->type != OPEN_PER)
		return (NULL);
	while (end->next)
	{
		end = end->next;
		if (end->type == OPEN_PER)
			paren_level++;
		else if (end->type == CLOSE_PER)
		{
			paren_level--;
			if (paren_level == 0)
				break ;
		}
	}
	if (paren_level != 0)
		return (NULL);
	return (deep_copy_tokens(start, end));
}
