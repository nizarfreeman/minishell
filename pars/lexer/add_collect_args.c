/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_collect_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:46:48 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:49:43 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

void	add_to_moved_list(t_token *token, t_token **first, t_token **last)
{
	if (!*first)
	{
		*first = token;
		*last = token;
	}
	else
	{
		(*last)->next = token;
		token->prev = *last;
		*last = token;
	}
}

void	collect_word_tokens(t_token *start, t_token *delimiter,
			t_token **first_moved, t_token **last_moved)
{
	t_token	*current;
	t_token	*next_token;

	current = start;
	while (current && current->type == WORD)
	{
		next_token = current->next;
		remove_token_from_position(current, delimiter);
		add_to_moved_list(current, first_moved, last_moved);
		current = next_token;
	}
}
