/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:24:53 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 13:25:14 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*str;

	if (!s1)
		return (NULL);
	(1 == 1) && (i = ft_strlen(s1), str = gc_malloc(i + 1));
	if (str == NULL)
		return (NULL);
	ft_memcpy(str, s1, i);
	str[i] = 0;
	return (str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (char)*s1 == (char)*s2)
	{
		s1++;
		s2++;
	}
	return ((char)*s1 - (char)*s2);
}
