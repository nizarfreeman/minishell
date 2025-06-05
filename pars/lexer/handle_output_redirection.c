/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_output_redirection.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:02 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 15:57:48 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_single_output(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = init_operator_arg();
	if (ft_isspace(s[i + 1]))
		arg->space = 1;
	arg->token = gc_strdup(">");
	arg->type = REDIRECTION_OUT;
	add_token(head, arg);
	return (i + 1);
}

int	handle_output_redirection(char *s, int i, t_token **head)
{
	t_arg	*arg;

	if (s[i + 1] == '>')
	{
		arg = init_operator_arg();
		if (ft_isspace(s[i + 2]))
			arg->space = 1;
		arg->token = gc_strdup(">>");
		arg->type = APPEND;
		add_token(head, arg);
		return (i + 2);
	}
	return (handle_single_output(s, i, head));
}
