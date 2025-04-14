#include "minishell.h"

void	signal_handler(int signal, siginfo_t *info, void *oldact)
{
	printf("\n");
	rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int chck_partner(char *s, char c, int i)
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
			partner = chck_partner(s, s[i], i);
			if (partner == -1)
				return (0);
			i = partner + 1;
		}
		else
		i++;
	}
	return (1);
}

int main(int argc, char const *argv[])
{
	struct sigaction sa;

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	t_token *head = NULL;
	while (1)
	{
		char *s = readline("$ ");
		if (!s)
			break ;
		if (*s)
			add_history(s);
		if (!check_quotes(s))
		{
			printf("Error ! unclosed quote !\n");
			free(s);
			continue ;
		}
		lexer(s);
		free(s);
	}

	rl_clear_history();
	return (0);
}