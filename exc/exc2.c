/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:56:20 by aayache           #+#    #+#             */
/*   Updated: 2025/06/27 22:38:02 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	expand5(t_env **list, char *tmp, t_env *t_envr)
{
	char	*ret;
	char	**tmp1;
	int		p;

	ret = NULL;
	while (*tmp)
	{
		if (*tmp == '"' || *tmp == '\'')
			ret = ft_strjoin(ret, expand_quotes(t_envr, &p, &(tmp)));
		if (*tmp)
		{
			if (expand4(&tmp1, t_envr, &tmp))
				ret = ft_strjoin(ret, quote_string(*tmp1, 0));
			else if (append_to_lst(&ret, list, *tmp1, tmp))
				tmp += expand3(&ret, tmp1, list, t_envr);
		}
		if (!*tmp)
		{
			ft_lstnew(list, ret, 0);
			ret = NULL;
		}
	}
	return (1);
}

int	expand4(char ***tmp1, t_env *t_envr, char **tmp)
{
	int	*ex;
	int	p;

	ex = get_exit_status(NULL);
	*tmp1 = gc_malloc(sizeof(char *) * 2);
	(*tmp1)[0] = creat_word(*tmp, 0, 0, &p);
	(*tmp1)[1] = NULL;
	*tmp1 = expand(*tmp1, t_envr, ex);
	if (((*tmp1)[0] && !(*tmp1)[1]) || !(*tmp1)[0])
	{
		*tmp += p;
		return (1);
	}
	return (0);
}

char	*expand_quotes(t_env *t_envr, int *p, char **tmp1)
{
	char	*ret;
	int		*ex;
	char	*tmp;

	tmp = *tmp1;
	ex = get_exit_status(NULL);
	ret = NULL;
	if (*tmp == '"')
	{
		ret = ft_strjoin(ret, expand2(quote_string(creat_word(++tmp,  1, '"', p),
						0), t_envr, ex));
		*tmp1 += *p + 1;
		return (ret);
	}
	if (*tmp == '\'')
	{
		ret = ft_strjoin(ret, quote_string(creat_word(++tmp, 1, '\'', p),
					0));
		*tmp1 += *p + 1;
		return (ret);
	}
	return (NULL);
}

int	append_to_lst(char **ret, t_env **list, char *tmp1, char *tmp)
{
	*ret = ft_strjoin(*ret, quote_string(tmp1, 0));
	ft_lstnew(list, *ret, 0);
	*ret = tmp;
	return (1);
}

int	expand3(char **ret, char **tmp1, t_env **list, t_env *t_envr)
{
	int		p;
	int		*ex;
	char	*tmp;

	tmp = *ret;
	ex = get_exit_status(NULL);
	if (tmp1)
		tmp1++;
	if (tmp)
		*ret = expand2(creat_word(tmp, 0, 0, &p), t_envr, ex);
	if (ret && *ret && ((**ret + ft_strlen(*ret) - 1) == ' ' || (**ret
				+ ft_strlen(*ret) - 1) == '\t'))
	{
		while (*tmp1)
			ft_lstnew(list, quote_string(*tmp1++, 0), 0);
		tmp1 = NULL;
		*ret = NULL;
	}
	else
	{
		while (*tmp1 && *(tmp1 + 1))
			ft_lstnew(list, quote_string(*tmp1++, 0), 0);
		*ret = *tmp1;
	}
	return (p);
}
