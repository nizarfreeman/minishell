/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 08:52:15 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/04 10:09:33 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../utilities/utilities.h"

int	read_heredoc_input(int fd, char *delim)
{
	char	*s;

	s = readline("> ");
	while (s)
	{
		if (strcmp(s, delim) == 0)
		{
			free(s);
			return (0);
		}
		write(fd, s, strlen(s));
		write(fd, "\n", 1);
		free(s);
		s = readline("> ");
	}
	ft_putstr_fd("minishell: warning: here-document delimited ", 2);
	ft_putstr_fd("by end-of-file (wanted '", 2);
	ft_putstr_fd(delim, 2);
	ft_putstr_fd("')\n", 2);
	return (0);
}

int	process_single_heredoc(t_token *token, int comm_fd)
{
	char	*name;
	char	*delim;
	int		fd;
	int		result;

	name = ft_strjoin("/tmp/", ft_itoa(rand()));
	if (!name)
		return (-1);
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	delim = unquote_string(token->next->token);
	if (!delim)
	{
		close(fd);
		return (-1);
	}
	result = read_heredoc_input(fd, delim);
	close(fd);
	if (result == -1)
		return (-1);
	write(comm_fd, name, strlen(name));
	write(comm_fd, "\n", 1);
	return (0);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	child_process(t_token **head, char *comm_file)
{
	t_token	*list;
	int		comm_fd;

	setup_child_signals();
	comm_fd = open(comm_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (comm_fd == -1)
		exit(1);
	list = *head;
	while (list)
	{
		if (list->type == 9 && list->next)
		{
			if (process_single_heredoc(list, comm_fd) == -1)
			{
				close(comm_fd);
				exit(1);
			}
		}
		list = list->next;
	}
	close(comm_fd);
	exit(0);
}
