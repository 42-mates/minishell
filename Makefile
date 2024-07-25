NAME    = minishell

SRCDIR  = src
INCDIR  = include

# Find all .c files in the src directory and its subdirectories
SRC     = $(shell find $(SRCDIR) -name '*.c')
OBJS    = $(SRC:.c=.o)

LIBFT = libft/libft.a
LIBFT_PATH = libft/
LIBFT_INCLUDE = libft/include

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
RM      = rm -f

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -o $(NAME)

# COLORS
GREEN = \033[1;38;5;76m
RESET = \033[0m

$(LIBFT):
	@echo "$(GREEN)Compiling libft..."
	@make -sC $(LIBFT_PATH)
	@echo "Libft compiled!$(RESET)"

%.o: %.c $(INCDIR)/minishell.h
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFT_INCLUDE) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(LIBFT)

re: fclean all

.PHONY: all clean fclean re
