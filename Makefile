NAME	=	minishell
#BONUS	=	minishell_bonus

LIBFT_DIR	= 	./libft
LIBFT_SRC	= 	$(LIBFT_DIR)/ft_memset.c					\
				$(LIBFT_DIR)/ft_strlen.c					\
				$(LIBFT_DIR)/ft_memcpy.c					\
				$(LIBFT_DIR)/ft_memccpy.c					\
				$(LIBFT_DIR)/ft_strncmp.c					\
				$(LIBFT_DIR)/ft_strdup.c
LIBFT_OBJ	=	$(LIBFT_SRC:%.c=%.o)
LIBFT_A		=	$(LIBFT_DIR)/libft.a

#for common resources like utility, error handling
UTIL_DIR	=	./util
UTIL_SRC	=	$(PARSING_DIR)/error.c					\
				$(PARSING_DIR)/util.c
UTIL_OBJ	= 	$(UTIL_SRC:%.c=%.o)

#for getting environment variables and keybound setting
INIT_DIR	=	./init
INIT_SRC	=	$(PARSING_DIR)/init.c					\
				$(PARSING_DIR)/init_util.c
INIT_OBJ	= 	$(INIT_SRC:%.c=%.o)

#for expanding $---
EXPAND_DIR		=	./expand
EXPAND_SRC		=	$(EXPAND_DIR)/expand.c
EXPAND_OBJ		=	$(EXPAND_DIR:%c=%.o)

#for setting file information to cmd-struct from (<, <<, > , >>)
FILE_DIR		= 	./file
FILE_SRC		= 	$(FILE_DIR)/infile.c						\
					$(FILE_DIR)/heredoc.c						\
					$(FILE_DIR)/outfile.c						\
					$(FILE_DIR)/outfile_append.c
FILE_OBJ		=	$(FILE_SRC:%.c=%.o)

# For parsing from a line
# If there is(are) pipe(s), divide line
# If there is <, <<, > , >>, pass that with next word to the file command (above)
# make strings array with rest of words and complete each cmd-struct 
PARSING_DIR	=	./parsing
PARSING_SRC	=	$(PARSING_DIR)/parsing.c					\
				$(PARSING_DIR)/parsing_util.c
PARSING_OBJ	=	$(PARSING_SRC:%.c=%.0)

#for builtin commands
BUILTIN_DIR	=	./builtin
BUILTIN_SRC	=	$(BUILTIN_DIR)/echo.c				\
				$(BUILTIN_DIR)/cd.c					\
				$(BUILTIN_DIR)/pwd.c				\
				$(BUILTIN_DIR)/export.c				\
				$(BUILTIN_DIR)/unset.c				\
				$(BUILTIN_DIR)/env.c				\
				$(BUILTIN_DIR)/exit.c
BULTIN_OBJ	=	$(BUILTIN_SRC:%.c=%.o)

#for main and running commands (fork and execte)
MAIN_SRC	= 	main.c
MAIN_OBJ	=	$(MAIN_SRC:%.c=%.o)

OBJ			=	$(UTIL_OBJ)		\
				$(INIT_OBJ)		\
				$(EXPAND_OBJ) 	\
				$(FILE_OBJ)		\
				$(PARSING_OBJ)	\
				$(BULTIN_OBJ)	\
				$(MAIN_OBJ)

HEADER		=	minishell.h

CFLAGS		=	-Wall -Wetra -Werror -L/usr/local/lib -I/usr/local/include -lreadline

all:		$(NAME)

libft:		$(LIBFT_A)

$(LIBFT_A):	$(LIBFT_OBJ) $(LIBFT_DIR)/libft.h
	cd $(LIBFT_DIR); make ; cd ../

%.o:		%.c $(HEADER)
	gcc -c $(CFLAGS) -o $@ $<

$(NAME):	$(LIBFT_A) $(OBJ) $(HEADER)
	GCC $(CFLAGS) $(OBJ) $(LIBFT_A) -O $(NAME)

