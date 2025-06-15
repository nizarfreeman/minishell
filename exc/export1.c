/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:22:41 by aayache           #+#    #+#             */
/*   Updated: 2025/06/04 23:19:49 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

char	**sort_arr(char **arr)
{
	char	**ret;
	int		size;
	int		i;
	int		j;

	ret = arr;
	size = 0;
	while (arr[size] != NULL)
		size++;
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				swap_strings(&arr[j], &arr[j + 1]);
			}
			j++;
		}
		i++;
	}
	return (ret);
}

void	print_export(char **arr)
{
	char	**s;

	while (*arr)
	{
		s = export_split(*arr);
		write(1, "declare -x ", 11);
		write(1, *s, ft_strlen(*s));
		if (ft_strchr(*arr, '='))
		{
			write(1, "\"", 1);
			write(1, *(s + 1), ft_strlen(*(s + 1)));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		arr++;
	}
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
			while (*tmp1)
			{
				if (**tmp1)
					ft_lstnew(&ret, *tmp1, 0);
				tmp1++;
			}
		}
		else
			ft_lstnew(&ret, expand2(unquote_string(tmp), t_envr, ex), 0);
		s++;
	}
	return (lst_to_arr2(ret));
}

int	export(t_env **t_envr, char **args, int *ex)
{
	int	flag;

	flag = 0;
	args = export_expand(args, *t_envr, ex);
	if ((args && !*args) || (args && args[1] && !*args[1]))
	{
		print_export(sort_arr(lst_to_arr2(*t_envr)));
		*ex = 0;
		return (0);
	}
	while (*args)
	{
		if (export2(*args, t_envr))
			flag = 1;
		args++;
	}
	*ex = flag;
	return (flag);
}
