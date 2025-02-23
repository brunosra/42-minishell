# Nome do executável
NAME = minishell

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft

# Nome da biblioteca
LIBFT = $(LIBFT_DIR)/libft.a

# Compilador e flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -I $(INC_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft -L. -lreadline

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/builtins/*.c)

# Regras principais
all: $(LIBFT) $(NAME)

# Criação do executável principal
$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) $(LDFLAGS) -o $(NAME)

# Compilação da biblioteca libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Limpeza de arquivos objetos
clean:
	$(MAKE) -C $(LIBFT_DIR) clean

# Limpeza total (inclui biblioteca e executável)
fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(LIB) $(NAME)

test:
	cd ./tester && ./tester

# Recompila tudo
re: fclean all

# Declarando que não são arquivos reais
.PHONY: all clean fclean re tester
