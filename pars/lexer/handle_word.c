/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:12:02 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:15:08 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	get_word_type(char *token)
{
	if (strcmp(token, "echo") == 0)
		return (BUILTIN_ECHO);
	if (strcmp(token, "cd") == 0)
		return (BUILTIN_CD);
	if (strcmp(token, "pwd") == 0)
		return (BUILTIN_PWD);
	if (strcmp(token, "export") == 0)
		return (BUILTIN_EXPORT);
	if (strcmp(token, "unset") == 0)
		return (BUILTIN_UNSET);
	if (strcmp(token, "env") == 0)
		return (BUILTIN_ENV);
	if (strcmp(token, "exit") == 0)
		return (BUILTIN_EXIT);
	return (WORD);
}

int	handle_word(char *s, int i, t_token **head)
{
	int		start;
	t_arg	*arg;

	start = i;
	arg = gc_malloc(sizeof(t_arg));
	while (s[i] && !ft_isspace(s[i]) && !ft_is_operator_char(s[i])
		&& !ft_is_quote(s[i]) && s[i] != '$')
		i++;
	arg->space = 0;
	if (ft_isspace(s[i]))
		arg->space = 1;
	arg->token = ft_strndup(&s[start], i - start);
	arg->quoted = 0;
	arg->file = NULL;
	arg->type = get_word_type(arg->token);
	add_token(head, arg);
	return (i);
}
