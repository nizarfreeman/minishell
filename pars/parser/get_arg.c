/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:58:22 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/02 12:58:46 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

char	**get_args(t_token *head)
{
	t_token	*tmp;
	int		i;
	char	**ret;
	int		y;

	(1) && (i = 0, tmp = head);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	ret = gc_malloc(sizeof(char *) * (i + 1));
	if (!ret)
		return (NULL);
	(1) && (y = 0, tmp = head);
	while (y < i && tmp)
	{
		ret[y] = gc_strdup(tmp->token);
		if (!ret[y])
			return (NULL);
		tmp = tmp->next;
		y++;
	}
	ret[y] = NULL;
	return (ret);
}
