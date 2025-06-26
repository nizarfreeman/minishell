/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:31:09 by aayache           #+#    #+#             */
/*   Updated: 2025/06/26 22:46:07 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*rm_q(char *tmp)
{
	char	*ret;
	int		p;

	ret = NULL;
	while (tmp && *tmp)
	{
		if (*tmp && *tmp == '\'')
		{
			ret = ft_strjoin(ret, creat_word(++tmp, 1, '\'', &p));
			tmp += p;
		}
		else if (*tmp && *tmp == '"')
		{
			ret = ft_strjoin(ret, creat_word(++tmp, 1, '"', &p));
			tmp += p;
		}
		else if (*tmp)
		{
			ret = ft_strjoin(ret, creat_word(tmp, 0, 0, &p));
			tmp += p;
		}
	}
	return (ret);
}

void	export_expand2(char *tmp, int *ex, t_env **ret1, t_env *t_envr)
{
	char	*ret;
	int		p;

	ret = NULL;
	while (tmp && *tmp)
	{
		if (*tmp && *tmp == '\'')
		{
			ret = ft_strjoin(ret, creat_word(++tmp, 1, '\'', &p));
			tmp += p;
		}
		else if (*tmp && *tmp == '"')
		{
			ret = ft_strjoin(ret, expand2(creat_word(++tmp, 1, '"', &p), t_envr,
						ex));
			tmp += p;
		}
		else if (*tmp)
		{
			ret = ft_strjoin(ret, expand2(creat_word(tmp, 0, 0, &p), t_envr,
						ex));
			tmp += p;
		}
	}
	ft_lstnew(ret1, ret, 0);
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
