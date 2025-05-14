#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Basic character checks */
int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int ft_isalpha(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}

int ft_isspace(int c)
{
    return (c == ' ' || c == '\t');
}

/* Check if character is a special operator */
int ft_is_operator_char(int c)
{
    return (c == '&' || c == '|' || c == '<' || c == '>' || 
            c == '(' || c == ')' || c == '=' || c == '*');
}

int ft_is_quote(int c)
{
    return (c == '\'' || c == '\"');
}

size_t ft_strlen(char *s)
{
    size_t len = 0;
    while (s[len])
        len++;
    return len;
}

/* Joins two strings into a new one, freeing the first string */
char *ft_strjoin(char *s1, char *s2)
{
    size_t len1 = (s1) ? ft_strlen(s1) : 0;
    size_t len2 = (s2) ? ft_strlen(s2) : 0;
    char *res = malloc(len1 + len2 + 1);
    size_t i = 0;

    if (!res)
        return NULL;
    if (s1)
    {
        for (size_t j = 0; j < len1; j++)
            res[i++] = s1[j];
    }
    if (s2)
    {
        for (size_t j = 0; j < len2; j++)
            res[i++] = s2[j];
    }
    res[i] = '\0';
    return res;
}

/* A custom implementation of strndup */
char *ft_strndup(const char *s, size_t n)
{
    char *result = malloc(n + 1);
    
    if (!result)
        return NULL;
    strncpy(result, s, n);
    result[n] = '\0';
    return result;
}

/* Checks if a given character is present in the string */
int is_there_char(char *s, char c)
{
    while (*s)
    {
        if (*s == c)
            return 1;
        s++;
    }
    return 0;
}

/* Handle quoted string tokenization */
int handle_quotes(char *s, int i, t_token **head)
{
    char quote = s[i++];
    int start = i;
    int space = 0;
    
    while (s[i] && s[i] != quote)
        i++;
    if (ft_isspace(s[i + 1]))
        space = 1;
    char *token = ft_strndup(&s[start], i - start);
    int type;
    if (quote == '\'')
        type = S_QUOTE;
    else
        type = D_QUOTE;
    add_token(head, token, type, 1, space);
    if (s[i] == quote)
        i++;
    return i;
}

/* Handle AND operator tokenization */
int handle_and_operator(char *s, int i, t_token **head)
{
    int space = 0;

    if (s[i + 1] == '&')
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup("&&"), AND_IF, 0, space);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup("&"), AND, 0, space);
        return i + 1;
    }
}

/* Handle OR and PIPE operator tokenization */
int handle_or_pipe_operator(char *s, int i, t_token **head)
{
    int space = 0;

    if (s[i + 1] == '|')
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup("||"), OR_IF, 0, space);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("|"), PIPE, 0, space);
        return i + 1;
    }
}

/* Handle output redirection tokenization */
int handle_output_redirection(char *s, int i, t_token **head)
{
    int space = 0;

    if (s[i + 1] == '>')
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup(">>"), APPEND, 0, space);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup(">"), REDIRECTION_OUT, 0, space);
        return i + 1;
    }
}

/* Handle input redirection tokenization */
int handle_input_redirection(char *s, int i, t_token **head)
{
    int space = 0;

    if (s[i + 1] == '<')
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup("<<"), HERE_ODC, 0, space);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("<"), REDIRECTION_IN, 0, space);
        return i + 1;
    }
}

/* Handle parentheses tokenization (for bonus) */
int handle_parentheses(char *s, int i, t_token **head)
{
    int space = 0;

    if (s[i] == '(')
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("("), OPEN_PER, 0, space);
        return i + 1;
    }
    else if (s[i] == ')')
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup(")"), CLOSE_PER, 0, space);
        return i + 1;
    }
    return i;
}

/* Handle assignment operator tokenization */
int handle_assignment(char *s, int i, t_token **head)
{
    int space = 0;

    if (ft_isspace(s[i + 1]))
            space = 1;
    add_token(head, strdup("="), ASSIGN, 0, space);
    return i + 1;
}

