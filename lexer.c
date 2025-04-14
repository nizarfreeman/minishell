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

/* Compute length of string */
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
    
    /* Skip until matching closing quote or end of string */
    while (s[i] && s[i] != quote)
        i++;
    /* Create token from quoted content */
    char *token = ft_strndup(&s[start], i - start);
    add_token(head, token, (quote == '\'' ? S_QUOTE : D_QUOTE), 1);
    /* Move past closing quote if present */
    if (s[i] == quote)
        i++;
    return i;
}

/* Handle AND operator tokenization */
int handle_and_operator(char *s, int i, t_token **head)
{
    if (s[i + 1] == '&')
    {
        add_token(head, strdup("&&"), AND_IF, 0);
        return i + 2;
    }
    else
    {
        add_token(head, strdup("&"), AND, 0);
        return i + 1;
    }
}

/* Handle OR and PIPE operator tokenization */
int handle_or_pipe_operator(char *s, int i, t_token **head)
{
    if (s[i + 1] == '|')
    {
        add_token(head, strdup("||"), OR_IF, 0);
        return i + 2;
    }
    else
    {
        add_token(head, strdup("|"), PIPE, 0);
        return i + 1;
    }
}

/* Handle output redirection tokenization */
int handle_output_redirection(char *s, int i, t_token **head)
{
    if (s[i + 1] == '>')
    {
        add_token(head, strdup(">>"), APPEND, 0);
        return i + 2;
    }
    else
    {
        add_token(head, strdup(">"), REDIRECTION_OUT, 0);
        return i + 1;
    }
}

/* Handle input redirection tokenization */
int handle_input_redirection(char *s, int i, t_token **head)
{
    if (s[i + 1] == '<')
    {
        add_token(head, strdup("<<"), HERE_ODC, 0);
        return i + 2;
    }
    else
    {
        add_token(head, strdup("<"), REDIRECTION_INT, 0);
        return i + 1;
    }
}

/* Handle parentheses tokenization (for bonus) */
int handle_parentheses(char *s, int i, t_token **head)
{
    if (s[i] == '(')
    {
        add_token(head, strdup("("), OPEN_PER, 0);
        return i + 1;
    }
    else if (s[i] == ')')
    {
        add_token(head, strdup(")"), CLOSE_PER, 0);
        return i + 1;
    }
    return i;
}

/* Handle assignment operator tokenization */
int handle_assignment(char *s, int i, t_token **head)
{
    add_token(head, strdup("="), ASSIGN, 0);
    return i + 1;
}

/* Handle wildcard tokenization (for bonus) */
int handle_wildcard(char *s, int i, t_token **head)
{
    add_token(head, strdup("*"), WILDCARD, 0);
    return i + 1;
}

/* Handle dollar sign for environment variables and exit status */
int handle_dollar(char *s, int i, t_token **head)
{
    /* Check for $? (exit status) */
    if (s[i + 1] == '?')
    {
        add_token(head, strdup("$?"), EXIT_STATUS, 0);
        return i + 2;
    }
    else
    {
        /* Regular environment variable - just mark the $ */
        add_token(head, strdup("$"), DOLLAR, 0);
        return i + 1;
    }
}

/* Handle word tokenization */
int handle_word(char *s, int i, t_token **head)
{
    int start = i;
    
    /* Keep going until we hit a special character or whitespace */
    while (s[i] && !ft_isspace(s[i]) && !ft_is_operator_char(s[i]) && 
           !ft_is_quote(s[i]) && s[i] != '$')
        i++;
    
    /* Create token from characters */
    char *token = ft_strndup(&s[start], i - start);
    
    /* Check if this is a built-in command */
    if (strcmp(token, "echo") == 0)
        add_token(head, token, BUILTIN_ECHO, 0);
    else if (strcmp(token, "cd") == 0)
        add_token(head, token, BUILTIN_CD, 0);
    else if (strcmp(token, "pwd") == 0)
        add_token(head, token, BUILTIN_PWD, 0);
    else if (strcmp(token, "export") == 0)
        add_token(head, token, BUILTIN_EXPORT, 0);
    else if (strcmp(token, "unset") == 0)
        add_token(head, token, BUILTIN_UNSET, 0);
    else if (strcmp(token, "env") == 0)
        add_token(head, token, BUILTIN_ENV, 0);
    else if (strcmp(token, "exit") == 0)
        add_token(head, token, BUILTIN_EXIT, 0);
    else
        add_token(head, token, WORD, 0);
    
    return i;
}

/*
 * tokenize_input:
 * Tokenizes the complete input string into individual tokens.
 * Handles all types of tokens defined in the minishell.h
 */
void tokenize_input(char *s, t_token **head)
{
    int i = 0;

    while (s[i])
    {
        /* Skip whitespace */
        if (ft_isspace(s[i]))
            i++;
        /* Handle quoted strings */
        else if (ft_is_quote(s[i]))
            i = handle_quotes(s, i, head);
        /* Handle parentheses (for bonus) */
        else if (s[i] == '(' || s[i] == ')')
            i = handle_parentheses(s, i, head);
        /* Handle AND operator */
        else if (s[i] == '&')
            i = handle_and_operator(s, i, head);
        /* Handle OR and PIPE operators */
        else if (s[i] == '|')
            i = handle_or_pipe_operator(s, i, head);
        /* Handle output redirection */
        else if (s[i] == '>')
            i = handle_output_redirection(s, i, head);
        /* Handle input redirection */
        else if (s[i] == '<')
            i = handle_input_redirection(s, i, head);
        /* Handle assignment operator */
        else if (s[i] == '=')
            i = handle_assignment(s, i, head);
        /* Handle wildcard */
        else if (s[i] == '*')
            i = handle_wildcard(s, i, head);
        /* Handle dollar sign (environment vars and exit status) */
        else if (s[i] == '$')
            i = handle_dollar(s, i, head);
        /* Handle regular words */
        else
            i = handle_word(s, i, head);
    }
}

/*
 * lex:
 * Lexing entry point that delegates to tokenize_input
 */
void lex(char *s, t_token **head)
{
    tokenize_input(s, head);
}

/*
 * lexer:
 * Creates the token list by calling the tokenizer.
 * Then prints and frees the list.
 */
void lexer(char *s)
{
    t_token *head = NULL;
    
    tokenize_input(s, &head);
    print_token_list(&head);
    free_token_list(&head);
}