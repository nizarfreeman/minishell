# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "wait.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <dirent.h>
#include "../pars/minishell.h"
extern int sig_han;

typedef struct env
{
	char	*value;
	char	*tmp;
	int		f;
	struct	env *next;
} env;

env	*ft_lstnew(env **lst, void *content, int f);
env	*ft_lstlast(env *lst);
char	*ft_strdup(const char *s1);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strrchr(const char *s, int c);
char	*ft_strchr(char *s, char c);
void search_replace(env *env, char *key, char *rep);
int	ft_strcmp(const char *s1, const char *s2);
int  cd(env *env, char *path, int *ex);
int  cd2(env *env, char *path);
char *get_value(env *env, char *key);
void no_env(env **ret);
void remove_node(env **envrr, char *str);
env *creat_env(char **envr);
void	repl(char c, char b, char *s);
void	repl1(char c, char b, char *s);
int  unset(env **env, char **str, int *ex);
int  pwd(env *env, int *ex);
int  echo(char **str, int *ex);
char	**ft_split(const char *s, char c);
int 	envr(env* env, int *ex);
char	*word(char *s, char *e);
int check_value(char *str);
int export(env **envr, char **args, int *ex);
int ft_lstsize(env *env);
char	*get_path(char **cmd, char **env);
char	*creat_path(char *cmd, char **path);
void	printerr(char *cmd, int i);
int exec_tree(t_tree *root, env **env, int *ex);
int excute(char **cmd, env **env, int fd_in, int *ex);
long long	ft_atoi(const char *str);
int excute_cmd(char **cmd, env **env, int fd_in, int *status);
int exce_pipe(t_tree *root, env **env, int *exi);
void handle_int(int sig);
char **lst_to_arr2(env *env);
char	*ft_itoa(int n);
char **lst_to_arr(env *env);
int ft_atoi2(const char *str);
void my_exit(char **cmd, int *ex);
void redirections(t_tree *root, env **env, int *ex, t_tree *left);
char **expand(char **args, env *envr, int *ex);
char **pre_expand(char **args, env *envr, int *ex);
char	*get_next_line(int fd);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char *expand2(char *str, env *envr, int *ex);
int *get_exit_status(int *ex);
int read_and_expand(int fd, env *envr, int *ex);
#endif