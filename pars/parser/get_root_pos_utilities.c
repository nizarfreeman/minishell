/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_root_pos_utilities.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:00:29 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:01:09 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

int	get_operator_precedence(int token_type)
{
	if (token_type == OR_IF || token_type == AND_IF)
		return (1);
	else if (token_type == PIPE)
		return (3);
	else if (token_type == REDIRECTION_OUT || token_type == REDIRECTION_IN
		|| token_type == APPEND || token_type == HERE_ODC)
		return (4);
	return (100);
}

void	update_left_to_right(t_operator_data *data,
		int current_pos, int current_precedence)
{
	if (data->ltr_pos == -1 || current_precedence < data->ltr_prec)
	{
		data->ltr_pos = current_pos;
		data->ltr_prec = current_precedence;
	}
}

void	update_right_to_left(t_operator_data *data,
		int current_pos, int current_precedence)
{
	if (data->rtl_pos == -1 || current_precedence <= data->rtl_prec)
	{
		data->rtl_pos = current_pos;
		data->rtl_prec = current_precedence;
	}
}

void	process_operator(t_token *curr, int position, t_operator_data *data)
{
	int	curr_precedence;

	curr_precedence = get_operator_precedence(curr->type);
	if (curr_precedence == 4)
		update_left_to_right(data, position, curr_precedence);
	else if (curr_precedence < 100)
		update_right_to_left(data, position, curr_precedence);
}

void	find_operators(t_token *head, t_operator_data *data)
{
	t_token	*curr;
	int		position;
	int		paren_level;

	curr = head;
	position = 0;
	paren_level = 0;
	while (curr)
	{
		if (curr->type == OPEN_PER)
			paren_level++;
		else if (curr->type == CLOSE_PER)
			paren_level--;
		if (paren_level == 0)
			process_operator(curr, position, data);
		curr = curr->next;
		position++;
	}
}
