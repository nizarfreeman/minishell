/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 22:16:53 by aayache           #+#    #+#             */
/*   Updated: 2025/06/05 00:23:59 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pars/pars.h"
#include "exc/s.h"

static void	init_shell(t_env **env, char **envr, int *ex)
{
	get_exit_status(ex);
	*env = creat_env(envr);
	g_han = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_int);
}

static int	process_input(char *input, t_env **env, int *ex)
{
	t_token	*head;
	t_tree	*root;

	if (!*input)
	{
		free(input);
		*ex = 0;
		return (1);
	}
	add_history(input);
	if (!check_quotes(input))
	{
		write(2, "Error! syntax error: unclosed quote !\n", 39);
		free(input);
		return (1);
	}
	head = lexer(input);
	root = parse_expression(head);
	if (root)
		exec_tree(root, env, ex);
	free(input);
	gc_free();
	return (0);
}

static void	shell_loop(t_env **env, int *ex)
{
	char	*input;

	while (1)
	{
		if (!isatty(0))
			exit(1);
		g_han = 1;
		input = readline("$ ");
		g_han = 0;
		if (!input)
		{
			write(1, "exit\n", 5);
			exit(*ex);
		}
		if (process_input(input, env, ex))
			continue ;
	}
}

int	main(int argc, char const *argv[], char **envr)
{
	int		ex;
	t_env	*env;

	(void)argc;
	(void)argv;
	ex = 0;
	init_shell(&env, envr, &ex);
	shell_loop(&env, &ex);
	gc_free();
	ft_free();
	rl_clear_history();
	return (ex);
}
