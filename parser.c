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

/*performs an initial syntax check. looks for logical operators
in the first and last nodes, if they exist if flags a syntax error*/
int	initial_syntax_check(t_token *head)
{
	t_token *tail;
	char a[2] = "|&";
	int	i;
	int y;

	tail = head;
	while (tail->next)
		tail = tail->next;
	if (strcmp(head->token, "(") == 0) 
		head = head->next;
	if (strcmp(tail->token, "(") == 0)
		tail = tail->prev;
	i = head->type;
	y = tail->type;
	if (i == 15 || (i >= 2 && i <= 5))
		return (0);
	if (y >= 2 && y <= 5)
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
	return (new_node);
}

int get_root_pos(t_token *head)
{
	t_token *curr = head;
	int position = 0;
	int root_position = -1;
	int paren_level = 0;
	int lowest_precedence = 100;

	while (curr)
	{
		if (curr->type == OPEN_PER)
			paren_level++;
		else if (curr->type == CLOSE_PER)
			paren_level--;
		if (paren_level == 0)
		{
			int curr_precedence = 100;

			if (curr->type == OR_IF || curr->type == AND_IF)
				curr_precedence = 1;
			else if (curr->type == PIPE)
				curr_precedence = 3;
			else if (curr->type == REDIRECTION_OUT || curr->type == REDIRECTION_IN
						|| curr->type == APPEND || curr->type == HERE_ODC)
						curr_precedence = 4;
			if (curr_precedence < 100)
			{
				if (curr_precedence <= lowest_precedence)
				{
					lowest_precedence = curr_precedence;
					root_position = position;
				}
			}
		}
		curr = curr->next;
		position++;
	}
	return (root_position);
}

// /*extracts the root token*/
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

/* Helper function to free the tree */
void	free_tree(t_tree *root)
{
	if (!root)
		return;

	/* Free subtrees recursively */
	free_tree(root->left);
	free_tree(root->right);
	/* Free node contents */
	if (root->cmd)
		free(root->cmd);
	/* Free arguments array */
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
	/* Free files list if implemented */
	/* TODO: Implement file list freeing if needed */
	/* Free the node itself */
	free(root);
}

t_tree	*build_tree(t_token *head)
{
	t_tree	*node;
	int root_pos;
	t_token	*left_tokens = NULL;
	t_token	*right_tokens = NULL;
	int	i;

	/* Base case: empty token list */
	if (!head)
		return (NULL);
	/* If command is simple (no operators), create a command node */
	if (is_simple(head))
		return (insert_command(head));
	/* Find the root operator with the lowest precedence */
	root_pos = get_root_pos(head);
	/* If no operator found (should not happen if syntax check passed) */
	if (root_pos == -1)
		return (NULL);
	/* Create the root node for the current operator */
	node = root(head, root_pos);
	if (!node)
		return (NULL);
	/* Build left side tokens */
	i = 0;
	t_token *curr = head;
	while (i < root_pos)
	{
		if (!add_token(&left_tokens, curr->token, curr->type, curr->quoted, curr->space_after))
		{
			/* Free everything and return NULL on error */
			free_tree(node);
			free_token_list(&left_tokens);
			return (NULL);
		}
		curr = curr->next;
		i++;
	}
	/* Skip the root operator */
	curr = curr->next;
	/* Build right side tokens */
	while (curr)
	{
		if (!add_token(&right_tokens, curr->token, curr->type, curr->quoted, curr->space_after))
		{
			/* Free everything and return NULL on error */
			free_tree(node);
			free_token_list(&left_tokens);
			free_token_list(&right_tokens);
			return (NULL);
		}
		curr = curr->next;
	}
	/* Recursively build left subtree if there are tokens */
	if (left_tokens)
		node->left = build_tree(left_tokens);
	else
		node->left = NULL;
	/* Recursively build right subtree if there are tokens */
	if (right_tokens)
		node->right = build_tree(right_tokens);
	else
		node->right = NULL;

	/* Check if both subtrees were created successfully when needed */
	if ((left_tokens && !node->left) || (right_tokens && !node->right))
	{
		free_tree(node);
		return NULL;
	}
	return (node);
}

/* Handle parenthesized expressions */
t_tree	*build_tree_paren(t_token *head)
{
	t_token *inner_tokens = NULL;
	t_token *curr = head;
	int paren_level = 0;
	/* Skip opening parenthesis */
	if (curr && curr->type == OPEN_PER)
	{
		curr = curr->next;
		paren_level = 1;       
		/* Extract tokens inside parentheses */
		while (curr)
		{
			if (curr->type == OPEN_PER)
				paren_level++;
			else if (curr->type == CLOSE_PER)
			{
				paren_level--;
				if (paren_level == 0)
				break;  /* Found matching closing parenthesis */
			}
			/* Copy token to inner_tokens */
			if (!add_token(&inner_tokens, curr->token, curr->type, curr->quoted, curr->space_after))
			{
				free_token_list(&inner_tokens);
				return NULL;
			}
			curr = curr->next;
		}
		/* Recursively parse the contents within parentheses */
		if (inner_tokens)
			return build_tree(inner_tokens);
	}
	/* If not properly enclosed in parentheses, fall back to regular parsing */
	return build_tree(head);
}

/* Helper function to check if token list starts with parenthesis */
int	with_paren(t_token *head)
{
	if (!head)
		return 0;
	return (head->type == OPEN_PER);
}

/* Helper function to add a tree node */
t_tree	*add_tree_node(t_token *head, int root_pos)
{
	t_token *curr = head;
	int i = 0;

	/* Navigate to the root position */
	while (i < root_pos && curr)
	{
		curr = curr->next;
		i++;
	}
	/* Create and return the node */
	if (curr)
	{
		t_tree *node = malloc(sizeof(t_tree));
		if (!node)
			return NULL;

		node->left = NULL;
		node->right = NULL;
		node->type = curr->type;
		node->cmd = strdup(curr->token);
		node->args = NULL;
		node->files = NULL;
		return node;
	}
	return NULL;
}

/*main parsing functions that triggers all parsing mechanisms*/
t_tree	*parse_expression(t_token *head)
{
	t_tree *root;

	root = NULL;
	if (!initial_syntax_check(head))
	{
		printf("minishell: syntax error\n");
		return (NULL);
	}
	if (is_simple(head))
		return (insert_command(head));
	if (with_paren(head))
		root = build_tree_paren(head);
	// else
	// 	root = build_tree(head);
	root = build_tree(head);
	return (root);
	return (NULL);
}`