/* Handle wildcard tokenization (for bonus) */
int handle_wildcard(char *s, int i, t_token **head)
{
    int space = 0;

    if (ft_isspace(s[i + 1]))
            space = 1;
    add_token(head, strdup("*"), WILDCARD, 0, space);
    return i + 1;
}

/* Handle dollar sign for environment variables and exit status */
int handle_dollar(char *s, int i, t_token **head)
{
    int space = 0;

    if (s[i + 1] == '?')
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup("$?"), EXIT_STATUS, 0, space);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("$"), DOLLAR, 0, space);
        return i + 1;
    }
}

/* Handle word tokenization */
int handle_word(char *s, int i, t_token **head)
{
    int start = i;
    int space = 0;

    while (s[i] && !ft_isspace(s[i]) && !ft_is_operator_char(s[i]) && 
           !ft_is_quote(s[i]) && s[i] != '$')
        i++;
    if (ft_isspace(s[i]))
        space = 1;
    char *token = ft_strndup(&s[start], i - start);
    if (strcmp(token, "echo") == 0)
        add_token(head, token, BUILTIN_ECHO, 0, space);
    else if (strcmp(token, "cd") == 0)
        add_token(head, token, BUILTIN_CD, 0, space);
    else if (strcmp(token, "pwd") == 0)
        add_token(head, token, BUILTIN_PWD, 0, space);
    else if (strcmp(token, "export") == 0)
        add_token(head, token, BUILTIN_EXPORT, 0, space);
    else if (strcmp(token, "unset") == 0)
        add_token(head, token, BUILTIN_UNSET, 0, space);
    else if (strcmp(token, "env") == 0)
        add_token(head, token, BUILTIN_ENV, 0, space);
    else if (strcmp(token, "exit") == 0)
        add_token(head, token, BUILTIN_EXIT, 0, space);
    else
        add_token(head, token, WORD, 0, space);
    
    return (i);
}

void tokenize_input(char *s, t_token **head)
{
    int i = 0;

    while (s[i])
    {
        if (ft_isspace(s[i]))
            i++;
        else if (ft_is_quote(s[i]))
            i = handle_quotes(s, i, head);
        else if (s[i] == '(' || s[i] == ')')
            i = handle_parentheses(s, i, head);
        else if (s[i] == '&')
            i = handle_and_operator(s, i, head);
        else if (s[i] == '|')
            i = handle_or_pipe_operator(s, i, head);
        else if (s[i] == '>')
            i = handle_output_redirection(s, i, head);
        else if (s[i] == '<')
            i = handle_input_redirection(s, i, head);
        else if (s[i] == '=')
            i = handle_assignment(s, i, head);
        else if (s[i] == '*')
            i = handle_wildcard(s, i, head);
        else if (s[i] == '$')
            i = handle_dollar(s, i, head);
        else
            i = handle_word(s, i, head);
    }
}

int count_args(t_token *head)
{
    int i = 0;
    t_token *current = head->next->next;
    
    while (current && (current->type == 0 || current->type >= 18))
    {
        i++;
        current = current->next;
    }
    return (i);
}

void move_args(t_token **head)
{
    int args_count = count_args(*head);
    if (args_count == 0)
        return;
    
    t_token *redir_node = *head;
    t_token *file_node = redir_node->next;
    t_token *first_arg = file_node->next;
    t_token *last_arg = first_arg;
    t_token *after_args;
    for (int i = 1; i < args_count; i++)
        last_arg = last_arg->next;
    after_args = last_arg->next;
    file_node->next = after_args;
    if (after_args)
        after_args->prev = file_node;
    t_token *before_redir = redir_node->prev;
    first_arg->prev = before_redir;
    last_arg->next = redir_node;
    if (before_redir)
        before_redir->next = first_arg;
    else
        *head = first_arg;
    redir_node->prev = last_arg;
}

