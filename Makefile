NAME	=	minishell
BONUS	=	minishell_bonus


# added jaka: a temporary folder, only for object files
OBJ_DIR = obj_dir

LIBFT_DIR	= 	./libft
LIBFT_SRC	= 	$(LIBFT_DIR)/ft_memset.c					\
				$(LIBFT_DIR)/ft_strlen.c					\
				$(LIBFT_DIR)/ft_isalpha.c					\
				$(LIBFT_DIR)/ft_isspace.c					\
				$(LIBFT_DIR)/ft_memcpy.c					\
				$(LIBFT_DIR)/ft_memccpy.c					\
				$(LIBFT_DIR)/ft_strchr.c					\
				$(LIBFT_DIR)/ft_strncmp.c					\
				$(LIBFT_DIR)/ft_strdup.c					\
				$(LIBFT_DIR)/ft_strjoin.c					\
				$(LIBFT_DIR)/ft_strjoin_free.c				\
				$(LIBFT_DIR)/ft_strlcpy.c					\
				$(LIBFT_DIR)/ft_itoa.c						\
				$(LIBFT_DIR)/ft_split.c
LIBFT_OBJ	=	$(LIBFT_SRC:%.c=%.o)
LIBFT_A		=	$(LIBFT_DIR)/libft.a

#for common resources like utility, error handling
UTIL_DIR	=	util
UTIL_SRC	=	$(UTIL_DIR)/error.c					\
				$(UTIL_DIR)/util.c
UTIL_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(UTIL_SRC:.c=.o))

#for getting environment variables and keybound setting
INIT_DIR	=	./init
INIT_SRC	=	$(INIT_DIR)/init.c					\
				$(INIT_DIR)/env.c					\
				$(INIT_DIR)/expand.c	
INIT_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(INIT_SRC:.c=.o))


#for excute the command
EXEC_DIR	=	./exec
EXEC_SRC	=	$(EXEC_DIR)/exec.c	\
				$(EXEC_DIR)/fd.c	
EXEC_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(EXEC_SRC:.c=.o))

# EXEC_SRC	=	$(EXEC_DIR)/exec.c					\
# 				$(EXEC_DIR)/fd.c	
# EXEC_OBJ	= 	$(EXEC_SRC:%.c=%.o)
# >>>>>>> main

#for setting file information to cmd-struct from (<, <<, > , >>)
FILE_DIR		= 	./file
FILE_SRC		= 	$(FILE_DIR)/heredoc.c						
#					$(FILE_DIR)/infile.c						
# 					$(FILE_DIR)/outfile.c						
# 					$(FILE_DIR)/outfile_append.c
FILE_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(FILE_SRC:.c=.o))

# ADDED JAKA: SYNTAX ERRORS (name changed)
SYNTAX_DIR		= 	./check_syntax
SYNTAX_SRC		= 	$(SYNTAX_DIR)/check_syntax.c			\
					$(SYNTAX_DIR)/check_syntax_quotes.c		\
					$(SYNTAX_DIR)/check_syntax_pipes.c		\
					$(SYNTAX_DIR)/check_syntax_redirects.c	\
					$(SYNTAX_DIR)/check_syntax_redirects_utils.c
SYNTAX_OBJ		=	$(patsubst %, $(OBJ_DIR)/%, $(SYNTAX_SRC:.c=.o))


# ADDED JAKA: PARSING
PARSING_DIR		= 	./parsing
PARSING_SRC		= 	$(PARSING_DIR)/make_commands.c \
					$(PARSING_DIR)/make_commands_utils.c \
					$(PARSING_DIR)/free_commands_list.c \
					$(PARSING_DIR)/print_commands_info.c
PARSING_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(PARSING_SRC:.c=.o))

# For parsing from a line
# If there is(are) pipe(s), divide line
# If there is <, <<, > , >>, pass that with next word to the file command (above)
# make strings array with rest of words and complete each cmd-struct 
# PARSING_DIR	=	./parsing
# PARSING_SRC	=	$(PARSING_DIR)/parsing.c					
# 				$(PARSING_DIR)/parsing_util.c
# PARSING_OBJ	=	$(PARSING_SRC:%.c=%.0)




# ADDED JAKA: BUILTINS
BUILTINS_DIR		= 	./builtins
BUILTINS_SRC		= 	$(BUILTINS_DIR)/echo.c \
						$(BUILTINS_DIR)/pwd.c \
						$(BUILTINS_DIR)/cd.c
#		 				$(BUILTIN_DIR)/export.c				
# 						$(BUILTIN_DIR)/unset.c				
# 						$(BUILTIN_DIR)/env.c				
# 						$(BUILTIN_DIR)/exit.c
BUILTINS_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(BUILTINS_SRC:.c=.o))


# JAKA_UTILS
JAKA_UTILS_DIR		= 	./jaka_utils
JAKA_UTILS_SRC		= 	$(JAKA_UTILS_DIR)/utils.c \
						$(JAKA_UTILS_DIR)/from_libft.c
JAKA_UTILS_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(JAKA_UTILS_SRC:.c=.o))


#for main and running commands (fork and execte)
MAIN_SRC	=	testmain.c
#MAIN_SRC	=	testmain.c
MAIN_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(MAIN_SRC:.c=.o))





OBJ			=	$(UTIL_OBJ)		\
				$(INIT_OBJ)		\
				$(EXEC_OBJ)		\
				$(FILE_OBJ)		\
				$(SYNTAX_OBJ)	\
				$(PARSING_OBJ)	\
				$(BUILTINS_OBJ)	\
				$(JAKA_UTILS_OBJ)	\
				$(MAIN_OBJ)

HEADER		=	minishell.h

CFLAGS		=	-Wall -Wextra -Werror 

RLFLAG = $(shell brew --prefix readline)

# all:		$(NAME)
all:		make_obj_dir	$(NAME)  # added Jaka

### added Jaka: folders for temporary object files
make_obj_dir:
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/util/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/init/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/exec/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/file/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/check_syntax/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/parsing/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/builtins/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/jaka_utils/

libft:		$(LIBFT_A)

$(LIBFT_A):	$(LIBFT_OBJ) $(LIBFT_DIR)/libft.h
	cd $(LIBFT_DIR); make ; cd ../

# %.o:		%.c $(HEADER)
# 	gcc -c $(CFLAGS) -I$(RLFLAG)/include -o $@ $<

# duplicated Jaka
$(OBJ_DIR)/%.o:		%.c $(HEADER)
	gcc -c $(CFLAGS) -I$(RLFLAG)/include -o $@ $<


$(NAME):	$(LIBFT_A) $(OBJ) $(HEADER)
	gcc $(CFLAGS) $(OBJ) -I$(RLFLAG)/include -L$(RLFLAG)/lib -lreadline $(LIBFT_A) -o $(NAME)
# -I$(RLFLAG)/include 		# headers 	inside /Users/jmurovec/.brew/opt/readline/include/readline
# -L$(RLFLAG)/lib 			# libraries	inside /Users/jmurovec/.brew/opt/readline/lib

# if compile doesn't work, check 'brew info readline' and add 2 flags of LDFLAGS and CPPFLAGS
# for example "-L/usr/local/lib -I/usr/local/include"

clean:
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)	# added Jaka

fclean: clean
	rm -f $(NAME)

libftclean:
	cd $(LIBFT_DIR); make fclean; cd ../

re: fclean all
