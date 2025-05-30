#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
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
    return (c == ' ' || c == '\t' || c == '\n');
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

/* GC-safe version of strdup */
char *gc_strdup(const char *s)
{
    if (!s)
        return NULL;
    
    size_t len = ft_strlen((char *)s);
    char *result = gc_malloc(len + 1);
    
    if (!result)
        return NULL;
    
    strcpy(result, s);
    return result;
}

/* Joins two strings into a new one */
char    *ft_strjoin(char *s1, char *s2)
{
    size_t  len1;
    size_t  len2;
    char    *res;
    size_t  i;
    size_t  j;

    len1 = 0;
    len2 = 0;
    if (s1)
        len1 = ft_strlen(s1);
    if (s2)
        len2 = ft_strlen(s2);
    res = gc_malloc(len1 + len2 + 1);
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    while (s1 && j < len1)
        res[i++] = s1[j++];
    j = 0;
    while (s2 && j < len2)
        res[i++] = s2[j++];
    res[i] = '\0';
    return (res);
}

char    *ft_strncpy(char *dest, const char *src, unsigned int n)
{
    int i;

    i = 0;

    while (src[i] && i <= n)
    {
        dest[i] = src[i];
        i++;
    }
    while (i <= n)
    {
        dest[i++] = '\0';
    }
    return (dest);
}

/* A custom implementation of strndup */
char *ft_strndup(const char *s, size_t n)
{
    char *result;

    result = gc_malloc(n + 1);
    if (!result)
        return NULL;
    ft_strncpy(result, s, n);
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

void    *ft_memset(void *s, int c, size_t n)
{
    unsigned char   *src;

    src = (unsigned char *) s;
    while (n--)
        *src++ = (unsigned char) c;
    return (s);
}

/* Handle quoted string tokenization */
int handle_quotes(char *s, int i, t_token **head, int preserve_quotes)
{
    char    quote;
    int     start;
    int     space;
    char    *token;
    int     type;
    t_arg *arg;

    // Allocate memory for arg - this was missing!
    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i; // Handle allocation failure

    quote = s[i];
    start = i;
    space = 0;
    i++;
    while (s[i] && s[i] != quote)
        i++;
    if (s[i] == quote)
        i++;
    
    // Initialize arg fields
    arg->space = 0;  // Initialize to 0 first
    if (ft_isspace(s[i]))
        arg->space = 1;
    
    arg->token = ft_strndup(&s[start], i - start);
    if (preserve_quotes)
        arg->type = WORD;
    else if (quote == '\'')
    {
        arg->quoted = 1;
        arg->type = S_QUOTE;
    }
    else
    {
        arg->quoted = 1;
        arg->type = D_QUOTE;
    }
    arg->file = NULL;
    add_token(head, arg);
    return (i);
}

/* Handle AND operator tokenization */
int handle_and_operator(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (s[i + 1] == '&')
    {
        if (ft_isspace(s[i + 2]))
            arg->space = 1;
        arg->token = gc_strdup("&&");
        arg->type = AND_IF;
        add_token(head, arg);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup("&");
        arg->type = WORD;
        add_token(head, arg);
        return i + 1;
    }
}

/* Handle OR and PIPE operator tokenization */
int handle_or_pipe_operator(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (s[i + 1] == '|')
    {
        if (ft_isspace(s[i + 2]))
            arg->space = 1;
        arg->token = gc_strdup("||");
        arg->type = OR_IF;
        add_token(head, arg);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup("|");
        arg->type = PIPE;
        add_token(head, arg);
        return i + 1;
    }
}

/* Handle output redirection tokenization */
int handle_output_redirection(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (s[i + 1] == '>')
    {
        if (ft_isspace(s[i + 2]))
            arg->space = 1;
        arg->token = gc_strdup(">>");
        arg->type = APPEND;
        add_token(head, arg);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup(">");
        arg->type = REDIRECTION_OUT;
        add_token(head, arg);
        return i + 1;
    }
}

/* Handle input redirection tokenization */
int handle_input_redirection(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (s[i + 1] == '<')
    {
        if (ft_isspace(s[i + 2]))
            arg->space = 1;
        arg->token = gc_strdup("<<");
        arg->type = HERE_ODC;
        add_token(head, arg);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup("<");
        arg->type = REDIRECTION_IN;
        add_token(head, arg);
        return i + 1;
    }
}

/* Handle parentheses tokenization (for bonus) */
int handle_parentheses(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (s[i] == '(')
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup("(");
        arg->type = OPEN_PER;
        add_token(head, arg);
        return i + 1;
    }
    else if (s[i] == ')')
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup(")");
        arg->type = CLOSE_PER;
        add_token(head, arg);
        return i + 1;
    }
    return i;
}

