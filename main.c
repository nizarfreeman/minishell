#include "minishell.h"

void	signal_handler(int signal, siginfo_t *info, void *oldact)
{
	printf("\n");
	rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int check_partner(char *s, char c, int i)
{
	i++;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

/*this function checks if quotes are matching meanign every quote have
a closing quote*/
int check_quotes(char *s)
{
	int i = 0;
	int partner;

	if (!s)
		return (1);
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			partner = check_partner(s, s[i], i);
			if (partner == -1)
				return (0);
			i = partner + 1;
		}
		else
		i++;
	}
	return (1);
}

/*this functions checks for parenthesis matching meaning every opening parenthesis should
have a closing parenthesis*/
int	check_parentheses(const char *text)
{
	int		stack_size;
	char	*stack;
	int		top;
	int		i;

	stack_size = strlen(text);
	stack = (char *)malloc(stack_size * sizeof(char));
	if (!stack)
		return (0);
	top = -1;
	i = 0;
	while (text[i] != '\0')
	{
		if (text[i] == '(')
			stack[++top] = text[i];
		else if (text[i] == ')')
		{
			if (top < 0)
			{
				free(stack);
				return (0);
			}
			top--;
		}
		i++;
	}
	free(stack);
	return (top == -1);
}

int	grab_main_level(t_token *head)
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

// int get_root(t_token *head)
// {
// 	t_token *curr = head;
// 	int position = 0;
// 	int root_position = -1;
// 	int paren_level = 0;
// 	int lowest_precedence = 100;

// 	while (curr)
// 	{
// 		if (curr->type == OPEN_PER)
// 			paren_level++;
// 		else if (curr->type == CLOSE_PER)
// 			paren_level--;
// 		if (paren_level == 0)
// 		{
// 			int curr_precedence = 100;

// 			if (curr->type == OR_IF)
// 				curr_precedence = 1;
// 			else if (curr->type == AND_IF)
// 				curr_precedence = 2;
// 			else if (curr->type == PIPE)
// 				curr_precedence = 3;
// 			else if (curr->type == REDIRECTION_OUT || curr->type == REDIRECTION_IN
// 						|| curr->type == APPEND || curr->type == HERE_ODC)
// 						curr_precedence = 4;
			
// 			if (curr_precedence < 100)
// 			{
// 				if (curr_precedence <= lowest_precedence)
// 				{
// 					lowest_precedence = curr_precedence;
// 					root_position = position;
// 				}
// 			}
// 		}
// 		curr = curr->next;
// 		position++;
// 	}
// 	return (root_position);
// }

int main(int argc, char const *argv[])
{
	struct sigaction sa;
	t_token *head = NULL;
	t_tree *root;

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		char *s = readline("$ ");
		if (!s)
			break ;
		if (*s)
			add_history(s);
		if (!check_quotes(s))
		{
			printf("Error!syntax error: unclosed quote !\n");
			free(s);
			continue ;
		}
		if (!check_parentheses(s))
		{
			printf("Error! syntax error: unclosed prentheses!\n");
			free(s);
			continue ;
		}
		head = lexer(s);
		root = parse_expression(head);
		//print_token_list(&head);
		if (root)
		{
			printf("\n\n");
			//print_tree(root);
    		print_ast(root);
			free_token_list(&head);
		}
		free(s);
	}

	rl_clear_history();
	return (0);
}