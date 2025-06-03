#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <unistd.h>
# include <stdbool.h>
# include <time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_tokentype
{
    WORD,
    HIPHEN, // -
    PIPE, // |
    AND, // &
    AND_IF, // &&
    OR_IF, // ||
    REDIRECTION_OUT, // >
    REDIRECTION_IN, // <
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

typedef struct tokens
{
    char    *token;
    t_tokentype type;
    int quoted;
    int space;
    struct tokens   *next;
    struct tokens   *prev;
    char *file;
} t_token;

typedef struct token_arg
{
    char *token;
    int type;
    int quoted;
    int space;
    char *file;
} t_arg;

typedef struct dsa_arg
{
    int type;
    char *cmd;
    char **args;
    char *file;
    int fd;
} t_dsa_arg;

typedef struct dsa
{
    struct dsa *left;
    struct dsa *right;
    int type;
    char *cmd;
    char **args;
    char *file;
    int fd; 
} t_tree;


typedef struct gc
{
    void    *ptr;
    struct gc *next;
} t_gc;

int add_token(t_token **head, t_arg *arg);
int syntax_check(t_token *head);
/* ========================================================================== */
/*                              MODULE INCLUDES                               */
/* ========================================================================== */

/* 
 * Include the PROJECT module headers.
 * These should be included after the main structures are defined
 * to avoid circular dependencies.
 */

# include "PROJECT/lexer/lexer.h"
# include "PROJECT/parser/parser.h"
# include "PROJECT/syntax_check/syntax_check.h"
# include "PROJECT/utilities/utilities.h"

# endif