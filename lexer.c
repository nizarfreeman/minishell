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

t_token *lexer(char *s)
{
    t_token *head = NULL;
    
    tokenize_input(s, &head);
    return (head);

    /*print_token_list(&head);
    free_token_list(&head);*/
}