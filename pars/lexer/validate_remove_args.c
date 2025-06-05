/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_remove_args.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:45:46 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:46:26 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	validate_move_args_input(t_token *redirection)
{
	if (!redirection || !redirection->next || !redirection->next->next)
		return (0);
	return (1);
}

void	remove_token_from_position(t_token *token, t_token *delimiter)
{
	delimiter->next = token->next;
	if (token->next)
		token->next->prev = delimiter;
	token->next = NULL;
	token->prev = NULL;
}
