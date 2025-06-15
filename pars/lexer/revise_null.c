/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revise_null.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:27:59 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/01 08:28:51 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	revise_null(t_token *head)
{
	while (head)
	{
		if (head->file == NULL && head->type == HERE_ODC)
			return (-1);
		head = head->next;
	}
	return (0);
}
