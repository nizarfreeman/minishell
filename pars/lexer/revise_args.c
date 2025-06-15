/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revise_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:04:33 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 17:13:11 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

void	process_token_joining(t_token *current)
{
	char	*combined;
	t_token	*last_joined;
	t_token	*next_token;

	combined = build_combined_string(current, &last_joined);
	if (!combined)
		return ;
	next_token = last_joined->next;
	if (last_joined != current)
	{
		update_current_token(current, combined, last_joined, next_token);
		remove_tokens_between(current, next_token);
	}
}

void	revise_args(t_token **head)
{
	t_token	*current;

	if (!validate_revise_input(head))
		return ;
	current = *head;
	while (current)
	{
		if (is_joinable_type(current->type) && current->space == 0)
			process_token_joining(current);
		current = current->next;
	}
}
