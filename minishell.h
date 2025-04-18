#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_tokentype {
    WORD,
    HIPHEN, // -
    PIPE, // |
    AND, // &
    AND_IF, // &&
    OR_IF, // ||
    REDIRECTION_OUT, // >
    REDIRECTION_INT, // <
    APPEND, // >>
    HERE_ODC, // <<
    S_QUOTE, // ''
    D_QUOTE, // ""
    DOLLAR, // $
    OPEN_PER, // (
    CLOSE_PER, // )
    WILDCARD, // *
    EXIT_STATUS, // $?
    ASSIGN, // = 
    BUILTIN_ECHO,
    BUILTIN_CD,
    BUILTIN_PWD,
    BUILTIN_EXPORT,
    BUILTIN_UNSET,
    BUILTIN_ENV,
    BUILTIN_EXIT
} t_tokentype;

typedef struct tokens {
    char    *token;
    t_tokentype type;
    int quoted;
    int space_after;
    struct tokens   *next;
    struct tokens   *prev;
} t_token;

char    **split(const char *s, const char *delim);
void    lexer(char *s);
void    free_token_list(t_token **head);
void    print_token_list(t_token **head);
int add_token(t_token **head, char *token, int type, int quoted, int space);

int check_quotes(char *s);


#endif