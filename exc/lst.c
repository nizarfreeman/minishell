
#include "s.h"

int ft_lstsize(env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return i;
	
}  

void	ft_lstadd_back(env **lst, env *new)
{
	env	*last;

	if (lst == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

env	*ft_lstnew(env **lst, void *content, int f)
{
	env	*ret;
	env	*tmp;

	ret = malloc(sizeof(env));
	if (!ret)
		return (NULL);
	ret->value = ft_strdup(content);
	ret->f = f;
	ret->next = NULL;
	tmp = *lst;
	ft_lstadd_back(lst, ret);
	return (ret);
}

void	ft_lstclear(env **lst)
{
	env	*l1;
	env	*l2;

	if (lst == NULL)
		return ;
	l1 = *lst;
	while (l1)
	{
		l2 = l1;
		l1 = l1->next;
		free(l2);
	}
	*lst = NULL;
}