void revise_redirections(t_token **head)
{
    t_token *current = *head;
    t_token *next;
    
    while (current)
    {
        next = current->next;
        
        if (current->type >= 6 && current->type <= 9 && current->next)
        {
            t_token *temp_head = current;
            move_args(&temp_head);
            if (temp_head != current && current == *head)
                *head = temp_head;
        }
        current = next;
    }
}

void revise_args(t_token **head)
{
    t_token *current = *head;
    
    while (current)
    {
        // Check if this token needs joining
        if ((current->type == 0 || current->type >= 19 || 
             current->type == 10 || current->type == 11) && 
             current->space_after == 0)
        {
            // Start building the combined token
            char *combined = strdup(current->token);
            if (!combined)
                return; // Memory allocation error
            
            // Save start position for node replacement
            t_token *start_node = current;
            t_token *next_node = current->next;
            t_token *last_joined = current;
            
            // Join all consecutive tokens without spaces
            while (next_node && (next_node->type == 0 || next_node->type >= 19 || 
                   next_node->type == 10 || next_node->type == 11 || next_node->type == 12 || next_node->type == 17) && 
                   last_joined->space_after == 0)
            {
                char *temp = combined;
                combined = ft_strjoin(temp, next_node->token);
                free(temp);
                
                if (!combined)
                    return; // Memory allocation error
                
                last_joined = next_node;
                next_node = next_node->next;
            }
            
            // Create a new token with the combined string
            char *new_token_str = strdup(combined);
            if (!new_token_str)
            {
                free(combined);
                return; // Memory allocation error
            }
            
            // Get the space_after value from the last token in the group
            int space_after = last_joined->space_after;
            
            // Fix the list linkage to skip the nodes that will be removed
            t_token *prev_node = start_node->prev;
            
            // Remove all nodes from start_node to last_joined
            t_token *to_remove = start_node;
            while (to_remove != next_node)
            {
                t_token *next_to_remove = to_remove->next;
                free(to_remove->token);
                free(to_remove);
                to_remove = next_to_remove;
            }
            
            // Create a new node with the combined token
            t_token *new_node = malloc(sizeof(t_token));
            if (!new_node)
            {
                free(combined);
                free(new_token_str);
                return; // Memory allocation error
            }
            
            // Initialize the new node
            memset(new_node, 0, sizeof(t_token));
            new_node->token = new_token_str;
            new_node->type = 0; // Keep the type of the first token
            new_node->space_after = space_after;
            new_node->next = next_node;
            new_node->prev = prev_node;
            
            // Update the list connections
            if (prev_node)
                prev_node->next = new_node;
            else
                *head = new_node; // If we replaced the first node, update the head
                
            if (next_node)
                next_node->prev = new_node;
                
            // For debugging
            //printf("%s\n", combined);
            free(combined);
            
            // Move to the next token after our new combined token
            current = next_node;
        }
        else
        {
            // Move to the next token normally
            current = current->next;
        }
    }
}

// void    revise_args(t_token **head)
// {
//     t_token *current = *head;
//     t_token *tmp;
//     char *s = NULL;

//     while (current)
//     {
//         s = NULL;
//         tmp = current;

//         while (tmp && ((tmp->type == 0 || tmp->type >= 19 || tmp->type == 10 || tmp->type == 11) && tmp->space_after == 0))
//         {
//             char *new_s;
//             if (s == NULL)
//                 new_s = strdup(tmp->token);
//             else
//                 new_s = ft_strjoin(s, tmp->token);
//             if (s)
//                 free(s);
//             s = new_s;
//             tmp = tmp->next;
//         }
//         if (s)
//         {
//             printf("%s\n", s);
//             free(s);
//         }
//         if (tmp && tmp != current)
//         {
//             current = tmp;
//             // free(s);
//             // tmp = NULL;
//         }
//         else
//             current = current->next;
//     }
// }

t_token *lexer(char *s)
{
    t_token *head = NULL;
    
    tokenize_input(s, &head);
    //exapnd_args(&head)
    revise_args(&head);
    revise_redirections(&head);
    //print_token_list(&head);
    return (head);
}