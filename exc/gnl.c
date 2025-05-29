/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:26:55 by aayache           #+#    #+#             */
/*   Updated: 2025/05/29 12:26:56 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*freep(char **s1, char **s2, int i, int fd)
{
	if (i == 0)
	{
		if (fd >= 0 && fd <= 10240)
		{
			free(*s1);
			*s1 = NULL;
		}
		return (NULL);
	}
	if (i == 1)
	{
		free(*s2);
		*s2 = NULL;
		if (s1 && *s1 && !*(*s1))
		{
			free(*s1);
			*s1 = NULL;
			return (NULL);
		}
		return (*s1);
	}
	return (NULL);
}

char	*search(char **tmp, char **s)
{
	char	*ret;
	char	*tmp1;
	int		i;
	char	*a;

	i = 0;
	(1) && (*tmp = ft_strjoin(*s, *tmp), tmp1 = ft_strdup(*tmp));
	(1) && (a = tmp1, ret = NULL);
	while (tmp1 && *tmp1)
	{
		if (*tmp1 == '\n')
		{
			tmp1++;
			ret = ft_strdup(tmp1);
			(*tmp)[i + 1] = '\0';
			free(a);
			return (ret);
		}
		tmp1++;
		i++;
	}
	free(a);
	return (ret);
}

char	*_rest(char **s, int fd)
{
	char	*tmp;
	ssize_t	i;
	char	*ret;

	tmp = NULL;
	(1) && (tmp = ft_strdup(*s), ret = NULL);
	free(*s);
	(1) && (*s = NULL, *s = search(&tmp, s));
	if (*s)
		return (ft_strjoin(ret, tmp));
	ret = ft_strjoin(ret, tmp);
	while (1)
	{
		tmp = gc_malloc(((size_t)BUFFER_SIZE) + 1);
		i = read(fd, tmp, BUFFER_SIZE);
		if (i <= 0)
			return (freep(&ret, &tmp, 1, fd));
		(1) && (tmp[i] = '\0', *s = search(&tmp, s));
		if (*s)
			return (ft_strjoin(ret, tmp));
		ret = ft_strjoin(ret, tmp);
	}
	return (ret);
}

char	*get_next_line(int fd)
{
	char		*ret;
	static char	*rest[10240];
	char		*tmp;
	ssize_t		i;

	if (read(fd, NULL, 0) == -1 || BUFFER_SIZE < 0 || BUFFER_SIZE > INT_MAX)
		return (freep(&rest[fd], NULL, 0, fd));
	if (rest[fd])
		return (_rest(&rest[fd], fd));
	(1) && (ret = NULL, rest[fd] = NULL);
	while (1)
	{
		tmp = gc_malloc((size_t)BUFFER_SIZE + 1);
		i = read(fd, tmp, (size_t)BUFFER_SIZE);
		if (i <= 0)
		{
			free(tmp);
			return (ret);
		}
		(1) && (tmp[i] = '\0', rest[fd] = search(&tmp, &rest[fd]));
		if (rest[fd])
			return (ft_strjoin(ret, tmp));
		ret = ft_strjoin(ret, tmp);
	}
	return (ret);
}
