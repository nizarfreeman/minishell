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
		printf("syntax error : near unexpected token '%s'\n", head->token);
		return (0);
	}
	if (tail && (tail->type == PIPE || tail->type == AND ||
		tail->type == AND_IF || tail->type == OR_IF))
	{
		printf("syntax error : near unexpected token '%s'\n", tail->token);
		return (0);
	}
	return (1);
}

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

int	is_command_token(t_token *token)
{
	return (token->type == WORD ||
		(token->type >= BUILTIN_ECHO && token->type <= BUILTIN_EXIT) ||
		token->type == OPEN_PER);
}

int	check_cmd_sequences(t_token *head)
{
	t_token *current;
	int     expect_cmd;
	int     found_cmd;
	int     has_redirection;

	current = head;
	expect_cmd = 1;
	found_cmd = 0;
	has_redirection = 0;
	
	while (current)
	{
		if (current->type == REDIRECTION_IN || current->type == REDIRECTION_OUT ||
			current->type == APPEND || current->type == HERE_ODC)
		{
			has_redirection = 1;
			if (current->next)
				current = current->next;
			else
				break ;
		}
		else if (current->type == PIPE || current->type == AND_IF ||
			current->type == OR_IF)
		{
			if (expect_cmd && has_redirection)
				found_cmd = 1;
			expect_cmd = 1;
			found_cmd = 0;
			has_redirection = 0;
		}
		else if (expect_cmd && is_command_token(current))
		{
			expect_cmd = 0;
			found_cmd = 1;
		}
		current = current->next;
	}
	if (expect_cmd && !found_cmd && !has_redirection)
	{
		printf("syntax error: incomplete command\n");
		return (0);
	}
	return (1);
}

int syntax_check(t_token *head)
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

int main(int argc, char const *argv[])
{
	struct sigaction sa;
	t_token *head = NULL;
	t_tree *root;
	
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	
	while (1)
	{
		char *s = readline("$ ");
		if (!s)
		{
			printf("exit\n");
			break;
		}
		if (*s)
			add_history(s);
		if (strcmp(s, "") == 0)
		{
			free(s);
			continue;
		}
		if (!check_quotes(s))
		{
			printf("Error! syntax error: unclosed quote !\n");
			free(s);
			continue;
		}
		head = lexer(s);
		print_token_list(&head);
		if (head && syntax_check(head))
			root = parse_expression(head);
		else
			root = NULL;
		if (root)
		{
			printf("\n\n");
   	    	print_ast(root);
		}
		gc_free();
		free(s);
		head = NULL;
	}
	gc_free();
	rl_clear_history();
	return (0);
}