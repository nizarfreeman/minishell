#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "wait.h"
#include <limits.h>
#include <signal.h>
int expand_valid(char c)
{
	return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && c != '_');
}

int main()
{
	
	printf("%d\n", chdir("ss"));
}