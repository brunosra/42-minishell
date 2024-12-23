# Nome do executável
NAME = minishell

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft

# Nome da biblioteca
LIB = minishell.a
LIBFT = $(LIBFT_DIR)/libft.a

# Compilador e flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -I $(INC_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft -L. -lreadline

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regras principais
all: $(LIBFT) $(NAME)

# Criação do executável principal
$(NAME): $(LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# Compilação da biblioteca minishell
$(LIB): $(OBJS)
	ar rcs $@ $^

# Criação dos arquivos objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Diretório de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilação da biblioteca libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Limpeza de arquivos objetos
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

# Limpeza total (inclui biblioteca e executável)
fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(LIB) $(NAME)

# Recompila tudo
re: fclean all

# Declarando que não são arquivos reais
.PHONY: all clean fclean re
