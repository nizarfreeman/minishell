/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:29:34 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/05 00:20:52 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utilities/utilities.h"

void	read_comm_file_data(char *comm_file, t_token **head)
{
	int		comm_fd;
	char	buffer[1024];
	int		bytes_read;
	t_token	*list;

	comm_fd = open(comm_file, O_RDONLY);
	if (comm_fd == -1)
		return ;
	bytes_read = read(comm_fd, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
	{
		close(comm_fd);
		return ;
	}
	buffer[bytes_read] = '\0';
	list = *head;
	parse_buffer_to_tokens(buffer, &list);
	close(comm_fd);
}

void	set_heredoc_files_null(t_token **head)
{
	t_token	*list;

	list = *head;
	while (list)
	{
		if (list->type == 9 && list->next)
			list->file = NULL;
		list = list->next;
	}
}

void	handle_child_success(char *comm_file, t_token **head)
{
	read_comm_file_data(comm_file, head);
}

void	handle_child_failure(int status, t_token **head)
{
	set_heredoc_files_null(head);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		*(get_exit_status(NULL)) = 130;
		write(2, "\n", 1);
	}
}

void	parent_process(pid_t pid, char *comm_file, t_token **head)
{
	struct sigaction	old_sigint;
	int					status;

	setup_signal_handling(&old_sigint);
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &old_sigint, NULL);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		handle_child_success(comm_file, head);
	else
		handle_child_failure(status, head);
	unlink(comm_file);
}
