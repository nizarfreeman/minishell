/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:34:37 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/01 08:51:51 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

void	setup_signal_handling(struct sigaction *old_sigint)
{
	struct sigaction	ignore_action;

	ignore_action.sa_handler = SIG_IGN;
	sigemptyset(&ignore_action.sa_mask);
	ignore_action.sa_flags = 0;
	sigaction(SIGINT, &ignore_action, old_sigint);
}

void	parse_buffer_to_tokens(char *buffer, t_token **list)
{
	char	*line_start;
	char	*newline_pos;

	line_start = buffer;
	while (*list)
	{
		newline_pos = strchr(line_start, '\n');
		if (!newline_pos)
			break ;
		if ((*list)->type == 9 && (*list)->next)
		{
			*newline_pos = '\0';
			(*list)->file = gc_strdup(line_start);
			line_start = newline_pos + 1;
		}
		*list = (*list)->next;
	}
}
