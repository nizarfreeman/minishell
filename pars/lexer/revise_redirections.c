/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revise_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:55:46 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:58:45 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

void	move_args(t_token *redirection, t_token **head)
{
	t_token	*delimiter;
	t_token	*current;
	t_token	*first_moved;
	t_token	*last_moved;

	if (!validate_move_args_input(redirection))
		return ;
	delimiter = redirection->next;
	current = delimiter->next;
	first_moved = NULL;
	last_moved = NULL;
	collect_word_tokens(current, delimiter, &first_moved, &last_moved);
	if (first_moved)
		insert_before_redirection(first_moved, last_moved, redirection, head);
}

void	revise_redirections(t_token **head)
{
	t_token	*current;

	current = *head;
	while (current)
	{
		if (current->type >= 6 && current->type <= 9)
		{
			move_args(current, head);
			current = current->next;
		}
		else
		{
			current = current->next;
		}
	}
}
