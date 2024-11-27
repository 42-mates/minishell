NAME            = minishell

OBJDIR          = obj/
INCDIR          = include/

SRCS            = src/main.c \
				  src/signals.c \
				  src/env_arr.c \
				  src/env_lst.c \
				  src/parser/lexer.c \
				  src/parser/tokenize.c \
				  src/parser/quotes.c \
				  src/parser/parser.c \
				  src/parser/path.c \
				  src/parser/extract.c \
				  src/executor.c \
				  src/builtins/exit.c \
				  src/builtins/env.c \
				  src/builtins/pwd.c \
				  src/builtins/echo.c \
				  src/builtins/export.c \
				  src/builtins/unset.c \
				  src/error.c \
				  src/free.c \
				  src/utils.c \
				  src/debug.c

OBJS            = $(SRCS:src/%.c=$(OBJDIR)%.o)

LIBFT_DIR       = ./libft
LIBFT           = $(LIBFT_DIR)/libft.a

CC              = cc
CFLAGS          = -Wall -Wextra -Werror -I $(INCDIR) -I $(LIBFT_DIR)/include
LDFLAGS         = -lreadline
RM              = rm -f

all:		$(NAME)

$(NAME): 	$(LIBFT) $(OBJS)
			$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

$(OBJDIR)%.o: src/%.c
			@mkdir -p $(dir $@)
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