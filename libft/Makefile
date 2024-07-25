SRCS            = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS			= ${SRCS:.c=.o}

NAME			= libft.a

CC				= cc
RM				= rm -f
AR				= ar rc
INCLUDES		+= -I include/

CFLAGS			= -Wall -Wextra -Werror ${INCLUDES}

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS}
			${AR} ${NAME} ${OBJS}

all:		$(NAME)

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} $(NAME)

re:			fclean all