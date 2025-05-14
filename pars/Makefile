CC = gcc
C_FLAGS = -g
LIBS = -lreadline
NAME = minishell
HEADER = minishell.h
OBJ_DIR = obj
SRCS = main.c\
        split.c\
        lexer.c\
        token_list.c\
        parser.c\
		print_tree.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c $(HEADER) | $(OBJ_DIR)
	@ $(CC) $(C_FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	 @ $(CC) $(C_FLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
