/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_arg.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:06:52 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:07:02 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_arg	*create_token_arg(t_token *curr)
{
	t_arg	*arg;

	arg = gc_malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->token = gc_strdup(curr->token);
	arg->type = curr->type;
	arg->quoted = curr->quoted;
	arg->space = curr->space;
	arg->file = NULL;
	if (curr->file)
		arg->file = gc_strdup(curr->file);
	return (arg);
}
