/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parentheses_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:21:38 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 03:22:10 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "syntax_check.h"

int	is_invalid_after_open_paren(int type)
{
	return ((type >= 2 && type <= 5) || type == 14);
}

int	is_invalid_before_close_paren(int type)
{
	return ((type >= 2 && type <= 9) || type == 13);
}

int	is_valid_after_close_paren(int type)
{
	return ((type >= 2 && type <= 9) || type == 14);
}
