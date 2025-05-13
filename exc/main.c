

// int main(int ac, char **av, char **envr)
// {
// 	env *env = creat_env(envr);
// 	sig_han = 0;
// 	char *line;
// 	char **cmd;
// 	signal(SIGQUIT, SIG_IGN);
// 	signal(SIGINT, handle_int);
// 	while (1)
// 	{
// 		sig_han = 1;
// 		line = readline("$ ");
// 		sig_han = 0;
// 		if (!line)
// 		{
// 			write(1, "exit\n", 5);
// 			exit(1);
// 		}
// 		if (line)
// 			add_history(line);
// 		if (*line)
// 			excute(ft_split(line, ' '), &env);
// 	}
// }
 

//  //echo a a a a