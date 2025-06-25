/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revise_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:26:52 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/01 08:27:50 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

int	revise_heredocs(t_token **head)
{
	char	*comm_file;
	pid_t	pid;

	comm_file = NULL;
	comm_file = ft_strjoin("/tmp/", ft_itoa((uintptr_t)comm_file));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		child_process(head, comm_file);
	else
		parent_process(pid, comm_file, head);
	if (revise_null(*head) == -1)
		return (-1);
	return (0);
}
