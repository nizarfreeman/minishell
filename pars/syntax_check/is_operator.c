/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:27:04 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 03:28:19 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "syntax_check.h"

int	is_operator_start(int type)
{
	return (type >= 2 && type <= 5);
}

int	is_operator_end(int type)
{
	return (type >= 2 && type <= 5);
}

int	is_redirection_(int type)
{
	return (type >= 6 && type <= 9);
}

int	is_invalid_after_redir(int type)
{
	return ((type >= 2 && type <= 5)
		|| (type >= 6 && type <= 9) || (type == 14));
}
