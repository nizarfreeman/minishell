/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:29:07 by aayache           #+#    #+#             */
/*   Updated: 2025/05/31 14:03:31 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*tmp;

	tmp = NULL;
	while (*s)
	{
		if (*s == (char)c)
			tmp = (char *)s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (tmp);
}

int	is_all_empty(char **s)
{
	char	*tmp;
	char	c;
	int		p;

	while (*s)
	{
		tmp = *s;
		if (*tmp == '\'' || *tmp == '"')
		{
			c = *tmp;
			if (ft_strcmp(creat_word(++tmp, 1, c, &p), ""))
				return (0);
			tmp += p;
			if (*tmp)
				return (0);
		}
		else
			return (0);
		s++;
	}
	return (1);
}

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		gc_free();
		ft_free();
		exit(1);
	}
	return (pid);
}

void	ft_dup2(int a, int b)
{
	if (dup2(a, b) == -1)
	{
		perror("dup2");
		gc_free();
		ft_free();
		exit(1);
	}
}
