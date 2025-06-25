/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 09:17:30 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/23 14:38:22 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "../utilities/utilities.h"

typedef struct s_operator_data
{
	int	rtl_pos;
	int	rtl_prec;
	int	ltr_pos;
	int	ltr_prec;
}	t_operator_data;

int		is_simple(t_token *head);
int		get_operator_precedence(int token_type);
int		get_root_pos(t_token *head);
int		check_paren_balance(t_token *head, t_token *last);
int		is_enclosed_in_parentheses(t_token *head);
int		validate_tree_node(t_tree *node);

void	update_left_to_right(t_operator_data *data,
			int current_pos, int current_precedence);
void	update_right_to_left(t_operator_data *data,
			int current_pos, int current_precedence);
void	process_operator(t_token *curr, int position, t_operator_data *data);
void	find_operators(t_token *head, t_operator_data *data);

char	**get_args(t_token *head);

t_arg	*create_token_arg(t_token *curr);

t_token	*deep_copy_tokens(t_token *start, t_token *end);
t_token	*extract_paren_content(t_token *head);
t_token	*build_left_tokens(t_token *head, int root_pos);
t_token	*build_right_tokens(t_token *head, int root_pos);

t_tree	*build_tree(t_token *head);
t_tree	*handle_parentheses_(t_token *head);
t_tree	*build_complex_tree(t_token *head, int root_pos);
t_tree	*root(t_token *head, int root_pos);
t_tree	*insert_command(t_token *head);
t_tree	*parse_expression(t_token *head);

#endif
