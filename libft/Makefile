# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: kito <kito@student.codam.nl>                 +#+                      #
#                                                    +#+                       #
#    Created: 2020/12/01 17:08:43 by kito          #+#    #+#                  #
#    Updated: 2022/04/14 16:35:47 by kito          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=	libft.a
SRC			=	ft_memset.c			\
				ft_strlen.c			\
				ft_isalpha.c		\
				ft_isspace.c		\
				ft_memcpy.c			\
				ft_memccpy.c		\
				ft_strchr.c			\
				ft_strncmp.c		\
				ft_strdup.c			\
				ft_strjoin.c		\
				ft_strjoin_free.c	\
				ft_strlcpy.c		\
				ft_split.c

CFLAGS		=	-Wall -Wextra -Werror

OBJ	=	$(SRC:%.c=%.o)

HEADER_FILE	=	libft.h

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)

%.o: %.c libft.h
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) 

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
