NAME	=	minishell
BONUS	=	minishell_bonus

LIBFT_DIR	= 	./libft
LIBFT_SRC	= 	$(LIBFT_DIR)/ft_memset.c					\
				$(LIBFT_DIR)/ft_strlen.c					\
				$(LIBFT_DIR)/ft_memcpy.c					\
				$(LIBFT_DIR)/ft_memccpy.c					\
				$(LIBFT_DIR)/ft_strchr.c					\
				$(LIBFT_DIR)/ft_strncmp.c					\
				$(LIBFT_DIR)/ft_strdup.c					\
				$(LIBFT_DIR)/ft_strjoin.c					\
				$(LIBFT_DIR)/ft_strjoin_free.c				\
				$(LIBFT_DIR)/ft_strlcpy.c					\
				$(LIBFT_DIR)/ft_split.c
LIBFT_OBJ	=	$(LIBFT_SRC:%.c=%.o)
LIBFT_A		=	$(LIBFT_DIR)/libft.a

#for common resources like utility, error handling
UTIL_DIR	=	./util
UTIL_SRC	=	$(UTIL_DIR)/error.c					\
				$(UTIL_DIR)/util.c
UTIL_OBJ	= 	$(UTIL_SRC:%.c=%.o)

#for getting environment variables and keybound setting
INIT_DIR	=	./init
INIT_SRC	=	$(INIT_DIR)/init.c					\
				$(INIT_DIR)/env.c					\
				$(INIT_DIR)/expand.c	
INIT_OBJ	= 	$(INIT_SRC:%.c=%.o)

#for excute the command
EXEC_DIR	=	./exec
EXEC_SRC	=	$(EXEC_DIR)/exec.c	
EXEC_OBJ	= 	$(EXEC_SRC:%.c=%.o)

#for setting file information to cmd-struct from (<, <<, > , >>)
FILE_DIR		= 	./file
FILE_SRC		= 	$(FILE_DIR)/heredoc.c						
#					$(FILE_DIR)/infile.c						
# 					$(FILE_DIR)/outfile.c						
# 					$(FILE_DIR)/outfile_append.c
FILE_OBJ		=	$(FILE_SRC:%.c=%.o)

# For parsing from a line
# If there is(are) pipe(s), divide line
# If there is <, <<, > , >>, pass that with next word to the file command (above)
# make strings array with rest of words and complete each cmd-struct 
# PARSING_DIR	=	./parsing
# PARSING_SRC	=	$(PARSING_DIR)/parsing.c					
# 				$(PARSING_DIR)/parsing_util.c
# PARSING_OBJ	=	$(PARSING_SRC:%.c=%.0)

#for builtin commands
# BUILTIN_DIR	=	./builtin
# BUILTIN_SRC	=	$(BUILTIN_DIR)/echo.c				
# 				$(BUILTIN_DIR)/cd.c					
# 				$(BUILTIN_DIR)/pwd.c				
# 				$(BUILTIN_DIR)/export.c				
# 				$(BUILTIN_DIR)/unset.c				
# 				$(BUILTIN_DIR)/env.c				
# 				$(BUILTIN_DIR)/exit.c
# BULTIN_OBJ	=	$(BUILTIN_SRC:%.c=%.o)

#for main and running commands (fork and execte)
MAIN_SRC	=	testmain.c
MAIN_OBJ	=	$(MAIN_SRC:%.c=%.o)

OBJ			=	$(UTIL_OBJ)		\
				$(INIT_OBJ)		\
				$(EXEC_OBJ)		\
				$(FILE_OBJ)		\
				$(MAIN_OBJ)

HEADER		=	minishell.h

CFLAGS		=	-Wall -Wextra -Werror 

all:		$(NAME)

libft:		$(LIBFT_A)

$(LIBFT_A):	$(LIBFT_OBJ) $(LIBFT_DIR)/libft.h
	cd $(LIBFT_DIR); make ; cd ../

%.o:		%.c $(HEADER)
	gcc -c $(CFLAGS) -o $@ $<

$(NAME):	$(LIBFT_A) $(OBJ) $(HEADER)
	gcc $(CFLAGS) $(OBJ) -lreadline $(LIBFT_A) -o $(NAME)

# if compile doesn't work, check 'brew info readline' and add 2 flags of LDFLAGS and CPPFLAGS
# for example "-L/usr/local/lib -I/usr/local/include"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

libftclean:
	cd $(DIR_LIBFT); make fclean; cd ../

re: fclean all
