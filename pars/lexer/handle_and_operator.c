/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_and_operator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:47:04 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 15:49:30 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

t_arg	*init_operator_arg(void)
{
	t_arg	*arg;

	arg = gc_malloc(sizeof(t_arg));
	arg->space = 0;
	arg->quoted = 0;
	arg->file = NULL;
	return (arg);
}

void	set_space_if_needed(t_arg *arg, char *s, int pos)
{
	if (ft_isspace(s[pos]))
		arg->space = 1;
}

int	handle_and_operator(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = init_operator_arg();
	if (s[i + 1] == '&')
	{
		set_space_if_needed(arg, s, i + 2);
		arg->token = gc_strdup("&&");
		arg->type = AND_IF;
		add_token(head, arg);
		return (i + 2);
	}
	set_space_if_needed(arg, s, i + 1);
	arg->token = gc_strdup("&");
	arg->type = WORD;
	add_token(head, arg);
	return (i + 1);
}
