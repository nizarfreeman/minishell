/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:31:09 by aayache           #+#    #+#             */
/*   Updated: 2025/06/25 17:36:16 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	export_expand2(char *tmp, int *ex, t_env **ret, t_env *t_envr)
{
	char	**tmp1;

	tmp1 = pre_expand(lst_to_arr2(ft_lstnew(NULL, tmp, 0)), t_envr, ex);
	tmp = NULL;
	while (tmp1 && *tmp1)
		tmp = ft_strjoin(tmp, *tmp1++);
	ft_lstnew(ret, tmp, 0);
}

char	**export_expand(char **s, t_env *t_envr, int *ex)
{
	char	*tmp;
	char	**tmp1;
	t_env	*ret;

	ret = NULL;
	while (*s)
	{
		tmp = *s;
		if (*tmp == '$')
		{
			tmp1 = pre_expand(lst_to_arr2(ft_lstnew(NULL, tmp, 0)), t_envr, ex);
			while (tmp1 && *tmp1)
			{
				if (**tmp1)
					ft_lstnew(&ret, *tmp1, 0);
				tmp1++;
			}
		}
		else
			export_expand2(tmp, ex, &ret, t_envr);
		s++;
	}
	return (lst_to_arr2(ret));
}
