/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:56:58 by aayache           #+#    #+#             */
/*   Updated: 2025/06/25 18:38:04 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

t_env	*ft_lstnew1(t_env **lst, void *content, int f)
{
	t_env	*ret;

	ret = ft_malloc(sizeof(t_env));
	if (!ret)
		return (NULL);
	ret->value = ft_strdup1(content);
	if (f == -1)
		ret->tmp = ft_strdup1(content);
	ret->f = f;
	ret->next = NULL;
	if (lst)
		ft_lstadd_back(lst, ret);
	return (ret);
}

void	search_replace1(t_env *t_env, char *key, char *rep)
{
	char	*tmp;

	tmp = ft_strjoin1(ft_strdup1(key), ft_strdup("="));
	while (t_env && ft_strcmp(key, t_env->value) && ft_strncmp(t_env->value,
			tmp, ft_strlen(tmp)))
		t_env = t_env->next;
	if (!t_env)
		return ;
	if (rep)
	{
		t_env->value = NULL;
		t_env->value = ft_strjoin1(tmp, rep);
	}
	t_env->f = 1;
}

char	*ft_strjoin1(char *s1, char *s2)
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
	res = ft_malloc(len1 + len2 + 1);
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

char	*ft_strdup1(const char *s1)
{
	size_t	i;
	char	*str;

	if (!s1)
		return (NULL);
	(1 == 1) && (i = ft_strlen((char *)s1), str = ft_malloc(i + 1));
	ft_memcpy(str, s1, i);
	str[i] = 0;
	return (str);
}
