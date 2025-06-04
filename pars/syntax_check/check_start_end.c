/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_start_end.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:23:03 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/03 21:25:14 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utilities/utilities.h"

int	check_start_end(t_token *head)
{
	t_token	*tail;

	if (!head)
		return (1);
	tail = head;
	while (tail && tail->next)
		tail = tail->next;
	if (is_operator_start(head->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	if (tail && is_operator_end(tail->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}
