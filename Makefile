SRCS 			= $(wildcard src/*/*.c) $(wildcard src/*.c)

OBJS			= ${SRCS:.c=.o}

NAME			= minishell

LIBFT_DIR		= ./libft
LIBFT			= ./libft/libft.a

CC				= cc
RM				= rm -f
AR				= ar rc
CP				= cp

INCLUDES		+= -I libft/include
INCLUDES		+= -I ./include
CFLAGS			= -Wall -Wextra -Werror $(INCLUDES)

$(NAME):	$(LIBFT) ${OBJS}
			${CC} ${CFLAGS} ${OBJS} $(LIBFT) -o ${NAME}

$(LIBFT):	$(LIBFT_DIR)
				@$(MAKE) -C $(LIBFT_DIR)

all:		$(NAME)

clean:
			${RM} ${OBJS}
			@$(MAKE) -C $(LIBFT_DIR) clean

fclean:		clean
			${RM} $(NAME)
			@$(MAKE) -C $(LIBFT_DIR) fclean

re:			fclean all