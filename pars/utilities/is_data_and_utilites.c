/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_data_and_utilites.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:45:27 by nfaska            #+#    #+#             */
/*   Updated: 2025/05/31 03:53:58 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utilities.h"

int	ft_is_quote(int c)
{
	return (c == '\'' || c == '\"');
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*gc_strdup(char *s)
{
	size_t	len;
	char	*result;

	if (!s)
		return (NULL);
	len = ft_strlen((char *)s);
	result = gc_malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, s);
	return (result);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;
	size_t	i;
	size_t	j;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	res = gc_malloc(len1 + len2 + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && j < len1)
		res[i++] = s1[j++];
	j = 0;
	while (s2 && j < len2)
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i <= n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i <= n)
		dest[i++] = '\0';
	return (dest);
}
