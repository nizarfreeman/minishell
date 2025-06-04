/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 09:04:50 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/03 16:07:04 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	*process_quotes(char *str, char *ret, int len)
{
	int	i;
	int	j;
	int	in_dquote;
	int	in_squote;

	(1) && (i = 0, j = 0, in_dquote = 0, in_squote = 0);
	while (i < len)
	{
		if (str[i] == '\"' && !in_squote)
		{
			in_dquote = !in_dquote;
			i++;
		}
		else if (str[i] == '\'' && !in_dquote)
		{
			in_squote = !in_squote;
			i++;
		}
		else
			ret[j++] = str[i++];
	}
	ret[j] = '\0';
	return (ret);
}

char	*unquote_string(char *str)
{
	int		len;
	char	*ret;

	len = ft_strlen(str);
	ret = gc_malloc(len + 1);
	if (!ret)
		return (NULL);
	return (process_quotes(str, ret, len));
}

char	*ft_strrev(char *str)
{
	int		i;
	int		j;
	char	tmp;

	if (!str)
		return (NULL);
	i = 0;
	j = strlen(str) - 1;
	while (j > i)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
	return (str);
}

// char	*ft_itoa(int nbr)
// {
// 	int		i;
// 	int		neg;
// 	char	*tmp;

// 	(1) && (i = 0, neg = 0);
// 	if (nbr == 0)
// 		return (gc_strdup("0"));
// 	if (nbr == -2147483648)
// 		return (gc_strdup("-2147483648"));
// 	tmp = gc_malloc(sizeof(char) * 12);
// 	if (tmp == NULL)
// 		return (NULL);
// 	ft_memset(tmp, 0, 12);
// 	if (nbr < 0)
// 		(1) && (neg = 1, nbr = -nbr);
// 	while (nbr > 0)
// 	{
// 		tmp[i++] = (nbr % 10) + '0';
// 		nbr /= 10;
// 	}
// 	if (neg)
// 		tmp[i++] = '-';
// 	tmp[i] = '\0';
// 	return (ft_strrev(tmp));
// }
