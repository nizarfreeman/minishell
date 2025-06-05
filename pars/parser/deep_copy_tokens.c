/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deep_copy_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:03:47 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:04:41 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_token	*deep_copy_tokens(t_token *start, t_token *end)
{
	t_token	*result;
	t_token	*current;
	t_arg	*arg;

	result = NULL;
	current = start;
	while (current && current != end)
	{
		arg = gc_malloc(sizeof(t_arg));
		arg->token = gc_strdup(current->token);
		arg->type = current->type;
		arg->quoted = current->quoted;
		arg->space = current->space;
		arg->file = NULL;
		if (current->file)
			arg->file = gc_strdup(current->file);
		add_token(&result, arg);
		current = current->next;
	}
	return (result);
}
