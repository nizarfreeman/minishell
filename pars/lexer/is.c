/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:25:52 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:29:06 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	is_redirection(t_token *token)
{
	return (token && (token->type >= REDIRECTION_OUT
			&& token->type <= HERE_ODC));
}

int	is_command(t_token *token)
{
	return (token && (token->type >= BUILTIN_ECHO
			&& token->type <= BUILTIN_EXIT));
}

int	is_pipe_or_logical(t_token *token)
{
	return (token && (token->type == PIPE || token->type == AND_IF
			|| token->type == OR_IF || token->type == AND));
}

int	is_argument(t_token *token)
{
	return (token && (token->type == WORD || (token->type >= BUILTIN_ECHO
				&& token->type <= BUILTIN_EXIT)));
}

int	is_redirection_target(t_token *token)
{
	return (token && token->prev && is_redirection(token->prev));
}
