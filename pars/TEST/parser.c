/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:53:36 by nfaska            #+#    #+#             */
/*   Updated: 2025/04/19 10:54:04 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	check_start_end(t_token *head)
{
	t_token *tail;

	if (!head)
		return (1);
	tail = head;
	while (tail && tail->next)
		tail = tail->next;
	if (head->type == PIPE || head->type == AND ||
		head->type == AND_IF || head->type == OR_IF)
	{
		printf("syntax error near unexpected token `%s'\n", head->token);
		return (0);
	}
	if (tail && (tail->type == PIPE || tail->type == AND ||
		tail->type == AND_IF || tail->type == OR_IF))
	{
		printf("syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}

/*
** check_parentheses:
** Validates balanced parentheses and subshell syntax
*/
int	check_parentheses(t_token *head)
{
	t_token *current;
	int     count;

	count = 0;
	current = head;
	while (current)
	{
		if (current->type == OPEN_PER)
			count++;
		else if (current->type == CLOSE_PER)
		{
			count--;
			if (count < 0)
			{
				printf("syntax error: unexpected closing parenthesis ')'\n");
				return (0);
			}
		}
		if (current->type == OPEN_PER && current->next &&
			current->next->type == CLOSE_PER)
		{
			printf("syntax error: empty subshell ()\n");
			return (0);
		}
		current = current->next;
	}
	if (count > 0)
	{
		printf("syntax error: unexpected EOF while looking for matching `)'\n");
		return (0);
	}
	return (1);
}

/*
** check_redirection_target:
** Checks if a redirection token has a valid target
*/
int	check_redirection_target(t_token *redir)
{
	t_token *next;

	if (!redir->next)
	{
		printf("syntax error near unexpected token `newline'\n");
		return (0);
	}
	next = redir->next;
	if (next->type == PIPE || next->type == AND || next->type == AND_IF ||
		next->type == OR_IF || next->type == REDIRECTION_IN ||
		next->type == REDIRECTION_OUT || next->type == APPEND ||
		next->type == HERE_ODC || next->type == CLOSE_PER)
	{
		printf("syntax error near unexpected token `%s'\n", next->token);
		return (0);
	}
	return (1);
}

