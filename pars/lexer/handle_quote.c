/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:40:42 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 15:44:13 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	find_closing_quote(char *s, int start, char quote)
{
	int	i;

	i = start + 1;
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;
	return (i);
}

int	get_quote_token_type(char quote, int preserve_quotes)
{
	if (preserve_quotes)
		return (WORD);
	if (quote == '\'')
		return (S_QUOTE);
	return (D_QUOTE);
}

t_arg	*init_quote_arg(void)
{
	t_arg	*arg;

	arg = gc_malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->space = 0;
	arg->quoted = 1;
	arg->file = NULL;
	return (arg);
}

void	set_arg_properties(t_arg *arg, char *s, int start, int end)
{
	arg->token = ft_strndup(&s[start], end - start);
	if (ft_isspace(s[end]))
		arg->space = 1;
}

int	handle_quotes(char *s, int i, t_token **head, int preserve_quotes)
{
	char	quote;
	int		start;
	int		end;
	t_arg	*arg;

	quote = s[i];
	start = i;
	end = find_closing_quote(s, i, quote);
	arg = init_quote_arg();
	set_arg_properties(arg, s, start, end);
	arg->type = get_quote_token_type(quote, preserve_quotes);
	if (!preserve_quotes)
		arg->quoted = 1;
	add_token(head, arg);
	return (end);
}