/* Handle assignment operator tokenization */
int handle_assignment(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (ft_isspace(s[i + 1]))
        arg->space = 1;
    arg->token = gc_strdup("=");
    arg->type = ASSIGN;
    add_token(head, arg);
    return i + 1;
}

/* Handle wildcard tokenization (for bonus) */
int handle_wildcard(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (ft_isspace(s[i + 1]))
        arg->space = 1;
    arg->token = gc_strdup("*");
    arg->type = WORD;
    add_token(head, arg);
    return i + 1;
}

/* Handle dollar sign for environment variables and exit status */
int handle_dollar(char *s, int i, t_token **head)
{
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    arg->space = 0;  // Initialize to 0
    arg->quoted = 0;
    arg->file = NULL;
    
    if (s[i + 1] == '?')
    {
        if (ft_isspace(s[i + 2]))
            arg->space = 1;
        arg->token = gc_strdup("$?");
        arg->type = WORD;
        add_token(head, arg);
        return i + 2;
    }
    else
    {
        if (ft_isspace(s[i + 1]))
            arg->space = 1;
        arg->token = gc_strdup("$");
        arg->type = DOLLAR;
        add_token(head, arg);
        return i + 1;
    }
}

/* Handle word tokenization */
int handle_word(char *s, int i, t_token **head)
{
    int start = i;
    t_arg *arg;

    arg = gc_malloc(sizeof(t_arg));
    if (!arg)
        return i;
    
    while (s[i] && !ft_isspace(s[i]) && !ft_is_operator_char(s[i]) && 
           !ft_is_quote(s[i]) && s[i] != '$')
        i++;
    
    arg->space = 0;  // Initialize to 0
    if (ft_isspace(s[i]))
        arg->space = 1;
    arg->token = ft_strndup(&s[start], i - start);
    arg->quoted = 0;
    arg->file = NULL;
    
    // Set appropriate type based on the token
    if (strcmp(arg->token, "echo") == 0)
        arg->type = BUILTIN_ECHO;
    else if (strcmp(arg->token, "cd") == 0)
        arg->type = BUILTIN_CD;
    else if (strcmp(arg->token, "pwd") == 0)
        arg->type = BUILTIN_PWD;
    else if (strcmp(arg->token, "export") == 0)
        arg->type = BUILTIN_EXPORT;
    else if (strcmp(arg->token, "unset") == 0)
        arg->type = BUILTIN_UNSET;
    else if (strcmp(arg->token, "env") == 0)
        arg->type = BUILTIN_ENV;
    else if (strcmp(arg->token, "exit") == 0)
        arg->type = BUILTIN_EXIT;
    else
        arg->type = WORD;
    
    add_token(head, arg);
    return i;
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
        return;
    t_token *cmd_token = find_current_command_token(start);
    if (!cmd_token || cmd_token == end)
        return;
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
        return;
        
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
void handle_redirection_first(t_token **head, t_token *start, t_token *end)
{
    if (!start || !is_redirection(start))
        return;
    t_token *current = start;
    t_token *command_candidate = NULL;
    
    while (current && current != end && !is_pipe_or_logical(current))
    {
        if (is_redirection(current))
        {
            current = current->next;
            if (current)
                current = current->next;
            continue;
        }
        if (!is_redirection_target(current))
        {
            command_candidate = current;
            break;
        }
        
        current = current->next;
    }
    if (command_candidate)
    {
        extract_node(head, command_candidate);
        if (start->prev)
        {
            command_candidate->prev = start->prev;
            command_candidate->next = start;
            start->prev->next = command_candidate;
            start->prev = command_candidate;
        }
        else
        {
            command_candidate->next = start;
            start->prev = command_candidate;
            *head = command_candidate;
        }
    }
}

void revise_redirections(t_token **head)
{
    if (!head || !*head)
        return;
    t_token *current = *head;
    t_token *segment_start = current;
    while (current)
    {
        if (is_pipe_or_logical(current))
        {
            handle_redirection_first(head, segment_start, current);
            segment_start = current->next;
        }
        
        current = current->next;
    }
    if (segment_start)
        handle_redirection_first(head, segment_start, NULL);
    reorganize_command_args(head);
}

void revise_args(t_token **head)
{
    if (!head || !*head)
        return;
        
    t_token *current = *head;
    while (current)
    {
        if ((current->type == WORD || current->type == S_QUOTE || 
             current->type == D_QUOTE || current->type == DOLLAR) && 
             current->space == 0)
        {
            char *combined = gc_strdup(current->token);
            if (!combined)
                return;
                
            t_token *next_token = current->next;
            t_token *last_joined = current;
            while (next_token && 
                   (next_token->type == WORD || next_token->type == S_QUOTE || 
                    next_token->type == D_QUOTE || next_token->type == DOLLAR) &&
                   last_joined->space == 0)
            {
                char *temp = combined;
                combined = ft_strjoin(temp, next_token->token);
                if (!combined)
                    return;
                last_joined = next_token;
                next_token = next_token->next;
            }
            if (last_joined != current)
            {
                current->token = combined;
                current->type = WORD;
                current->space = last_joined->space;
                t_token *to_remove = current->next;
                current->next = next_token;
                if (next_token)
                    next_token->prev = current;
                while (to_remove && to_remove != next_token)
                {
                    t_token *temp = to_remove->next;
                    if (to_remove->prev)
                        to_remove->prev->next = to_remove->next;
                    if (to_remove->next)
                        to_remove->next->prev = to_remove->prev;
                    to_remove->prev = NULL;
                    to_remove->next = NULL;
                    to_remove = temp;
                }
            }
        }  
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
    ret = gc_malloc(len + 1);
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
    int i = 0;
    int j = 0;
    char tmp;

    if (!str)
        return (NULL);
    
    i = 0;
    j = strlen(str) - 1;
    
    while (j > i)
    {
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
    return (str);
}

// char *ft_itoa(int nbr)
// {
//     int i;
//     int neg;
//     char *tmp;

//     i = 0;
//     neg = 0;
//     if (nbr == 0)
//         return (gc_strdup("0"));
//     if (nbr == -2147483648)
//         return (gc_strdup("-2147483648"));
    
//     tmp = gc_malloc(sizeof(char) * 12);
//     if (tmp == NULL)
//         return (NULL);
//     ft_memset(tmp, 0, 12);
    
//     if (nbr < 0)
//     {
//         neg = 1;
//         nbr = -nbr;
//     }
//     while (nbr > 0)
//     {
//         tmp[i++] = (nbr % 10) + '0';
//         nbr /= 10;
//     }
//     if (neg)
//         tmp[i++] = '-';
//     tmp[i] = '\0';
//     return (ft_strrev(tmp));
// }

int create_temp_file(void)
{
    char *name;
    int fd;

    name = ft_strjoin("/tmp/", ft_itoa(rand()));
    if (!name)
        return (-1);
    fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
    return (fd);
}

int read_heredoc_input(int fd, char *delim)
{
    char *s;

    while ((s = readline("> ")) != NULL)
    {
        if (strcmp(s, delim) == 0)
        {
            free(s);
            return (0);
        }
        write(fd, s, strlen(s));
        write(fd, "\n", 1);
        free(s);
    }
    return (-1);
}

int process_single_heredoc(t_token *token, int comm_fd)
{
    char *name;
    char *delim;
    int fd;
    int result;

    name = ft_strjoin("/tmp/", ft_itoa(rand()));
    if (!name)
        return (-1);
    fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1)
        return (-1);
    delim = unquote_string(token->next->token);
    if (!delim)
    {
        close(fd);
        return (-1);
    }
    result = read_heredoc_input(fd, delim);
    close(fd);
    if (result == -1)
        return (-1);
    write(comm_fd, name, strlen(name));
    write(comm_fd, "\n", 1);
    return (0);
}

void child_process(t_token **head, char *comm_file)
{
    int comm_fd;
    t_token *list;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    comm_fd = open(comm_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (comm_fd == -1)
        exit(1);
    list = *head;
    while (list)
    {
        if (list->type == 9 && list->next)
        {
            if (process_single_heredoc(list, comm_fd) == -1)
            {
                close(comm_fd);
                exit(1);
            }
        }
        list = list->next;
    }
    close(comm_fd);
    exit(0);
}

void setup_signal_handling(struct sigaction *old_sigint)
{
    struct sigaction ignore_action;

    ignore_action.sa_handler = SIG_IGN;
    sigemptyset(&ignore_action.sa_mask);
    ignore_action.sa_flags = 0;
    sigaction(SIGINT, &ignore_action, old_sigint);
}

void parse_buffer_to_tokens(char *buffer, t_token **list)
{
    char *line_start;
    char *newline_pos;

    line_start = buffer;
    while (*list && (newline_pos = strchr(line_start, '\n')) != NULL)
    {
        if ((*list)->type == 9 && (*list)->next)
        {
            *newline_pos = '\0';
            (*list)->file = gc_strdup(line_start);
            line_start = newline_pos + 1;
        }
        *list = (*list)->next;
    }
}

void read_comm_file_data(char *comm_file, t_token **head)
{
    int comm_fd;
    char buffer[1024];
    int bytes_read;
    t_token *list;

    comm_fd = open(comm_file, O_RDONLY);
    if (comm_fd == -1)
        return;
    bytes_read = read(comm_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
    {
        close(comm_fd);
        return;
    }
    buffer[bytes_read] = '\0';
    list = *head;
    parse_buffer_to_tokens(buffer, &list);
    close(comm_fd);
}

void set_heredoc_files_null(t_token **head)
{
    t_token *list;

    list = *head;
    while (list)
    {
        if (list->type == 9 && list->next)
            list->file = NULL;
        list = list->next;
    }
}

void handle_child_success(char *comm_file, t_token **head)
{
    read_comm_file_data(comm_file, head);
}

void handle_child_failure(int status, t_token **head)
{
    set_heredoc_files_null(head);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        printf("\n");
}

void parent_process(pid_t pid, char *comm_file, t_token **head)
{
    struct sigaction old_sigint;
    int status;

    setup_signal_handling(&old_sigint);
    waitpid(pid, &status, 0);
    sigaction(SIGINT, &old_sigint, NULL);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        handle_child_success(comm_file, head);
    else
        handle_child_failure(status, head);
    unlink(comm_file);
}

void revise_heredocs(t_token **head)
{
    char *comm_file;
    pid_t pid;

    srand(time(NULL));
    comm_file = ft_strjoin("/tmp/", ft_itoa(getpid()));
    if (!comm_file)
        return ;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
        child_process(head, comm_file);
    else
        parent_process(pid, comm_file, head);
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