/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parentheses.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:17:07 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/04 10:16:15 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../utilities/utilities.h"

int	handle_open_paren(t_token *current, int *count)
{
	(*count)++;
	if (current->next && current->next->type == 14)
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	if (current->next && is_invalid_after_open_paren(current->next->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	if (current->prev && current->prev->type == 0)
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}

int	handle_close_paren(t_token *current, int *count)
{
	(*count)--;
	if (*count < 0)
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	if (current->prev && is_invalid_before_close_paren(current->prev->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	if (current->next && !is_valid_after_close_paren(current->next->type))
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}

int	check_parentheses(t_token *head)
{
	t_token	*current;
	int		count;

	(1) && (count = 0, current = head);
	while (current)
	{
		if (current->type == 13)
		{
			if (!handle_open_paren(current, &count))
				return (0);
		}
		else if (current->type == 14)
		{
			if (!handle_close_paren(current, &count))
				return (0);
		}
		current = current->next;
	}
	if (count > 0)
	{
		ft_putstr_fd("minishell : syntax error !\n", 2);
		*(get_exit_status(NULL)) = 2;
		return (0);
	}
	return (1);
}
