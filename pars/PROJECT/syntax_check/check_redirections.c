/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:14:48 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/03 21:25:29 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utilities/utilities.h"

int	check_redirection_target(t_token *redir)
{
	t_token	*next;

	if (!redir->next)
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	next = redir->next;
	if (is_invalid_after_redir(next->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}

int	check_redirections(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if (is_redirection_(current->type))
		{
			if (!check_redirection_target(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
