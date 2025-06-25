/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aayache <aayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 21:26:26 by nfaska            #+#    #+#             */
/*   Updated: 2025/06/25 17:14:03 by aayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARS_H
# define PARS_H 

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "includes.h"
# include "utilities/utilities.h"
# include "parser/parser.h"
# include "syntax_check/syntax_check.h"
# include "lexer/lexer.h"

int					add_token(t_token **head, t_arg *arg);
int					syntax_check(t_token *head);

#endif