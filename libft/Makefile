NAME		=	libft.a

CC			=	cc -Wall -Wextra -Werror

INCDIR		=	include/
SRCS		=	src/check/ft_isalnum.c \
				src/check/ft_isalpha.c \
				src/check/ft_isascii.c \
				src/check/ft_isdigit.c \
				src/check/ft_isprint.c \
				src/check/ft_isspace.c \
				src/check/ft_strcmp.c \
				src/convert/ft_atoi.c \
				src/convert/ft_itoa.c \
				src/convert/ft_tolower.c \
				src/convert/ft_toupper.c \
				src/ft_printf/ft_print_unsigned.c \
				src/ft_printf/ft_printchar.c \
				src/ft_printf/ft_printhex.c \
				src/ft_printf/ft_printnum.c \
				src/ft_printf/ft_printptr.c \
				src/ft_printf/ft_printstr.c \
				src/ft_printf/ft_printf.c \
				src/get_next_line/get_next_line_bonus.c \
				src/list/ft_lstadd_back.c \
				src/list/ft_lstadd_front.c \
				src/list/ft_lstclear.c \
				src/list/ft_lstdelone.c \
				src/list/ft_lstiter.c \
				src/list/ft_lstlast.c \
				src/list/ft_lstmap.c \
				src/list/ft_lstnew.c \
				src/list/ft_lstsize.c \
				src/mem/ft_bzero.c \
				src/mem/ft_calloc.c \
				src/mem/ft_memchr.c \
				src/mem/ft_memcmp.c \
				src/mem/ft_memcpy.c \
				src/mem/ft_memmove.c \
				src/mem/ft_memset.c \
				src/put/ft_putchar_fd.c \
				src/put/ft_putendl_fd.c \
				src/put/ft_putnbr_fd.c \
				src/put/ft_putstr_fd.c \
				src/str/ft_split.c \
				src/str/ft_strchr.c \
				src/str/ft_strdup.c \
				src/str/ft_striteri.c \
				src/str/ft_strjoin.c \
				src/str/ft_strlcat.c \
				src/str/ft_strlcpy.c \
				src/str/ft_strlen.c \
				src/str/ft_strmapi.c \
				src/str/ft_strncmp.c \
				src/str/ft_strnstr.c \
				src/str/ft_strrchr.c \
				src/str/ft_strtok.c \
				src/str/ft_strtrim.c \
				src/str/ft_substr.c
		
OBJS		=	$(SRCS:.c=.o)

RM			=	rm -f

all:		$(NAME)

$(NAME):	$(OBJS) $(INCDIR)libft.h
			ar rcs $@ $^

%.o: %.c	$(INCDIR)libft.h
			$(CC) -I$(INCDIR) -c $< -o $@

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all


.PHONY: all clean fclean re