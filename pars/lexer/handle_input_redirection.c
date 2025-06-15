/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:00:23 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:02:02 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_input_redirection(char *s, int i, t_token **head)
{
	t_arg	*arg;

	arg = gc_malloc(sizeof(t_arg));
	arg->space = 0;
	arg->quoted = 0;
	arg->file = NULL;
	if (s[i + 1] == '<')
	{
		if (ft_isspace(s[i + 2]))
			arg->space = 1;
		arg->token = gc_strdup("<<");
		arg->type = HERE_ODC;
		add_token(head, arg);
		return (i + 2);
	}
	else
	{
		if (ft_isspace(s[i + 1]))
			arg->space = 1;
		arg->token = gc_strdup("<");
		arg->type = REDIRECTION_IN;
		add_token(head, arg);
		return (i + 1);
	}
}