/*
** check_redirections:
** Checks for ambiguous and invalid redirections
*/
int	check_redirections(t_token *head)
{
	t_token *current;

	current = head;
	while (current)
	{
		if (current->type == REDIRECTION_IN || current->type == REDIRECTION_OUT ||
			current->type == APPEND || current->type == HERE_ODC)
		{
			if (!check_redirection_target(current))
				return (0);
			if ((current->type == REDIRECTION_IN ||
				current->type == REDIRECTION_OUT) && current->next &&
				(current->next->type == REDIRECTION_IN ||
				current->next->type == REDIRECTION_OUT))
			{
				printf("syntax error: ambiguous redirection\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** check_operators:
** Validates pipe and logical operators
*/
int	check_operators(t_token *head)
{
	t_token *current;

	current = head;
	while (current)
	{
		if (current->type == PIPE || current->type == AND ||
			current->type == AND_IF || current->type == OR_IF)
		{
			if (current->next && (current->next->type == PIPE ||
				current->next->type == AND || current->next->type == AND_IF ||
				current->next->type == OR_IF))
			{
				printf("syntax error near unexpected token `%s'\n",
					current->next->token);
				return (0);
			}
			if (current->next && current->next->type == CLOSE_PER)
			{
				printf("syntax error near unexpected token `)'\n");
				return (0);
			}
			if (!current->next)
			{
				printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** check_assignments:
** Validates assignment syntax
*/
int	check_assignments(t_token *head)
{
	t_token *current;

	current = head;
	while (current)
	{
		if (current->type == ASSIGN)
		{
			if (current == head && (current->prev == NULL ||
				(current->prev->type != WORD && !current->prev->quoted)))
			{
				printf("syntax error near unexpected token `='\n");
				return (0);
			}
			if (!current->next)
			{
				printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** check_heredoc:
** Validates here-document syntax
*/
int	check_heredoc(t_token *head)
{
	t_token *current;

	current = head;
	while (current)
	{
		if (current->type == HERE_ODC)
		{
			if (!current->next)
			{
				printf("syntax error: missing delimiter for here-document\n");
				return (0);
			}
			if (current->next->type != WORD && current->next->type != S_QUOTE &&
				current->next->type != D_QUOTE)
			{
				printf("syntax error near unexpected token `%s'\n",
					current->next->token);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** is_command_token:
** Helper function to check if token can start a command
*/
int	is_command_token(t_token *token)
{
	return (token->type == WORD ||
		(token->type >= BUILTIN_ECHO && token->type <= BUILTIN_EXIT) ||
		token->type == OPEN_PER);
}

/*
** check_cmd_sequences:
** Validates command sequences
*/
int	check_cmd_sequences(t_token *head)
{
	t_token *current;
	int     expect_cmd;
	int     found_cmd;

	current = head;
	expect_cmd = 1;
	found_cmd = 0;
	while (current)
	{
		if (current->type == PIPE || current->type == AND_IF ||
			current->type == OR_IF)
		{
			expect_cmd = 1;
			found_cmd = 0;
		}
		if (expect_cmd && is_command_token(current))
		{
			expect_cmd = 0;
			found_cmd = 1;
		}
		if (current->type == REDIRECTION_IN || current->type == REDIRECTION_OUT ||
			current->type == APPEND || current->type == HERE_ODC)
		{
			if (current->next)
				current = current->next;
		}
		current = current->next;
	}
	if (expect_cmd && !found_cmd)
	{
		printf("syntax error: incomplete command\n");
		return (0);
	}
	return (1);
}

/*
** initial_syntax_check:
** Main function for syntax validation
*/
int initial_syntax_check(t_token *head)
{
	if (!check_start_end(head))
		return (0);
	if (!check_parentheses(head))
		return (0);
	if (!check_redirections(head))
		return (0);
	if (!check_operators(head))
		return (0);
	if (!check_assignments(head))
		return (0);
	if (!check_heredoc(head))
		return (0);
	if (!check_cmd_sequences(head))
		return (0);
	return (1);
}

/*checks if the command is simple, meaning it contains no logical operators
or redirections and generally follow the syntax : command + options + args*/
int	is_simple(t_token *head)
{
	int	i;

	while (head->next)
	{
		i = head->type;
		if (i >= 2 && i <= 9)
			return (0);
		head = head->next;
	}
	return (1);
}

/*a function to copy cmd + args to a double array*/
char	**get_args(t_token *head)
{
	t_token *tmp;
	int i;
	char **ret;
	int y;

	i = 0;
	tmp = head;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	ret = malloc(sizeof(char *) * (i + 1));
	if (!ret)
		return (NULL);
	y = 0;
	tmp = head;
	while (y < i && tmp)
	{
		ret[y] = strdup(tmp->token);
		if (!ret[y])
		{
			while (--y >= 0)
				free(ret[y]);
			free(ret);
			return (NULL);
		}
		tmp = tmp->next;
		y++;
	}
	ret[y] = NULL;
	return (ret);
}

/*a function to insert commands in nodes*/
t_tree	*insert_command(t_token *head)
{
	t_tree *new_node;

	new_node = malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = head->type;
	new_node->cmd = strdup(head->token);
	new_node->args = get_args(head);
	new_node->files = NULL;
	new_node->fd = -1;
	return (new_node);
}

int	get_root_pos(t_token *head)
{
	t_token *curr = head;
	int position = 0;
	int root_position = -1;
	int paren_level = 0;
	int lowest_precedence = 100;
	int is_redirection = 0;

	while (curr)
	{
		if (curr->type == OPEN_PER)
			paren_level++;
		else if (curr->type == CLOSE_PER)
			paren_level--;
		if (paren_level == 0)
		{
			int curr_precedence = 100;
			int curr_is_redirection = 0;
			if (curr->type == OR_IF || curr->type == AND_IF)
				curr_precedence = 1;
			// else if (curr->type == AND_IF)
			// 	curr_precedence = 2;
			else if (curr->type == PIPE)
				curr_precedence = 3;
			else if (curr->type == REDIRECTION_OUT || curr->type == REDIRECTION_IN ||
				curr->type == APPEND || curr->type == HERE_ODC)
			{
				curr_precedence = 4;
				curr_is_redirection = 1;
			}
			if (curr_precedence < 100)
			{
				if (curr_precedence < lowest_precedence ||
					(curr_precedence == lowest_precedence &&
					((curr_precedence <= 3) ||
					(curr_is_redirection && curr_is_redirection == is_redirection))))
				{
					lowest_precedence = curr_precedence;
					root_position = position;
					is_redirection = curr_is_redirection;
				}
			}
		}
		curr = curr->next;
		position++;
	}
	return root_position;
}

int	is_enclosed_in_parentheses(t_token *head)
{
	if (!head || !head->next)
		return 0;

	t_token *first = head;
	t_token *last = head;
	while (last->next)
		last = last->next;
	if (first->type != OPEN_PER || last->type != CLOSE_PER)
		return 0;
	int level = 1;
	t_token *curr = first->next;
	while (curr != last)
	{
		if (curr->type == OPEN_PER)
			level++;
		else if (curr->type == CLOSE_PER)
		{
			level--;
			if (level == 0 && curr != last)
				return 0;
		}
		curr = curr->next;
	}
	return (level == 1);
}

/*extracts the root token*/
t_tree *root(t_token *head, int root_pos)
{
	t_tree *new_node;
	int i;

	i = 0;
	while (i != root_pos)
	{
		head = head->next;
		i++;
	}
	new_node = malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = head->type;
	new_node->cmd = strdup(head->token);
	new_node->args = NULL;
	new_node->files = NULL;
	return (new_node);
}

/* function to free the tree */
void	free_tree(t_tree *root)
{
	if (!root)
		return;

	free_tree(root->left);
	free_tree(root->right);
	if (root->cmd)
		free(root->cmd);
	if (root->args)
	{
		int i = 0;
		while (root->args[i])
		{
			free(root->args[i]);
			i++;
		}
		free(root->args);
	}
	free(root);
}

t_token	*deep_copy_tokens(t_token *start, t_token *end)
{
	t_token *result = NULL;
	t_token *current = start;

	while (current && current != end)
	{
		if (!add_token(&result, strdup(current->token), current->type, 
			current->quoted, current->space_after))
		{
			free_token_list(&result);
			return NULL;
		}
		current = current->next;
	}
	return result;
}

// Improved parenthesis content extraction
t_token	*extract_paren_content(t_token *head)
{
	if (!head || head->type != OPEN_PER)
		return (NULL);

	t_token *start = head->next;
	t_token *end = head;
	int paren_level = 1;

	while (end->next)
	{
		end = end->next;
		if (end->type == OPEN_PER)
			paren_level++;
		else if (end->type == CLOSE_PER)
		{
			paren_level--;
			if (paren_level == 0)
				break;
		}
	}
	if (paren_level != 0)
		return NULL;
	return (deep_copy_tokens(start, end));
}

t_tree	*build_tree(t_token *head)
{
	t_tree *node = NULL;
	int root_pos;

	if (!head)
		return (NULL);
	if (is_enclosed_in_parentheses(head))
	{
		t_token *inner_tokens = extract_paren_content(head);
		if (inner_tokens)
		{
			node = build_tree(inner_tokens);
			free_token_list(&inner_tokens);
			return (node);
		}
	}
	if (is_simple(head))
		return (insert_command(head));
	root_pos = get_root_pos(head);
	if (root_pos == -1)
		return (NULL);
	node = root(head, root_pos);
	if (!node)
		return (NULL);
	t_token *left_side = NULL;
	t_token *right_side = NULL;

	t_token *curr = head;
	for (int i = 0; i < root_pos && curr; i++)
	{
		if (!add_token(&left_side, strdup(curr->token), curr->type,
			curr->quoted, curr->space_after))
		{
			free_tree(node);
			free_token_list(&left_side);
			return (NULL);
		}
		curr = curr->next;
	}
	curr = curr->next;
	while (curr)
	{
		if (!add_token(&right_side, strdup(curr->token), curr->type,
			curr->quoted, curr->space_after))
		{
			free_tree(node);
			free_token_list(&left_side);
			free_token_list(&right_side);
			return (NULL);
		}
		curr = curr->next;
	}
	if (left_side)
	{
		node->left = build_tree(left_side);
		free_token_list(&left_side);
	}
	if (right_side)
	{
		node->right = build_tree(right_side);
		free_token_list(&right_side);
	}
	if ((node->type == PIPE && (!node->left || !node->right)) ||
		((node->type == AND_IF || node->type == OR_IF) && !node->right))
	{
		free_tree(node);
		return (NULL);
	}
	return (node);
}

/*main parsing functions that triggers all parsing mechanisms*/
t_tree	*parse_expression(t_token *head)
{
	if (!initial_syntax_check(head))
		return (NULL);
	return (build_tree(head));
}