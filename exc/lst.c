/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:27:14 by aayache           #+#    #+#             */
/*   Updated: 2025/06/04 14:20:12 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

t_env	*ft_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

int	ft_lstsize(t_env *t_env)
{
	int	i;

	i = 0;
	while (t_env)
	{
		i++;
		t_env = t_env->next;
	}
	return (i);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (lst == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

t_env	*ft_lstnew(t_env **lst, void *content, int f)
{
	t_env	*ret;

	ret = gc_malloc(sizeof(t_env));
	if (!ret)
		return (NULL);
	ret->value = ft_strdup(content);
	if (f == -1)
		ret->tmp = ft_strdup(content);
	ret->f = f;
	ret->next = NULL;
	if (lst)
		ft_lstadd_back(lst, ret);
	return (ret);
}

void	ft_lstclear(t_env **lst)
{
	t_env	*l1;
	t_env	*l2;

	if (lst == NULL)
		return ;
	l1 = *lst;
	while (l1)
	{
		l2 = l1;
		l1 = l1->next;
		free(l2);
	}
	*lst = NULL;
}
