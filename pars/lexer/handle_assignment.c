/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_assignment.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:05:59 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:07:03 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_assignment(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = gc_malloc(sizeof(t_arg));
	arg->space = 0;
	arg->quoted = 0;
	arg->file = NULL;
	if (ft_isspace(s[i + 1]))
		arg->space = 1;
	arg->token = gc_strdup("=");
	arg->type = WORD;
	add_token(head, arg);
	return (i + 1);
}
