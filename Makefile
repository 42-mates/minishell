NAME			= minishell

OBJDIR			= obj/
INCDIR			= include/

SRCS			= src/main.c \
				  src/parser/lexer.c \
				  src/parser/quotes.c \
				  src/parser/tokenize.c \
				  src/parser/parser.c \
				  src/parser/extract.c \
				  src/parser/syntax.c \
				  src/parser/utils_parser.c \
				  src/executor/exec.c \
				  src/executor/path.c \
				  src/executor/builtin.c \
				  src/executor/pipes.c \
				  src/executor/redirect.c \
				  src/executor/heredoc.c \
				  src/executor/heredoc_file.c \
				  src/executor/utils_exec.c \
				  src/builtins/exit.c \
				  src/builtins/env.c \
				  src/builtins/pwd.c \
				  src/builtins/echo.c \
				  src/builtins/export.c \
				  src/builtins/unset.c \
				  src/builtins/cd.c \
				  src/utils/init.c \
				  src/utils/signals.c \
				  src/utils/env.c \
				  src/utils/error.c \
				  src/utils/free.c \
				  src/utils/utils_env.c \
				  src/utils/utils_string.c \
				  src/utils/debug.c

OBJS			= $(SRCS:src/%.c=$(OBJDIR)%.o)

LIBFT_DIR		= ./libft
LIBFT			= $(LIBFT_DIR)/libft.a

CC				= cc
INCLUDES		= -I $(INCDIR) -I $(LIBFT_DIR)/include
CFLAGS			= -Wall -Wextra -Werror
LDFLAGS			= -lreadline
RM				= rm -f

all:			$(NAME)

$(NAME): 		$(LIBFT) $(OBJS)
				$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

$(OBJDIR)%.o:	src/%.c $(INCDIR)/minishell.h
				@mkdir -p $(dir $@)
				$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
				@$(MAKE) -C $(LIBFT_DIR)

clean:
				rm -rf $(OBJDIR)
				@$(MAKE) -C $(LIBFT_DIR) clean

fclean:
				$(RM) $(NAME)
				rm -rf $(OBJDIR)
				@$(MAKE) -C $(LIBFT_DIR) fclean

re: 			fclean all

.PHONY: 		all clean fclean re