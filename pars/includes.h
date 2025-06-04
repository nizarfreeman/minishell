/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:11:16 by aayache           #+#    #+#             */
/*   Updated: 2025/06/05 00:13:47 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

typedef enum e_tokentype
{
	WORD,
	HIPHEN,
	PIPE,
	AND,
	AND_IF,
	OR_IF,
	REDIRECTION_OUT,
	REDIRECTION_IN,
	APPEND,
	HERE_ODC,
	S_QUOTE,
	D_QUOTE,
	DOLLAR,
	OPEN_PER,
	CLOSE_PER,
	WILDCARD,
	EXIT_STATUS,
	ASSIGN,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}					t_tokentype;

typedef struct tokens
{
	char			*token;
	t_tokentype		type;
	int				quoted;
	int				space;
	struct tokens	*next;
	struct tokens	*prev;
	char			*file;
}					t_token;

typedef struct token_arg
{
	char			*token;
	int				type;
	int				quoted;
	int				space;
	char			*file;
}					t_arg;

typedef struct dsa_arg
{
	int				type;
	char			*cmd;
	char			**args;
	char			*file;
	int				fd;
}					t_dsa_arg;

typedef struct dsa
{
	struct dsa		*left;
	struct dsa		*right;
	int				type;
	char			*cmd;
	char			**args;
	char			*file;
	int				fd;
}					t_tree;

typedef struct gc
{
	void			*ptr;
	struct gc		*next;
}					t_gc;

#endif
