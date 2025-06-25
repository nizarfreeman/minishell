/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:00:23 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/05 00:24:39 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_CHECK_H
# define SYNTAX_CHECK_H
# include "../utilities/utilities.h"

int		check_start_end(t_token *head);
int		is_invalid_after_open_paren(int type);
int		is_invalid_before_close_paren(int type);
int		is_valid_after_close_paren(int type);
int		handle_open_paren(t_token *current, int *count);
int		handle_close_paren(t_token *current, int *count);
int		check_parentheses(t_token *head);
int		check_redirection_target(t_token *redir);
int		check_redirections(t_token *head);
int		check_operator_sequence(t_token *current);
int		check_operator_before_close(t_token *current);
int		check_operators(t_token *head);
int		check_heredoc(t_token *head);
int		is_valid_heredoc_delimiter(int type);
int		syntax_check(t_token *head);
int		is_invalid_after_redir(int type);
int		is_redirection_(int type);
int		is_operator_end(int type);
int		is_operator_start(int type);
int		*get_exit_status(int *ex);
int		check_quotes(char *s);

void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	line_too_long(void);

#endif
