NAME            = minishell

OBJDIR          = obj/
INCDIR          = include/

SRCS            = src/minishell.c \
                  src/executor.c \
                  src/lexer.c \
                  src/parser.c

OBJS            = $(SRCS:src/%.c=$(OBJDIR)%.o)

LIBFT_DIR       = ./libft
LIBFT           = $(LIBFT_DIR)/libft.a

CC              = cc
CFLAGS          = -Wall -Wextra -Werror -I $(INCDIR) -I $(LIBFT_DIR)/include
RM              = rm -f

all:		$(NAME)

$(NAME): 	$(LIBFT) $(OBJS)
			$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJDIR)%.o: src/%.c
			@mkdir -p $(OBJDIR)
			$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
			@$(MAKE) -C $(LIBFT_DIR)

clean:
			$(RM) $(OBJS)
			@$(MAKE) -C $(LIBFT_DIR) clean

fclean: 	clean
			$(RM) $(NAME)
			@$(MAKE) -C $(LIBFT_DIR) fclean
			rm -rf $(OBJDIR)

re: 		fclean all

.PHONY: 	all clean fclean re