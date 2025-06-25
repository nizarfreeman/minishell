/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:08:52 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:10:44 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_single_dollar(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = init_operator_arg();
	if (!arg)
		return (i);
	if (ft_isspace(s[i + 1]))
		arg->space = 1;
	arg->token = gc_strdup("$");
	arg->type = WORD;
	add_token(head, arg);
	return (i + 1);
}

int	handle_dollar(char *s, int i, t_token **head)
{
	t_arg	*arg;

	if (s[i + 1] == '?')
	{
		arg = init_operator_arg();
		if (!arg)
			return (i);
		if (ft_isspace(s[i + 2]))
			arg->space = 1;
		arg->token = gc_strdup("$?");
		arg->type = WORD;
		add_token(head, arg);
		return (i + 2);
	}
	return (handle_single_dollar(s, i, head));
}
