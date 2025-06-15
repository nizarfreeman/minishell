/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parentheses.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:02:53 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:04:46 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_close_parenthesis(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = init_operator_arg();
	if (ft_isspace(s[i + 1]))
		arg->space = 1;
	arg->token = gc_strdup(")");
	arg->type = CLOSE_PER;
	add_token(head, arg);
	return (i + 1);
}

int	handle_parentheses(char *s, int i, t_token **head)
{
	t_arg	*arg;

	if (s[i] == '(')
	{
		arg = init_operator_arg();
		if (ft_isspace(s[i + 1]))
			arg->space = 1;
		arg->token = gc_strdup("(");
		arg->type = OPEN_PER;
		add_token(head, arg);
		return (i + 1);
	}
	if (s[i] == ')')
		return (handle_close_parenthesis(s, i, head));
	return (i);
}
