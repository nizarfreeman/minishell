/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revise_args_follow.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:15:48 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 17:22:07 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	is_joinable_type(t_tokentype type)
{
	if (type == WORD || type == S_QUOTE || type == D_QUOTE || type == DOLLAR)
		return (1);
	return (0);
}

int	validate_revise_input(t_token **head)
{
	if (!head || !*head)
		return (0);
	return (1);
}

char	*build_combined_string(t_token *start, t_token **last_joined)
{
	char	*combined;
	char	*temp;
	t_token	*next_token;

	combined = gc_strdup(start->token);
	next_token = start->next;
	*last_joined = start;
	while (next_token && is_joinable_type(next_token->type)
		&& (*last_joined)->space == 0)
	{
		temp = combined;
		combined = ft_strjoin(temp, next_token->token);
		if (!combined)
			return (NULL);
		*last_joined = next_token;
		next_token = next_token->next;
	}
	return (combined);
}

void	remove_tokens_between(t_token *current, t_token *next_token)
{
	t_token	*to_remove;
	t_token	*temp;

	to_remove = current->next;
	while (to_remove && to_remove != next_token)
	{
		temp = to_remove->next;
		if (to_remove->prev)
			to_remove->prev->next = to_remove->next;
		if (to_remove->next)
			to_remove->next->prev = to_remove->prev;
		to_remove->prev = NULL;
		to_remove->next = NULL;
		to_remove = temp;
	}
}

void	update_current_token(t_token *current, char *combined,
			t_token *last_joined, t_token *next_token)
{
	current->token = combined;
	current->type = WORD;
	current->space = last_joined->space;
	current->next = next_token;
	if (next_token)
		next_token->prev = current;
}
