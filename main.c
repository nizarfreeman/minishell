/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 22:16:53 by aayache           #+#    #+#             */
/*   Updated: 2025/06/15 16:29:06 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pars/pars.h"
#include "exc/s.h"

int	handle_input(char *s)
{
	if (!*s)
	{
		free(s);
		s = NULL;
		return (1);
	}
	add_history(s);
	return (0);
}

void	execute_command(char *s, t_env **env, int *ex)
{
	t_token	*head;
	t_tree	*root;

	head = lexer(s);
	root = parse_expression(head);
	if (root)
		exec_tree(root, env, ex);
	free(s);
	gc_free();
}

char	*main2(int *ex)
{
	char	*s;

	if (!isatty(0))
		exit(1);
	g_han = 1;
	s = readline("$ ");
	g_han = 0;
	if (!s)
	{
		write(1, "exit\n", 5);
		exit(*ex);
	}
	if (handle_input(s))
		return (NULL);
	return (s);
}

int	main(int argc, char const *argv[], char **envr)
{
	int		ex;
	t_env	*env;
	char	*s;

	(void)argv;
	(void)argc;
	ex = 0;
	env = creat_env(envr);
	get_exit_status(&ex);
	g_han = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_int);
	while (1)
	{
		s = main2(&ex);
		if (s)
			execute_command(s, &env, &ex);
	}
	gc_free();
	ft_free();
	rl_clear_history();
	return (ex);
}
