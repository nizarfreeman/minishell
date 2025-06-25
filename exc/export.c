/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:06:25 by aayache           #+#    #+#             */
/*   Updated: 2025/06/21 14:32:00 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

int	check_export_arg(char *arg)
{
	if (!(*arg >= 'a' && *arg <= 'z') && !(*arg >= 'A' && *arg <= 'Z')
		&& *arg != '_')
		return (1);
	arg++;
	while (*arg && ((*arg >= 'a' && *arg <= 'z') || (*arg >= 'A' && *arg <= 'Z')
			|| *arg == '_' || (*arg >= '0' && *arg <= '9')))
		arg++;
	if (!*arg)
		return (0);
	if (*arg == '+' && *(arg + 1) == '=' && !*(arg + 2))
	{
		*arg = '\0';
		return (2);
	}
	if (*arg == '=')
	{
		*arg = '\0';
		return (3);
	}
	return (1);
}

char	**export_split(char *str)
{
	char	**ret;
	char	*s;

	ret = gc_malloc(sizeof(char *) * 3);
	s = str;
	while (*str && *str != '=')
		str++;
	if (*str == '=')
		str++;
	ret[0] = word(s, str);
	if (!*str)
		ret[1] = ft_strdup("");
	else
		ret[1] = ft_strdup(str);
	ret[2] = NULL;
	return (ret);
}

int	export3(t_env **t_envr, char **spl, int flag)
{
	char	*tmp;

	if (flag == 3)
	{
		tmp = get_value(*t_envr, spl[0], 1);
		if (!tmp)
		{
			tmp = ft_strjoin(ft_strjoin(spl[0], ft_strdup("=")), spl[1]);
			ft_lstnew1(t_envr, tmp, 1);
			return (0);
		}
		search_replace1(*t_envr, spl[0], spl[1]);
		return (0);
	}
	if (flag == 0)
	{
		tmp = get_value(*t_envr, spl[0], 1);
		if (tmp == NULL)
		{
			ft_lstnew1(t_envr, spl[0], 0);
			return (0);
		}
		search_replace1(*t_envr, spl[0], NULL);
	}
	return (0);
}

int	export4(t_env **t_envr, char **spl)
{
	char	*tmp;

	tmp = get_value(*t_envr, spl[0], 1);
	if (!tmp)
	{
		tmp = ft_strjoin1(ft_strjoin(spl[0], ft_strdup("=")), spl[1]);
		ft_lstnew1(t_envr, tmp, 1);
		return (0);
	}
	if (*tmp == '=')
		tmp = ft_strjoin(ft_strdup(tmp + 1), spl[1]);
	else
		tmp = ft_strjoin(tmp, spl[1]);
	search_replace1(*t_envr, spl[0], tmp);
	return (0);
}

int	export2(char *arg, t_env **t_envr)
{
	char	**spl;
	int		flag;

	spl = export_split(arg);
	flag = check_export_arg(spl[0]);
	if (flag == 1)
	{
		write(2, "export: '", 10);
		write(2, spl[0], ft_strlen(spl[0]));
		write(2, "': not a valid identifier\n", 27);
		return (1);
	}
	if (flag == 2)
		return (export4(t_envr, spl));
	else
		return (export3(t_envr, spl, flag));
	return (0);
}
