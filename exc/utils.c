/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:25:31 by aayache           #+#    #+#             */
/*   Updated: 2025/06/27 22:13:30 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	*get_exit_status(int *ex)
{
	static int	*status;

	if (!ex)
		return (status);
	status = ex;
	return (NULL);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 && *s2 && (char)*s1 == (char)*s2)
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return ((char)*s1 - (char)*s2);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*s;
	unsigned char	*d;

	if (dst == (void *)0 && src == (void *)0)
		return (dst);
	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	while (n)
	{
		*(d++) = *(s++);
		n--;
	}
	return (dst);
}

int	count_digits(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(long long n)
{
	char	*str;
	int		digits;
	long	num;

	digits = count_digits(n);
	str = (char *)gc_malloc(sizeof(char) * (digits + 1));
	if (!str)
		return (NULL);
	num = n;
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	if (num == 0)
		str[0] = '0';
	str[digits] = '\0';
	while (num > 0)
	{
		str[--digits] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}
