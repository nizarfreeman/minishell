#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
# include "../pars/minishell.h"
# include "wait.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
extern int		g_han;

typedef struct env
{
	char		*value;
	char		*tmp;
	int			f;
	struct env	*next;
}				env;

void			gc_free(void);
env				*ft_lstnew(env **lst, void *content, int f);
void			ft_lstadd_back(env **lst, env *new);
int				is_all_num(char *s);
void			*ft_malloc(int size);
char			*ft_strjoin1(char *s1, char *s2);
void			ft_free(void);
void			search_replace1(env *env, char *key, char *rep);
env				*ft_lstnew1(env **lst, void *content, int f);
char			*ft_strdup1(const char *s1);
void			filter_mid2(env **files, int found, env **tmp, env **prev);
env				*ft_lstlast(env *lst);
char			*ft_strdup(const char *s1);
void			no_env2(env **ret);
void			*ft_memcpy(void *dst, const void *src, size_t n);
size_t			ft_strlen(const char *s);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strjoin(char *s1, char *s2);
void			cd3(char *path, char **tmp, char **pwd, char *oldpwd);
char			*ft_strrchr(const char *s, int c);
char			*ft_strchr(char *s, char c);
void			search_replace(env *env, char *key, char *rep);
int				ft_strcmp(const char *s1, const char *s2);
int				cd(env *env, char **path, int *ex);
int				cd2(env *env, char *path);
int				is_space_tab(char c);
void			wildcar_split2(char *s, char **src, env **ret);
char			*creat_word(char *str, int f, char c, int *p);
int				expand4(char ***tmp1, env *envr, char **tmp);
int				expand3(char **ret, char **tmp1, env **list, env *envr);
int				expand5(env **list, char *tmp, env *envr);
int				export2(char *arg, env **envr);
void			split2(char **split, char *str, char charset);
char			**export_split(char *str);
int				expand_valid(char c);
int				expand_wildcard(char *s, env **ret);
void			red_in(t_tree *root, env **env, int *ex, t_tree *left);
int				is_all_empty(char **s);
void			red_app(t_tree *root, env **env, int *ex, t_tree *left);
void			red_out(t_tree *root, env **env, int *ex, t_tree *left);
void			redirections_expand(t_tree *root, env **env, int *ex);
int				append_to_lst(char **ret, env **list, char *tmp1, char *tmp);
char			*expand_quotes(env *envr, int *p, char **tmp1);
char			*get_value(env *env, char *key);
void			no_env(env **ret);
void			remove_node(env **envrr, char *str);
env				*creat_env(char **envr);
void			repl(char c, char b, char *s);
void			filter_mid(env **files, env *arg);
void			repl1(char c, char b, char *s);
env				*wildcar_split(char *s);
void			get_dir(env **ret);
void			filter_last(env **files, env *arg);
void			filter_first(env **files, env *arg);
int				is_wildcard(char *s);
int				unset(env **env, char **str, int *ex);
int				pwd(env *env, int *ex);
int				echo(char **str, int *ex);
char			**ft_split(const char *s, char c);
int				envr(env *env, int *ex);
char			*word(char *s, char *e);
int				check_value(char *str);
int				export(env **envr, char **args, int *ex);
int				ft_lstsize(env *env);
char			*get_path(char **cmd, char **env);
char			*trim_last_dir(char *path);
char			*creat_path(char *cmd, char **path);
void			printerr(char *cmd, int i);
int				exec_tree(t_tree *root, env **env, int *ex);
int				excute(char **cmd, env **env, int fd_in, int *ex);
long long		ft_atoi(const char *str);
int				excute_cmd(char **cmd, env **env, int fd_in, int *status);
int				exce_pipe(t_tree *root, env **env, int *exi);
void			handle_int(int sig);
char			**lst_to_arr2(env *env);
char			*ft_itoa(int n);
char			**lst_to_arr(env *env);
int				ft_atoi2(const char *str);
void			my_exit(char **cmd, int *ex);
void			redirections(t_tree *root, env **env, int *ex, t_tree *left);
char			**expand(char **args, env *envr, int *ex);
char			**pre_expand(char **args, env *envr, int *ex);
char			*get_next_line(int fd);
void			*ft_memcpy(void *dst, const void *src, size_t n);
char			*expand2(char *str, env *envr, int *ex);
int				*get_exit_status(int *ex);
int				read_and_expand(int fd, env *envr, int *ex);
void			*gc_malloc(int size);
void			gc_free(void);
#endif