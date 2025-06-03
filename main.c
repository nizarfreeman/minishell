#include "pars/minishell.h"
#include "exc/s.h"

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

int	grab_main_level(t_token *head)
{
	t_token *tail = head;
	int count = 1;

	while (tail->next)
	{
		tail = tail->next;
		count++;
	}
	if (head->type == 13 && tail->type == 14)
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
		if (tmp == tail)
		{
			head = head->next;
			tail = tail->prev;
			count -= 2;
		}
	}
	while (count--)
	{
		if (head->type == 13 || head->type == 14)
			return 1;
		head = head->next;
	}
	return 0;
}

int main(int argc, char const *argv[], char **envr)
{
	(void)argc;
	(void)argv;
	int ex = 0;
	get_exit_status(&ex);
	t_token *head = NULL;
	t_tree *root;
	env *env = creat_env(envr);
	g_han = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_int);
	while (1)
	{
		if (!isatty(0))
			return 1;
		g_han = 1;
		char *s = readline("$ ");
		g_han = 0;
		if (!s)
		{
			write(1, "exit\n", 5);
			exit(ex);
		}
		if(!*s)
		{
			free(s);
			ex = 0;
			continue;
		}
		if (*s)
			add_history(s);
		if (!check_quotes(s))
		{
			write(2, "Error! syntax error: unclosed quote !\n", 39);
			free(s);
			continue ;
		}
		head = lexer(s);
		root = parse_expression(head);
		//print_token_list(&head);
		if (root)
		{
			// printf("\n\n");
			// print_tree(root);
    		// print_ast(root);
			exec_tree(root, &env, &ex);
			// free_token_list(&head);
		}
		free(s);
		gc_free();
	}
	gc_free();
	ft_free();
	rl_clear_history();

	return (ex);
}

//echo '(('"()(09009-0-"""''"((((''))))"""
//valgrind --gen-suppressions=all --show-leak-kinds=all --leak-check=full --track-fds=yes   ./minishell