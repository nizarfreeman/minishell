/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:07:52 by aayache           #+#    #+#             */
/*   Updated: 2025/05/30 20:31:26 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	*ft_malloc(int size)
{
	static t_gc	*head = NULL;
	void		*ptr;
	t_gc		*new_node;

	if (size == -1)
		return ((void *)&head);
	ptr = malloc(size);
	if (!ptr)
	{
		gc_free();
		ft_free();
		exit(1);
	}
	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		free(ptr);
		gc_free();
		ft_free();
		exit(1);
	}
	new_node->ptr = ptr;
	new_node->next = head;
	head = new_node;
	return (ptr);
}

void	ft_free(void)
{
	t_gc	**head_ptr;
	t_gc	*curr;
	t_gc	*tmp;

	head_ptr = (t_gc **)ft_malloc(-1);
	curr = *head_ptr;
	while (curr)
	{
		tmp = curr->next;
		free(curr->ptr);
		free(curr);
		curr = tmp;
	}
	*head_ptr = NULL;
}

// void    gc_free(void);

// void    *gc_malloc(int size)
// {
//     static t_gc *head = NULL;
//     void *ptr;
//     t_gc *new_node;

//     if (size == -1)
//         return ((void *)&head);
//     ptr = malloc(size);
//     if (!ptr)
//     {
//         gc_free();
//         exit(1);
//     }
//     new_node = malloc(sizeof(t_gc));
//     if (!new_node)
//     {
//         free(ptr);
//         gc_free();
//         exit(1);
//     }
//     new_node->ptr = ptr;
//     new_node->next = head;
//     head = new_node;
//     return (ptr);
// }

// void    gc_free(void)
// {
//     t_gc **head_ptr;
//     t_gc *curr;
//     t_gc *tmp;

//     head_ptr = (t_gc **)gc_malloc(-1);
//     curr = *head_ptr;
//     while (curr)
//     {
//         tmp = curr->next;
//         free(curr->ptr);
//         free(curr);
//         curr = tmp;
//     }
//     *head_ptr = NULL;
// }
