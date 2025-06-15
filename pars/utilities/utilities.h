/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfaska <nfaska@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 03:34:46 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/04 21:12:12 by nfaska           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTILITIES_H
# define UTILITIES_H
# include "../pars.h"

int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isspace(int c);
int		ft_is_operator_char(int c);
int		ft_is_quote(int c);
int		is_there_char(char *s, char c);
int		is_simple(t_token *head);
size_t	ft_strlen(char *s);

char	*gc_strdup(char *s);
char	*ft_strndup(char *s, size_t n);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
char	*ft_strndup(char *s, size_t n);
char	*ft_strcpy(char *s1, char *s2);

void	*ft_memset(void *s, int c, size_t n);
void	*gc_malloc(int size);
void	gc_free(void);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	set_variables(t_token **new_node, t_arg *arg);

#endif
