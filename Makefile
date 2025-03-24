NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I $(INC_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft -L. -lreadline
VALGRIND = valgrind --quiet --leak-check=full --show-leak-kinds=all --suppressions=readline.supp
SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")

all: $(LIBFT) $(NAME)

$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

test: $(NAME)
	clear && cd ./tester && ./tester	

valgrind: $(NAME)
	$(VALGRIND) ./$(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean  re test valgrind
