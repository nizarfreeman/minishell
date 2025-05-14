#include "minishell.h"

void	free_token_list(t_token **head)
{
	t_token *tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free((*head)->token);
		free(*head);
		*head = tmp;
	}
}

void print_token_list(t_token **head)
{
    t_token *current;
    
    if (!head || !*head)
        return;
        
    current = *head;  // Use a temporary pointer
    while (current)
    {
        printf("%s %d %d %d\n", current->token, current->type, current->space_after, current->quoted);
        current = current->next;
    }
}

#include <string.h>

int	add_token(t_token **head, char *token, int type, int quoted, int space)
{
	t_token	*new_node;	
	t_token	*tmp;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (0);
	memset(new_node, 0, sizeof(t_token));
	new_node->token = token;
	new_node->type = type;
	new_node->space_after = space;
	if (quoted)
		new_node->quoted = 1;
	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while ((*head)->next)
			*head = (*head)->next;
		(*head)->next = new_node;
		new_node->prev = *head;
		*head = tmp;
	}
	return (1);
}