NAME = minishell_project

# Compiler and flags
CC = cc
#CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./utilities -I./lexer -I./parser -I./syntax_check -I..
LIBS = -lreadline

# Directory paths
LEXER_DIR = lexer
PARSER_DIR = parser
SYNTAX_DIR = syntax_check
UTILS_DIR = utilities
OBJ_DIR = obj

# Source files
LEXER_SRCS = $(LEXER_DIR)/add_collect_args.c \
			$(LEXER_DIR)/adjust_list.c \
			$(LEXER_DIR)/child_process.c \
			$(LEXER_DIR)/handle_and_operator.c \
			$(LEXER_DIR)/handle_assignment.c \
			$(LEXER_DIR)/handle_dollar.c \
			$(LEXER_DIR)/handle_input_redirection.c \
			$(LEXER_DIR)/handle_or_pipe_operator.c \
			$(LEXER_DIR)/handle_output_redirection.c \
			$(LEXER_DIR)/handle_parentheses.c \
			$(LEXER_DIR)/handle_quote.c \
			$(LEXER_DIR)/handle_wildcard.c \
			$(LEXER_DIR)/handle_word.c \
			$(LEXER_DIR)/heredoc_utilities.c \
			$(LEXER_DIR)/insert_before_redirection.c \
			$(LEXER_DIR)/is.c \
			$(LEXER_DIR)/lexer.c \
			$(LEXER_DIR)/parent_process_2.c \
			$(LEXER_DIR)/parent_process.c \
			$(LEXER_DIR)/revise_args.c \
			$(LEXER_DIR)/revise_args_follow.c \
			$(LEXER_DIR)/revise_heredoc.c \
			$(LEXER_DIR)/revise_null.c \
			$(LEXER_DIR)/revise_redirections.c \
			$(LEXER_DIR)/tokenize_input.c \
			$(LEXER_DIR)/validate_remove_args.c

PARSER_SRCS = $(PARSER_DIR)/buil_tree.c \
			$(PARSER_DIR)/buil_tree_utilities.c \
			$(PARSER_DIR)/check_paren_balance.c \
			$(PARSER_DIR)/create_token_arg.c \
			$(PARSER_DIR)/deep_copy_tokens.c \
			$(PARSER_DIR)/extract_paren_content.c \
			$(PARSER_DIR)/get_arg.c \
			$(PARSER_DIR)/get_root_pos.c \
			$(PARSER_DIR)/get_root_pos_utilities.c \
			$(PARSER_DIR)/insert_command.c \
			$(PARSER_DIR)/is_enclosed_in_parentheses.c \
			$(PARSER_DIR)/parse_expression.c \
			$(PARSER_DIR)/root.c\
			$(PARSER_DIR)/simple.c\

SYNTAX_SRCS = $(SYNTAX_DIR)/check_heredoc.c \
			$(SYNTAX_DIR)/check_operators.c \
			$(SYNTAX_DIR)/check_parentheses_2.c \
			$(SYNTAX_DIR)/check_parentheses.c \
			$(SYNTAX_DIR)/check_redirections.c \
			$(SYNTAX_DIR)/check_start_end.c \
			$(SYNTAX_DIR)/is_operator.c \
			$(SYNTAX_DIR)/syntax_check.c

UTILS_SRCS = $(UTILS_DIR)/ft_strndup.c \
			$(UTILS_DIR)/is_data_1.c \
			$(UTILS_DIR)/gc_.c\
			$(UTILS_DIR)/is_data_and_utilites.c \
			$(UTILS_DIR)/utilities.c\
			$(UTILS_DIR)/ft_putstr.c\

# Main source (assuming there's a main file in parent directory)
MAIN_SRCS = main.c

# All source files
SRCS = $(LEXER_SRCS) $(PARSER_SRCS) $(SYNTAX_SRCS) $(UTILS_SRCS) $(MAIN_SRCS)

# Object files
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Header files for dependency tracking
HEADERS = $(LEXER_DIR)/lexer.h \
		$(PARSER_DIR)/parser.h \
		$(SYNTAX_DIR)/syntax_check.h \
		$(UTILS_DIR)/utilities.h \
		../minishell.h

# Colors for output
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m

# Default target
all: $(NAME)

# Create object directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(LEXER_DIR)
	@mkdir -p $(OBJ_DIR)/$(PARSER_DIR)
	@mkdir -p $(OBJ_DIR)/$(SYNTAX_DIR)
	@mkdir -p $(OBJ_DIR)/$(UTILS_DIR)
	@mkdir -p $(OBJ_DIR)/..

# Compile object files
$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link the executable
$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(NC)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)✓ Object files cleaned!$(NC)"

# Clean everything
fclean: clean
	@echo "$(RED)Cleaning executable...$(NC)"
	@rm -f $(NAME)
	@echo "$(RED)✓ Everything cleaned!$(NC)"

# Rebuild everything
re: fclean all

# Debug build with extra flags
debug: CFLAGS += -DDEBUG -fsanitize=address
debug: $(NAME)

# Test compilation without linking
test: $(OBJS)
	@echo "$(GREEN)✓ All files compiled successfully!$(NC)"

# Show compilation info
info:
	@echo "$(YELLOW)Project: $(NAME)$(NC)"
	@echo "$(YELLOW)Compiler: $(CC)$(NC)"
	@echo "$(YELLOW)Flags: $(CFLAGS)$(NC)"
	@echo "$(YELLOW)Includes: $(INCLUDES)$(NC)"
	@echo "$(YELLOW)Libraries: $(LIBS)$(NC)"
	@echo "$(YELLOW)Source files: $(words $(SRCS))$(NC)"

# Help target
help:
	@echo "$(GREEN)Available targets:$(NC)"
	@echo "  $(YELLOW)all$(NC)     - Build the project"
	@echo "  $(YELLOW)clean$(NC)   - Remove object files"
	@echo "  $(YELLOW)fclean$(NC)  - Remove object files and executable"
	@echo "  $(YELLOW)re$(NC)      - Rebuild everything"
	@echo "  $(YELLOW)debug$(NC)   - Build with debug flags and AddressSanitizer"
	@echo "  $(YELLOW)test$(NC)    - Test compilation without linking"
	@echo "  $(YELLOW)info$(NC)    - Show compilation information"
	@echo "  $(YELLOW)help$(NC)    - Show this help message"

# Phony targets
.PHONY: all clean fclean re debug test info help

# Prevent make from deleting intermediate files
.PRECIOUS: $(OBJ_DIR)/%.o
