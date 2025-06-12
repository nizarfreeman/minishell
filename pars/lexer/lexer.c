/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:33:46 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/12 18:21:15 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	count(t_token *head)
{
	int	i;

	i = 0;
	while (head)
	{
		if (head->type >= 6 && head->type <= 9)
			i++;
		head = head->next;
	}
	return (i);
}

t_token	*lexer(char *s)
{
	t_token		*head;
	int			i;

	head = NULL;
	if (!check_quotes(s))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (NULL);
	}
	tokenize_input(s, &head);
	revise_args(&head);
	i = count(head);
	while (i--)
		revise_redirections(&head);
	if (!syntax_check(head))
		return (NULL);
	if (revise_heredocs(&head) == -1)
		return (NULL);
	return (head);
}
