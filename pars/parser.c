#include "minishell.h"

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
	ret = gc_malloc(sizeof(char *) * (i + 1));
	if (!ret)
		return (NULL);
	y = 0;
	tmp = head;
	while (y < i && tmp)
	{
		ret[y] = gc_strdup(tmp->token);
		if (!ret[y])
			return (NULL);
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

	new_node = gc_malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = head->type;
	new_node->cmd = gc_strdup(head->token);
	new_node->args = get_args(head);
	new_node->file = NULL;
	if (head->file)
		new_node->file = gc_strdup(head->file);
	new_node->fd = -1;
	return (new_node);
}

int	get_root_pos(t_token *head)
{
	t_token *curr = head;
	int position = 0;
	int paren_level = 0;
	int right_to_left_pos = -1;
	int right_to_left_precedence = 100;
	int left_to_right_pos = -1;
	int left_to_right_precedence = 100;

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
			else if (curr->type == REDIRECTION_OUT || curr->type == REDIRECTION_IN ||
				curr->type == APPEND || curr->type == HERE_ODC)
				curr_precedence = 4;
			if (curr_precedence == 4)
			{
				if (left_to_right_pos == -1 || curr_precedence < left_to_right_precedence)
				{
					left_to_right_pos = position;
					left_to_right_precedence = curr_precedence;
				}
			}
			else if (curr_precedence < 100)
			{
				if (right_to_left_pos == -1 || curr_precedence <= right_to_left_precedence)
				{
					right_to_left_pos = position;
					right_to_left_precedence = curr_precedence;
				}
			}
		}
		curr = curr->next;
		position++;
	}
	if (right_to_left_pos != -1)
		return (right_to_left_pos);
	return (left_to_right_pos);
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
	new_node = gc_malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->type = head->type;
	new_node->cmd = gc_strdup(head->token);
	new_node->args = NULL;
	new_node->file = NULL;
	if (head->file)
		new_node->file = gc_strdup(head->file);
	new_node->fd = -1;
	return (new_node);
}

t_token	*deep_copy_tokens(t_token *start, t_token *end)
{
	t_token *result = NULL;
	t_token *current = start;
	t_arg *arg;

	while (current && current != end)
	{
		// Create a new t_arg for EACH token
		arg = gc_malloc(sizeof(t_arg));
		if (!arg)
			return NULL;
			
		// Copy data from the CURRENT token
		arg->token = gc_strdup(current->token);
		arg->type = current->type;
		arg->quoted = current->quoted;
		arg->space = current->space;
		arg->file = NULL;
		if (current->file)
			arg->file = gc_strdup(current->file);
		
		if (!add_token(&result, arg))
		{
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

	// Build left side - copy tokens from start to root_pos
	t_token *curr = head;
	for (int i = 0; i < root_pos && curr; i++)
	{
		t_arg *arg = gc_malloc(sizeof(t_arg));
		if (!arg)
			return NULL;
			
		arg->token = gc_strdup(curr->token);
		arg->type = curr->type;
		arg->quoted = curr->quoted;
		arg->space = curr->space;
		arg->file = NULL;
		if (curr->file)
			arg->file = gc_strdup(curr->file);
			
		if (!add_token(&left_side, arg))
		{
			return (NULL);
		}
		curr = curr->next;
	}
	
	// Skip the root token
	if (curr)
		curr = curr->next;
	
	// Build right side - copy remaining tokens
	while (curr)
	{
		t_arg *arg = gc_malloc(sizeof(t_arg));
		if (!arg)
			return NULL;
			
		arg->token = gc_strdup(curr->token);
		arg->type = curr->type;
		arg->quoted = curr->quoted;
		arg->space = curr->space;
		arg->file = NULL;
		if (curr->file)
			arg->file = gc_strdup(curr->file);
			
		if (!add_token(&right_side, arg))
		{
			return (NULL);
		}
		curr = curr->next;
	}
	
	if (left_side)
	{
		node->left = build_tree(left_side);
	}
	if (right_side)
	{
		node->right = build_tree(right_side);
	}
	
	if ((node->type == PIPE && (!node->left || !node->right)) ||
		((node->type == AND_IF || node->type == OR_IF) && !node->right))
	{
		return (NULL);
	}
	
	return (node);
}

t_tree	*parse_expression(t_token *head)
{
	if (!head)
		return NULL;
	return (build_tree(head));
}