/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:02:46 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/03 21:25:59 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utilities/utilities.h"

int	is_valid_heredoc_delimiter(int type)
{
	return (type == 0 || type == 10 || type == 11);
}

int	check_heredoc(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if (current->type == 9)
		{
			if (!current->next)
			{
				ft_putstr_fd("minishell : syntax error !\n", 2);
				*(get_exit_status(NULL)) = 2;
				return (0);
			}
			if (!is_valid_heredoc_delimiter(current->next->type))
			{
				ft_putstr_fd("minishell : syntax error !\n", 2);
				*(get_exit_status(NULL)) = 2;
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
