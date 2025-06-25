/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 04:06:26 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 15:37:50 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utilities.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*src;

	src = (unsigned char *) s;
	while (n--)
		*src++ = (unsigned char) c;
	return (s);
}

char	*ft_strndup(char *s, size_t n)
{
	char	*result;

	result = gc_malloc(n + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, s, n);
	result[n] = '\0';
	return (result);
}
