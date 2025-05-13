#include "s.h"
char *ft_strchr(char *s, char c)
{
	if (!s)
		return NULL;
	while (*s && *s != c)
		s++;
	if (*s != c)
		return NULL;
	return s;
	
}
int check_export_arg(char *arg)
{
	if (!(*arg >= 'a' && *arg <= 'z') && !(*arg >= 'A' && *arg <= 'Z') && *arg != '_')
		return (1);
	arg++;
	while (*arg && (*arg >= 'a' && *arg <= 'z') || (*arg >= 'A' && *arg <= 'Z') || *arg == '_' 
		|| (*arg >= '0' && *arg <= '9'))
		arg++;
	if (!*arg)
		return 0;
	if (*arg == '+' && *(arg + 1) == '=' && !*(arg + 2))
	{
		*arg = '\0';
		return 2;
	}
	if (*arg == '=')
	{
		*arg = '\0';
		return 3;
	}
	return 1;	
}
char **export_split(char *str)
{
	// printf("here\n");
	char **ret;
	char *s;
	char *e;
	ret = malloc(sizeof(char *) * 3);
	s = str;
	while (*str && *str != '=')
		str++;
	if (*str == '=')
		str++;
	ret[0] = word(s, str);
	if (!*str)
		ret[1] = NULL;
	else
		ret[1] = ft_strdup(str);
		ret[2] = NULL;
	// printf("|%s      | %s|\n", ret[0], ret[1]);
	// exit(1);
	//free2d(split)
	return ret;
}

int export2(char *arg ,env **envr)
{
	char **spl;
	int flag;
	char *tmp;
	char *tmp1;
	// printf("here\n");
	tmp1 = NULL;
	spl = export_split(arg);
	flag = check_export_arg(spl[0]);

	// printf("%d\n", flag);
	if(flag == 1)
	{
		printf("export: '%s': not a valid identifier\n", spl[0]);
		// perror("export");
		//free2d(spl)
		return 1;
	}
	if (flag == 2)
	{
		tmp = get_value(*envr, spl[0]);
		// printf("|{%s\n}|", spl[0]);
		if (tmp == NULL)
		{
			tmp = ft_strjoin(ft_strjoin(spl[0], ft_strdup("=")), spl[1]);
			ft_lstnew(envr, tmp, 1);
			free(tmp);
			free(spl);
			return 0;
		}
		// printf("%s\n", tmp);
		if (*tmp == '=')
		{
			tmp1 = tmp;
			tmp = ft_strjoin(ft_strdup(tmp + 1), spl[1]);
		}
		else
			tmp = ft_strjoin(tmp, spl[1]);
		search_replace(*envr, spl[0], tmp);
		free(spl[0]);
		free(tmp1);
		free(spl);
		return 0;
	}
	if (flag == 3)
	{
		tmp = get_value(*envr, spl[0]);
		if (tmp == NULL)
		{
			tmp = ft_strjoin(ft_strjoin(spl[0], ft_strdup("=")), spl[1]);
			ft_lstnew(envr, tmp, 1);
			free(tmp);
			free(spl);
			return 0;
		}
		search_replace(*envr, spl[0], spl[1]);
		// free2d(spl);
		return 0;
	}
	if (flag == 0)
	{
		tmp = get_value(*envr, spl[0]);
		if (tmp == NULL)
		{
			printf("here\n");
			// if (!ft_strcmp(spl[0], "PWD"))
			// {
			// 	tmp = ft_strjoin(ft_strdup(spl[0]), ft_strjoin(ft_strdup("="), getcwd(NULL, 0)));
			// 	ft_lstnew(envr, tmp, 1);
			// }
			// else
			ft_lstnew(envr, spl[0], 0);
			free(tmp);
			// free2d(spl);
			return 0;
		}
	}
	return 0;
		
}
void swap_strings(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

char **sort_arr(char **arr)
{
	char **ret = arr;
    int size = 0;
    while (arr[size] != NULL)
        size++;
    
    int i = 0;
    while (i < size - 1)
	{
        int j = 0;
        while (j < size - i - 1)
		{
            if (ft_strcmp(arr[j], arr[j + 1]) > 0) {
                swap_strings(&arr[j], &arr[j + 1]);
            }
            j++;
        }
        i++;
    }
	return ret;
}

void print_export(char **arr)
{
	while (*arr)
	{
		printf("declare -x %s\n", *arr);
		// write(1,"declare -x ", 11);
		// write(1, *arr, ft_strlen(*arr));
		// write(1,"\n", 1);
		arr++;
	}
}

int export(env **envr, char **args)
{
	int flag;
	flag = 0;
	if (args && !*args)
	{
		// printf("here\n");
		print_export(sort_arr(lst_to_arr2(*envr)));
		return 0;
	}
	while (*args)
	{
		if(export2(*args, envr))
			flag = 1;
		args++;
	}
	return flag;
}
