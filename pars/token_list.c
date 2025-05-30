#include "minishell.h"

void	free_token_list(t_token **head)
{
	t_token *tmp;

	while (*head)
	{
		tmp = (*head)->next;
		// free((*head)->token);
		// free((*head)->file);
		// free(*head);
		*head = tmp;
	}
}

void print_token_list(t_token **head)
{
    t_token *current;
    
    if (!head || !*head)
        return;
        
    current = *head;
    while (current)
    {
        printf("%s %d %d %d\n", current->token, current->type, current->space, current->quoted);
        current = current->next;
    }
}

#include <string.h>

int add_token(t_token **head, t_arg *arg)
{
    t_token *new_node;  
    t_token *tmp;

    new_node = (t_token *)gc_malloc(sizeof(t_token));
    if (!new_node)
    {
        gc_free();
        exit(1);
    }
    memset(new_node, 0, sizeof(t_token));
    new_node->file = arg->file;
    new_node->token = arg->token;
    new_node->type = arg->type;
    new_node->space = arg->space;
    if (arg->quoted)
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