/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_before_redirection.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:50:13 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 16:55:28 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

void	insert_before_redirection(t_token *first_moved, t_token *last_moved,
			t_token *redirection, t_token **head)
{
	last_moved->next = redirection;
	first_moved->prev = redirection->prev;
	if (redirection->prev)
		redirection->prev->next = first_moved;
	else
		*head = first_moved;
	redirection->prev = last_moved;
}
