/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:13:13 by aayache           #+#    #+#             */
/*   Updated: 2025/06/28 17:14:29 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*ft_itoa1(int n)
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

char	*expand2_1(char *str, t_env *t_envr, int *ex)
{
	char	*ret;
	char	*s;

	s = NULL;
	ret = NULL;
	s = str;
	while (*str && *(str + 1) != '$')
		str++;
	if (*str && *(str + 1) == '$')
		ret = ft_strjoin(word(s, str + 1), expand2(str + 1, t_envr, ex));
	else
		ret = word(s, str);
	return (ret);
}

char	*expand2_3(char *str, t_env *t_envr, int *ex)
{
	char	*ret;
	char	*s;	

	s = str;
	while ((*str && ((*str >= 'a' && *str <= 'z')
				|| (*str >= 'A' && *str <= 'Z')
				|| *str == '_' || (*str >= '0' && *str <= '9'))))
		str++;
	ret = ft_strjoin(get_value(t_envr, ft_strjoin(word(s, str), "="), 0),
			expand2(str, t_envr, ex));
	return (ret);
}

char	*expand2_2(char *str, t_env *t_envr, int *ex)
{
	char	*ret;

	ret = NULL;
	if (expand_valid(*(str + 1))
		&& !((*(str + 1) >= '0') && (*(str + 1) <= '9')))
	{
		ret = ft_strjoin(ft_strdup("$"), expand2(str + 1, t_envr, ex));
		return (ret);
	}
	str++;
	if (*str && *str == '?')
	{
		ret = ft_strjoin(ft_itoa(*ex), expand2(str + 1, t_envr, ex));
		return (ret);
	}
	if (*str && ((*str >= '0' && *str <= '9') || *str == '*'))
	{
		ret = ft_strjoin(ft_strdup(""), expand2(str + 1, t_envr, ex));
		return (ret);
	}
	else
		return (expand2_3(str, t_envr, ex));
	return (NULL);
}

char	*expand2(char *str, t_env *t_envr, int *ex)
{
	if (*str != '$')
		return (expand2_1(str, t_envr, ex));
	if (*str == '$')
		return (expand2_2(str, t_envr, ex));
	return (NULL);
}
