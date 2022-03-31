# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: kito <kito@student.codam.nl>                 +#+                      #
#                                                    +#+                       #
#    Created: 2020/12/01 17:08:43 by kito          #+#    #+#                  #
#    Updated: 2022/03/31 18:04:18 by kito          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=	libft.a
SRC			=	ft_memset.c		\
				ft_strlen.c		\
				ft_memcpy.c		\
				ft_memccpy.c	\
				ft_strncmp.c	\
				ft_strdup.c

CFLAGS		=	-Wall -Wextra -Werror

REGUL_OBJ	=	$(SRC:%.c=%.o)

HEADER_FILE	=	libft.h

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)

%.o: %.c libft.h
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -rf $(REGUL_OBJ) 

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
