/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s.h                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:18:02 by aayache           #+#    #+#             */
/*   Updated: 2025/06/27 22:13:44 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_H
# define S_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42

#  include "../pars/pars.h"
#  include "wait.h"
#  include <dirent.h>
#  include <errno.h>
#  include <fcntl.h>
#  include <limits.h>
#  include <readline/history.h>
#  include <readline/readline.h>
#  include <signal.h>
#  include <stdint.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <sys/stat.h>
#  include <unistd.h>

extern int			g_han;

typedef struct t_env
{
	char			*value;
	char			*tmp;
	int				f;
	struct t_env	*next;
}					t_env;

void				gc_free(void);
t_env				*ft_lstnew(t_env **lst, void *content, int f);
void				ft_lstadd_back(t_env **lst, t_env *new);
int					is_all_num(char *s);
void				*ft_malloc(int size);
char				*ft_strjoin1(char *s1, char *s2);
void				ft_free(void);
void				search_replace1(t_env *t_env, char *key, char *rep);
t_env				*ft_lstnew1(t_env **lst, void *content, int f);
char				*ft_strdup1(const char *s1);
void				filter_mid2(t_env **files, int found, t_env **tmp,
						t_env **prev);
t_env				*ft_lstlast(t_env *lst);
void				ft_dup2(int a, int b);
char				*rm_q(char *tmp);
char				*ft_strdup(const char *s1);
void				no_t_env2(t_env **ret);
void				*ft_memcpy(void *dst, const void *src, size_t n);
size_t				ft_strlen(char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strjoin(char *s1, char *s2);
void				cd3(char *path, char **tmp, char **pwd, char *oldpwd);
char				*ft_strrchr(const char *s, int c);
char				*ft_strchr(char *s, char c);
pid_t				ft_fork(int fd, int fd1);
void				search_replace(t_env *t_env, char *key, char *rep);
int					ft_strcmp(const char *s1, const char *s2);
int					cd(t_env *t_env, char **path, int *ex);
int					cd2(t_env *t_env, char *path);
int					is_space_tab(char c);
void				wildcar_split2(char *s, char **src, t_env **ret);
char				*creat_word(char *str, int f, char c, int *p);
int					expand4(char ***tmp1, t_env *t_envr, char **tmp);
int					expand3(char **ret, char **tmp1, t_env **list,
						t_env *t_envr);
int					expand5(t_env **list, char *tmp, t_env *t_envr);
int					export2(char *arg, t_env **t_envr);
void				split2(char **split, char *str, char charset);
char				**export_split(char *str);
int					expand_valid(char c);
int					expand_wildcard(char *s, t_env **ret);
void				red_in(t_tree *root, t_env **t_env, int *ex, t_tree *left);
int					is_all_empty(char **s);
void				red_app(t_tree *root, t_env **t_env, int *ex, t_tree *left);
void				red_out(t_tree *root, t_env **t_env, int *ex, t_tree *left);
int					redirections_expand(t_tree *root, t_env **t_env, int *ex);
int					append_to_lst(char **ret, t_env **list, char *tmp1,
						char *tmp);
char				*expand_quotes(t_env *t_envr, int *p, char **tmp1);
char				*get_value(t_env *t_env, char *key, int f);
void				no_t_env(t_env **ret);
void				remove_node(t_env **t_envrr, char *str);
char				**export_expand(char **s, t_env *t_envr, int *ex);
t_env				*creat_env(char **t_envr);
void				export_expand2(char *tmp, int *ex, t_env **ret,
						t_env *t_envr);
void				repl(char c, char b, char *s);
void				filter_mid(t_env **files, t_env *arg);
void				repl1(char c, char b, char *s);
t_env				*wildcar_split(char *s);
void				get_dir(t_env **ret);
void				filter_last(t_env **files, t_env *arg);
void				filter_first(t_env **files, t_env *arg);
int					is_wildcard(char *s);
char				*quote_string(char *str, char quote_char);
int					unset(t_env **t_env, char **str, int *ex);
int					pwd(t_env *t_env, int *ex);
int					echo(char **str, int *ex);
char				**ft_split(const char *s, char c);
int					t_envr(t_env *t_env, int *ex);
char				*word(char *s, char *e);
int					check_value(char *str);
int					export(t_env **t_envr, char **args, int *ex);
int					ft_lstsize(t_env *t_env);
char				*get_path(char **cmd, t_env **t_env1);
char				*trim_last_dir(char *path);
char				*creat_path(char *cmd, char **path);
int					exec_tree(t_tree *root, t_env **t_env, int *ex);
int					excute(char **cmd, t_env **t_env, int fd_in, int *ex);
long long			ft_atoi(const char *str);
int					excute_cmd(char **cmd, t_env **t_env, int fd_in,
						int *status);
int					exce_pipe(t_tree *root, t_env **t_env, int *exi);
void				handle_int(int sig);
char				**lst_to_arr2(t_env *t_env);
char				*ft_itoa(long long n);
char				**lst_to_arr(t_env *t_env);
void				printerr(char *cmd, int *ex);
int					ft_atoi2(const char *str);
void				my_exit(char **cmd, int *ex);
void				redirections(t_tree *root, t_env **t_env, int *ex,
						t_tree *left);
char				**expand(char **args, t_env *t_envr, int *ex);
char				**pre_expand(char **args, t_env *t_envr, int *ex);
char				*get_next_line(int fd);
void				*ft_memcpy(void *dst, const void *src, size_t n);
char				*expand2(char *str, t_env *t_envr, int *ex);
int					*get_exit_status(int *ex);
int					read_and_expand(int fd, t_env *t_envr, int *ex);
void				*gc_malloc(int size);
void				gc_free(void);

# endif
#endif