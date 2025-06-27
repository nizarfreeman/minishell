/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:16:26 by aayache           #+#    #+#             */
/*   Updated: 2025/06/27 15:21:08 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "s.h"

void	printerr2(char *cmd, int *ex)
{
	char		*s;
	struct stat	sb;

	*ex = 127;
	s = ": No such file or directory";
	if (stat(cmd, &sb) != -1 && S_ISDIR(sb.st_mode) && ft_strchr(cmd, '/'))
	{
		*ex = 126;
		s = ": Is a directory";
	}
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, s, ft_strlen(s));
	write(STDERR_FILENO, "\n", 1);
}

void	printerr(char *cmd, int *ex)
{
	char		*s;
	struct stat	sb;

	*ex = 127;
	if (ft_strrchr(cmd, '/') || (stat(cmd, &sb) != -1 && S_ISDIR(sb.st_mode)))
		printerr2(cmd, ex);
	else
	{
		s = ": command not found";
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, s, ft_strlen(s));
		write(STDERR_FILENO, "\n", 1);
	}
}
