# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Ipars/PROJECT -Ipars

# Directories
SRC_DIRS = exc pars/PROJECT
MAIN = main.c
EXCLUDED = pars/PROJECT/main.c
EXEC = minishell

# Find all .c files recursively and exclude main files
SRC = $(filter-out $(MAIN) $(EXCLUDED), $(shell find $(SRC_DIRS) -name "*.c"))

# Object directory and object files
OBJ_DIR = obj
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(EXEC)

# Compile .c to .o into obj/ keeping directory structure
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link all objects with main.c and readline
$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) -lreadline -o $(EXEC)

# Clean rules
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(EXEC)

re: fclean all
