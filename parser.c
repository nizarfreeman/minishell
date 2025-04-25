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

/*splits the command using a binary tree*/
t_tree	*build_tree(t_token *head)
{
	
}

/*splits the command containing parenthesis using a binary tree, meaning command with 
multiple execution levels*/
t_tree	*build_tree_paren(t_token *head)
{
	
}

int	 with_paren(t_token *head)
{
	t_token *tail = head;
	int count = 1;

	/*Find tail and count nodes*/
	while (tail->next)
	{
		tail = tail->next;
		count++;
	}
	/*Try skipping one outer pair of parentheses*/
	if (head->type == 13 && tail->type == 14) // 13 = '(', 14 = ')'
	{
		t_token *tmp = head->next;
		int paren = 1;

		while (tmp && tmp != tail)
		{
			if (tmp->type == 13)
				paren++;
			else if (tmp->type == 14)
				paren--;
			if (paren == 0)
				break;
			tmp = tmp->next;
		}
		/*If the closing parenthesis is tail itself, they enclose the whole expression*/
		if (tmp == tail)
		{
			head = head->next;
			tail = tail->prev;
			count -= 2;
		}
	}
	/*Scan again: return 1 if any remaining parens*/
	while (count--)
	{
		if (head->type == 13 || head->type == 14)
			return 1;
		head = head->next;
	}
	return 0;
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
	else
		root = build_tree(head);
	return (NULL);
}