/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_root_pos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:59:55 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 13:00:17 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

int	get_root_pos(t_token *head)
{
	t_operator_data	data;

	data.rtl_pos = -1;
	data.rtl_prec = 100;
	data.ltr_pos = -1;
	data.ltr_prec = 100;
	find_operators(head, &data);
	if (data.rtl_pos != -1)
		return (data.rtl_pos);
	return (data.ltr_pos);
}
