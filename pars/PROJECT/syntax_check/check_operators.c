/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:04:56 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/03 21:25:50 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utilities/utilities.h"

int	check_operator_sequence(t_token *current)
{
	if (current->next && is_operator_start(current->next->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}

int	check_operator_before_close(t_token *current)
{
	if (current->next && current->next->type == 14)
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}

void	line_too_long(void)
{
	ft_putstr_fd("minishell : syntax error !\n", 2);
	*(get_exit_status(NULL)) = 2;
}

int	check_operators(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if (is_operator_start(current->type))
		{
			if (!check_operator_sequence(current))
				return (0);
			if (!check_operator_before_close(current))
				return (0);
			if (!current->next)
			{
				line_too_long();
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
