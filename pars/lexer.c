#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
int handle_quotes(char *s, int i, t_token **head, int preserve_quotes)
{
    char quote = s[i];
    int start = i;
    int space = 0;
    char *token;
    int type;

    i++;
    while (s[i] && s[i] != quote)
        i++;
    if (s[i] == quote)
        i++;
    if (ft_isspace(s[i]))
        space = 1;
    token = ft_strndup(&s[start], i - start);
    if (preserve_quotes)
        type = WORD;
    else
        type = (quote == '\'') ? S_QUOTE : D_QUOTE;
    add_token(head, token, type, 0, space, 0);
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
        add_token(head, strdup("&&"), AND_IF, 0, space, 0);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 2]))
            space = 1;
        add_token(head, strdup("&"), AND, 0, space, 0);
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
        add_token(head, strdup("||"), OR_IF, 0, space, 0);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("|"), PIPE, 0, space, 0);
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
        add_token(head, strdup(">>"), APPEND, 0, space, 0);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup(">"), REDIRECTION_OUT, 0, space, 0);
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
        add_token(head, strdup("<<"), HERE_ODC, 0, space, 0);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("<"), REDIRECTION_IN, 0, space, 0);
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
        add_token(head, strdup("("), OPEN_PER, 0, space, 0);
        return i + 1;
    }
    else if (s[i] == ')')
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup(")"), CLOSE_PER, 0, space, 0);
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
    add_token(head, strdup("="), ASSIGN, 0, space, 0);
    return i + 1;
}

