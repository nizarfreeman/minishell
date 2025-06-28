NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
OBJDIR      = obj
INCLUDES    = -Ipars -Ipars/lexer -Ipars/parser -Ipars/syntax_check -Ipars/utilities -Iexc
LIBS        = -lreadline

SRC = \
	main.c \
	exc/export_expand.c \
	exc/expand.c \
	exc/atoi.c \
	exc/err.c \
	exc/cd.c exc/cd1.c exc/cd2.c \
	exc/env.c \
	exc/exc.c exc/exc1.c exc/exc2.c exc/exc3.c exc/exc4.c exc/exc5.c exc/exc6.c exc/exc_tree.c \
	exc/exit.c \
	exc/export.c exc/export1.c \
	exc/gc.c \
	exc/gnl.c \
	exc/lst.c exc/lst1.c \
	exc/path.c \
	exc/redirections.c exc/redirections1.c \
	exc/split.c exc/split2.c \
	exc/spu.c \
	exc/strdup.c \
	exc/utils.c exc/utils1.c \
	pars/lexer/add_collect_args.c \
	pars/lexer/adjust_list.c \
	pars/lexer/child_process.c \
	pars/lexer/handle_and_operator.c \
	pars/lexer/handle_assignment.c \
	pars/lexer/handle_dollar.c \
	pars/lexer/handle_input_redirection.c \
	pars/lexer/handle_or_pipe_operator.c \
	pars/lexer/handle_output_redirection.c \
	pars/lexer/handle_parentheses.c \
	pars/lexer/handle_quote.c \
	pars/lexer/handle_wildcard.c \
	pars/lexer/handle_word.c \
	pars/lexer/heredoc_utilities.c \
	pars/lexer/insert_before_redirection.c \
	pars/lexer/is.c \
	pars/lexer/lexer.c \
	pars/lexer/parent_process.c \
	pars/lexer/parent_process_2.c \
	pars/lexer/revise_args.c \
	pars/lexer/revise_args_follow.c \
	pars/lexer/revise_heredoc.c \
	pars/lexer/revise_null.c \
	pars/lexer/revise_redirections.c \
	pars/lexer/tokenize_input.c \
	pars/lexer/validate_remove_args.c \
	pars/parser/buil_tree.c \
	pars/parser/buil_tree_utilities.c \
	pars/parser/check_paren_balance.c \
	pars/parser/create_token_arg.c \
	pars/parser/deep_copy_tokens.c \
	pars/parser/extract_paren_content.c \
	pars/parser/get_arg.c \
	pars/parser/get_root_pos.c \
	pars/parser/get_root_pos_utilities.c \
	pars/parser/insert_command.c \
	pars/parser/is_enclosed_in_parentheses.c \
	pars/parser/parse_expression.c \
	pars/parser/root.c \
	pars/parser/simple.c \
	pars/syntax_check/check_heredoc.c \
	pars/syntax_check/check_operators.c \
	pars/syntax_check/check_parentheses.c \
	pars/syntax_check/check_parentheses_2.c \
	pars/syntax_check/check_redirections.c \
	pars/syntax_check/check_start_end.c \
	pars/syntax_check/is_operator.c \
	pars/syntax_check/syntax_check.c \
	pars/utilities/ft_putstr.c \
	pars/utilities/ft_strndup.c \
	pars/utilities/gc_.c \
	pars/utilities/is_data_1.c \
	pars/utilities/is_data_and_utilites.c \
	pars/utilities/utilities.c

OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

all: $(NAME)

bonus: all

$(NAME): $(OBJ)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

$(OBJDIR)/%.o: %.c pars/pars.h pars/lexer/lexer.h pars/parser/parser.h pars/syntax_check/syntax_check.h pars/utilities/utilities.h pars/includes.h exc/s.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
