CC       = cc
C_FLAGS  = -g -I. -I pars 
LIBS     = -lreadline
NAME     = minishell
OBJ_DIR  = obj

PARS_SRC := $(filter-out pars/main.c, $(wildcard pars/*.c))
EXC_SRC  := $(filter-out exc/main.c, $(wildcard exc/*.c))
SRCS     := main.c $(PARS_SRC) $(EXC_SRC)

OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

# pattern rule: any %.c → obj/%.o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

# link
$(NAME): $(OBJS)
	$(CC) $(C_FLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
