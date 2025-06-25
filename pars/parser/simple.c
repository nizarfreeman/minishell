/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:31:36 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 14:32:11 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../utilities/utilities.h"

int	is_simple(t_token *head)
{
	int	i;

	while (head->next)
	{
		i = head->type;
		if (i >= 2 && i <= 9)
			return (0);
		head = head->next;
	}
	return (1);
}
