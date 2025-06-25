/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:17:22 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:19:32 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	handle_operators(char *s, int i, t_token **head)
{
	if (s[i] == '(' || s[i] == ')')
		return (handle_parentheses(s, i, head));
	if (s[i] == '&')
		return (handle_and_operator(s, i, head));
	if (s[i] == '|')
		return (handle_or_pipe_operator(s, i, head));
	if (s[i] == '>')
		return (handle_output_redirection(s, i, head));
	if (s[i] == '<')
		return (handle_input_redirection(s, i, head));
	if (s[i] == '=')
		return (handle_assignment(s, i, head));
	if (s[i] == '*')
		return (handle_wildcard(s, i, head));
	if (s[i] == '$')
		return (handle_dollar(s, i, head));
	return (handle_word(s, i, head));
}

void	tokenize_input(char *s, t_token **head)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_isspace(s[i]))
			i++;
		else if (ft_is_quote(s[i]))
			i = handle_quotes(s, i, head, 1);
		else
			i = handle_operators(s, i, head);
	}
}
