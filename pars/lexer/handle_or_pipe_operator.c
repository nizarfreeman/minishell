/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_or_pipe_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:50:21 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 15:54:14 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_single_pipe(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = init_operator_arg();
	if (ft_isspace(s[i + 1]))
		arg->space = 1;
	arg->token = gc_strdup("|");
	arg->type = PIPE;
	add_token(head, arg);
	return (i + 1);
}

int	handle_or_pipe_operator(char *s, int i, t_token **head)
{
	t_arg	*arg;

	if (s[i + 1] == '|')
	{
		arg = init_operator_arg();
		if (ft_isspace(s[i + 2]))
			arg->space = 1;
		arg->token = gc_strdup("||");
		arg->type = OR_IF;
		add_token(head, arg);
		return (i + 2);
	}
	return (handle_single_pipe(s, i, head));
}
