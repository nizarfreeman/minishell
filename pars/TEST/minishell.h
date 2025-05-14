#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
#define SUBSHELL 13 
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
    int space_after;
    struct tokens   *next;
    struct tokens   *prev;
} t_token;

typedef struct dsa
{
    struct dsa *left;
    struct dsa *right;
    int type;
    char *cmd;
    char **args;
    char **files;
    int fd; 
} t_tree;

int check_quotes(char *s);

/*tokenizer*/
char    **split(const char *s, const char *delim);
t_token   *lexer(char *s);
void    free_token_list(t_token **head);
void    print_token_list(t_token **head);
int add_token(t_token **head, char *token, int type, int quoted, int space);
int is_there_char(char *s, char c);

/*parser*/
t_tree	*parse_expression(t_token *head);

/*tree print functions*/
int get_height(t_tree *root);
void print_node_data(t_tree *node);
void print_spaces(int count);
void print_level(t_tree *root, int level, int space_width);
void print_tree(t_tree *root);
void print_tree_horizontal(t_tree *root, int level, char *prefix, int is_left);
// void print_tree_h(t_tree *root);
void print_ast(t_tree *root);
int check_parenthesess(const char *text);

#endif