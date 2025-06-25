/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_data_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:43:35 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 03:45:11 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utilities.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	ft_is_operator_char(int c)
{
	return (c == '&' || c == '|' || c == '<' || c == '>'
		|| c == '(' || c == ')' || c == '=' || c == '*');
}