/* Handle wildcard tokenization (for bonus) */
int handle_wildcard(char *s, int i, t_token **head)
{
    int space = 0;

    if (ft_isspace(s[i + 1]))
            space = 1;
    add_token(head, strdup("*"), WILDCARD, 0, space, 0);
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
        add_token(head, strdup("$?"), EXIT_STATUS, 0, space, 0);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            space = 1;
        add_token(head, strdup("$"), DOLLAR, 0, space, 0);
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
        add_token(head, token, BUILTIN_ECHO, 0, space, 0);
    else if (strcmp(token, "cd") == 0)
        add_token(head, token, BUILTIN_CD, 0, space, 0);
    else if (strcmp(token, "pwd") == 0)
        add_token(head, token, BUILTIN_PWD, 0, space, 0);
    else if (strcmp(token, "export") == 0)
        add_token(head, token, BUILTIN_EXPORT, 0, space, 0);
    else if (strcmp(token, "unset") == 0)
        add_token(head, token, BUILTIN_UNSET, 0, space, 0);
    else if (strcmp(token, "env") == 0)
        add_token(head, token, BUILTIN_ENV, 0, space, 0);
    else if (strcmp(token, "exit") == 0)
        add_token(head, token, BUILTIN_EXIT, 0, space, 0);
    else
        add_token(head, token, WORD, 0, space, 0);
    
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
            i = handle_quotes(s, i, head, 1);
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

int is_redirection(t_token *token)
{
    return (token && (token->type >= REDIRECTION_OUT && token->type <= HERE_ODC));
}

int is_command(t_token *token)
{
    return (token && (token->type >= BUILTIN_ECHO && token->type <= BUILTIN_EXIT));
}

int is_pipe_or_logical(t_token *token)
{
    return (token && (token->type == PIPE || token->type == AND_IF || token->type == OR_IF || 
                     token->type == AND));
}

int is_argument(t_token *token)
{
    return (token && (token->type == WORD || 
           (token->type >= BUILTIN_ECHO && token->type <= BUILTIN_EXIT)));
}

int is_redirection_target(t_token *token)
{
    return (token && token->prev && is_redirection(token->prev));
}

t_token *find_current_command_token(t_token *start)
{
    t_token *current = start;
    while (current && !is_pipe_or_logical(current))
    {
        if (is_command(current))
            return (current);
        current = current->next;
    }
    current = start;
    while (current && !is_pipe_or_logical(current))
    {
        if (!is_redirection(current) && !is_redirection_target(current))
            return (current);
        current = current->next;
    }
    return (start);
}

/* Find the last argument already adjacent to the command */
t_token *find_last_command_arg(t_token *cmd_token)
{
    t_token *last_arg = cmd_token;
    t_token *current = cmd_token->next;
    
    while (current && is_argument(current) && !is_redirection(current) && 
           !is_redirection_target(current) && !is_pipe_or_logical(current))
    {
        last_arg = current;
        current = current->next;
    }
    
    return (last_arg);
}

/* Extract a node from the linked list */
void extract_node(t_token **head, t_token *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else if (*head == node)
        *head = node->next;
        
    if (node->next)
        node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;
}

/* Insert a node after the specified position */
void insert_node_after(t_token **head, t_token *position, t_token *node)
{
    if (!position)
    {
        node->next = *head;
        if (*head)
            (*head)->prev = node;
        *head = node;
    }
    else
    {
        node->next = position->next;
        node->prev = position;
        
        if (position->next)
            position->next->prev = node;
            
        position->next = node;
    }
}

/* Process a single command segment (between operators) */
void reorganize_single_command(t_token **head, t_token *start, t_token *end)
{
    if (!start)
        return ;
    t_token *cmd_token = find_current_command_token(start);
    if (!cmd_token || cmd_token == end)
        return ;
    t_token *last_cmd_arg = find_last_command_arg(cmd_token);
    t_token *current = last_cmd_arg->next;
    while (current && current != end)
    {
        t_token *next = current->next;
        if (is_argument(current) && !is_redirection_target(current) && 
            !is_redirection(current->prev))
        {
            extract_node(head, current);
            insert_node_after(head, last_cmd_arg, current);
            last_cmd_arg = current;
        }
        
        current = next;
    }
}

/* Reorganize the token list to process each command segment separately */
void reorganize_command_args(t_token **head)
{
    if (!head || !*head)
        return ;
        
    t_token *current = *head;
    t_token *segment_start = current;
    while (current)
    {
        if (is_pipe_or_logical(current))
        {
            reorganize_single_command(head, segment_start, current);
            segment_start = current->next;
        }
        
        current = current->next;
    }
    if (segment_start)
        reorganize_single_command(head, segment_start, NULL);
}

/* Process token list to properly organize command arguments and redirections */
void revise_redirections(t_token **head)
{
    if (!head || !*head)
        return ;
    reorganize_command_args(head);
}

void revise_args(t_token **head)
{
    t_token *current = *head;
    
    while (current)
    {
        if ((current->type == 0 || current->type >= 19 || 
             current->type == 10 || current->type == 11 || 
             current->type == 12 || current->type == 17) && 
             current->space_after == 0)
        {
            char *combined = strdup(current->token);
            if (!combined)
                return;
            t_token *start_node = current;
            t_token *next_node = current->next;
            t_token *last_joined = current;
            if (current->type == 12 && next_node && next_node->type == 0)
            {
                char *temp = combined;
                combined = ft_strjoin(temp, next_node->token);
                free(temp);
                
                if (!combined)
                    return;
                    
                last_joined = next_node;
                next_node = next_node->next;
            }
            while (next_node && (next_node->type == 0 || next_node->type >= 19 || 
                   next_node->type == 10 || next_node->type == 11 || 
                   next_node->type == 12 || next_node->type == 17) &&  
                   last_joined->space_after == 0)
            {
                char *temp = combined;
                combined = ft_strjoin(temp, next_node->token);
                free(temp);
                
                if (!combined)
                    return;
                    
                last_joined = next_node;
                next_node = next_node->next;
            }
            
            char *new_token_str = strdup(combined);
            if (!new_token_str)
            {
                free(combined);
                return;
            }
            
            int space_after = last_joined->space_after;
            t_token *prev_node = start_node->prev;
            t_token *to_remove = start_node;
            while (to_remove != next_node)
            {
                t_token *next_to_remove = to_remove->next;
                free(to_remove->token);
                free(to_remove);
                to_remove = next_to_remove;
            }
            t_token *new_node = malloc(sizeof(t_token));
            if (!new_node)
            {
                free(combined);
                free(new_token_str);
                return;
            }
            
            memset(new_node, 0, sizeof(t_token));
            new_node->fd = 0;
            new_node->token = new_token_str;
            new_node->type = 0;
            new_node->space_after = space_after;
            new_node->next = next_node;
            new_node->prev = prev_node;
            if (prev_node)
                prev_node->next = new_node;
            else
                *head = new_node;    
                
            if (next_node)
                next_node->prev = new_node;
                
            free(combined);
            current = next_node;
        }
        else
            current = current->next;
    }
}

char *unquote_string(char *str)
{
    int len;
    int i = 0;
    int j = 0;
    int in_dquote = 0;
    int in_squote = 0;
    char *ret;
    
    len = strlen(str);
    ret = malloc(len + 1);
    if (!ret)
        return (NULL);
    
    while (i < len)
    {
        if (str[i] == '\"' && !in_squote)
        {
            in_dquote = !in_dquote;
            i++;
        }
        else if (str[i] == '\'' && !in_dquote)
        {
            in_squote = !in_squote;
            i++;
        }
        else
            ret[j++] = str[i++];
    }
    ret[j] = '\0';
    return (ret);
}

static char *ft_strrev(char *str)
{
    int i;
    int j;
    int tmp;

    i = 0;
    j = strlen(str);
    while (j > i)
    {
        j--;
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
    }
    return str;
}

// char    *ft_itoa(int nbr)
// {
//     int i;
//     int neg;
//     char *tmp;

//     i = 0;
//     neg = 0;
//     tmp = malloc(sizeof(char) * 12);
//     if (tmp == NULL || nbr == 0)
//         return ((nbr == 0) ? "0" : NULL);
//     if (nbr == -2147483648)
//         return ("-2147483648");
//     if (nbr < 0)
//     {
//         neg = 1;
//         nbr *= -1;
//     }
//     while (nbr)
//     {
//         tmp[i++] = (nbr % 10) + '0';
//         nbr /= 10;
//     }
//     if (neg)
//         tmp[i] = '-';
//     return ft_strrev(tmp);
// }

void revise_heredocs(t_token **head)
{
    srand(time(NULL));
    char *name = NULL;
    int fd;
    char *s = NULL;

    t_token *list = *head;
    while (list)
    {
        if (list->type == 9 && list->next)
        {
            name = ft_strjoin("/tmp/", ft_itoa(rand()));
            if (!name)
                return ;
            fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
            if (fd == -1)
            {
                free(name);
                return ;
            }
            s = readline("> ");
            while (s && strcmp(s, list->next->token) != 0)
            {
                write(fd, s, strlen(s));
                write(fd, "\n", 1);
                free(s);
                s = readline("> ");
            }
            if (s)
                free(s);
            
            fd = open(name, O_RDWR);
            if (fd == -1)
            {
                free(name);
                return;
            }
            list->fd = fd;
        }
        list = list->next;
    }
    return ;
}

t_token *lexer(char *s)
{
    t_token *head = NULL;
    
    tokenize_input(s, &head);
    revise_args(&head);
    revise_redirections(&head);
    revise_heredocs(&head);
    return (head);
}