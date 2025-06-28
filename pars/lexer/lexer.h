/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:29:40 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/28 16:57:15 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "../utilities/utilities.h"

int		find_closing_quote(char *s, int start, char quote);
int		get_quote_token_type(char quote, int preserve_quotes);
int		handle_quotes(char *s, int i, t_token **head, int preserve_quotes);
int		handle_or_pipe_operator(char *s, int i, t_token **head);
int		handle_single_pipe(char *s, int i, t_token **head);
int		handle_and_operator(char *s, int i, t_token **head);
int		handle_single_output(char *s, int i, t_token **head);
int		handle_output_redirection(char *s, int i, t_token **head);
int		handle_input_redirection(char *s, int i, t_token **head);
int		handle_parentheses(char *s, int i, t_token **head);
int		handle_close_parenthesis(char *s, int i, t_token **head);
int		handle_assignment(char *s, int i, t_token **head);
int		handle_wildcard(char *s, int i, t_token **head);
int		handle_dollar(char *s, int i, t_token **head);
int		handle_single_dollar(char *s, int i, t_token **head);
int		get_word_type(char *token);
int		handle_word(char *s, int i, t_token **head);
int		handle_operators(char *s, int i, t_token **head);
int		is_redirection(t_token *token);
int		is_command(t_token *token);
int		is_pipe_or_logical(t_token *token);
int		is_argument(t_token *token);
int		is_redirection_target(t_token *token);
int		validate_move_args_input(t_token *redirection);
int		is_joinable_type(t_tokentype type);
int		validate_revise_input(t_token **head);
int		read_heredoc_input(int fd, char *delim);
int		process_single_heredoc(t_token *token, int comm_fd);
int		revise_null(t_token *head);
int		revise_heredocs(t_token **head);
int		count(t_token *head);

t_token	*copy_token(t_token *head);
t_token	*lexer(char *s);

t_arg	*init_quote_arg(void);
t_arg	*init_operator_arg(void);

void	remove_token_from_position(t_token *token, t_token *delimiter);
void	add_to_moved_list(t_token *token, t_token **first, t_token **last);
void	collect_word_tokens(t_token *start, t_token *delimiter,
			t_token **first_moved, t_token **last_moved);
void	insert_before_redirection(t_token *first_moved,
			t_token *last_moved, t_token *redirection, t_token **head);
void	move_args(t_token *redirection, t_token **head);
void	revise_redirections(t_token **head);
void	set_arg_properties(t_arg *arg, char *s, int start, int end);
void	set_space_if_needed(t_arg *arg, char *s, int pos);
void	tokenize_input(char *s, t_token **head);
void	adjust_list(t_token **head);
void	remove_tokens_between(t_token *current, t_token *next_token);
void	update_current_token(t_token *current, char *combined,
			t_token *last_joined, t_token *next_token);
void	process_token_joining(t_token *current);
void	revise_args(t_token **head);
void	setup_child_signals(void);
void	child_process(t_token **head, char *comm_file);
void	setup_signal_handling(struct sigaction *old_sigint);
int		*get_exit_status(int *ex);
void	parse_buffer_to_tokens(char *buffer, t_token **list);
void	read_comm_file_data(char *comm_file, t_token **head);
void	set_heredoc_files_null(t_token **head);
void	handle_child_success(char *comm_file, t_token **head);
void	handle_child_failure(int status, t_token **head);
void	parent_process(pid_t pid, char *comm_file, t_token **head);

char	*build_combined_string(t_token *start, t_token **last_joined);
char	*process_quotes(char *str, char *ret, int len);
char	*unquote_string(char *str);
char	*ft_strrev(char *str);
char	*ft_itoa(long long n);
char	*ft_itoa1(int n);
#endif
