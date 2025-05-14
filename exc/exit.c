#include "s.h"

int	check_value_exit(char *cmd)
{
	if (!*cmd)
		return 1;
	if (*cmd == '-' || *cmd == '+')
		cmd++;
	while (*cmd && *cmd >= '0' && *cmd <= '9')
		cmd++;
	if (!*cmd)
		return 0;
	return 1;
}

void my_exit(char **cmd)
{

	if (!cmd)
	{
		// exit(last_cmd_exit);
	}
	if (!check_value_exit(*cmd) && cmd[1])
	{
		write(2, "exit: too many arguments\n", 26);
		return ;
	}	
		
	if (check_value_exit(*cmd) || ft_atoi2(*cmd))
	{
		write(2, "exit: aa: numeric argument required\n", 37);
		exit(2);		
	}
	if (!check_value_exit(*cmd) && !cmd[1])
		exit(ft_atoi(*cmd) % 256);
}
