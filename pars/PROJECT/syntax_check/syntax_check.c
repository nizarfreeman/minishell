/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:01:23 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 03:02:11 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "syntax_check.h"

int	syntax_check(t_token *head)
{
	if (!check_start_end(head))
		return (0);
	if (!check_parentheses(head))
		return (0);
	if (!check_redirections(head))
		return (0);
	if (!check_operators(head))
		return (0);
	if (!check_heredoc(head))
		return (0);
	return (1);
}
