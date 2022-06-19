NAME	=	minishell
BONUS	=	minishell_bonus

# for temporary folder, only for object files
OBJ_DIR = obj_dir

LIBFT_DIR	= 	./libft
# LIBFT_SRC	= 	$(LIBFT_DIR)/ft_memset.c					\
# 				$(LIBFT_DIR)/ft_strlen.c					\
# 				$(LIBFT_DIR)/ft_isalpha.c					\
# 				$(LIBFT_DIR)/ft_isspace.c					\
# 				$(LIBFT_DIR)/ft_memcpy.c					\
# 				$(LIBFT_DIR)/ft_memccpy.c					\
# 				$(LIBFT_DIR)/ft_strchr.c					\
# 				$(LIBFT_DIR)/ft_strncmp.c					\
# 				$(LIBFT_DIR)/ft_strdup.c					\
# 				$(LIBFT_DIR)/ft_strjoin.c					\
# 				$(LIBFT_DIR)/ft_strjoin_free.c				\
# 				$(LIBFT_DIR)/ft_strlcpy.c					\
# 				$(LIBFT_DIR)/ft_atoi.c						\
# 				$(LIBFT_DIR)/ft_itoa.c						\
# 				$(LIBFT_DIR)/ft_split.c						\
# 				$(LIBFT_DIR)/ft_add_str.c
# LIBFT_OBJ	=	$(LIBFT_SRC:%.c=%.o)
LIBFT_A		=	$(LIBFT_DIR)/libft.a

#for common resources like utility, error handling
UTIL_DIR	=	util
UTIL_SRC	=	$(UTIL_DIR)/error.c					\
				$(UTIL_DIR)/free.c					\
				$(UTIL_DIR)/util.c
UTIL_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(UTIL_SRC:.c=.o))

#for getting environment variables and keybound setting
INIT_DIR	=	./init
INIT_SRC	=	$(INIT_DIR)/init.c					\
				$(INIT_DIR)/signal_handle.c			\
				$(INIT_DIR)/env_util.c				\
				$(INIT_DIR)/env.c					\
				$(INIT_DIR)/expand_util_1.c			\
				$(INIT_DIR)/expand_util_2.c			\
				$(INIT_DIR)/expand.c
INIT_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(INIT_SRC:.c=.o))

#for excute the command
EXEC_DIR	=	./exec
EXEC_SRC	=	$(EXEC_DIR)/exec.c					\
				$(EXEC_DIR)/open_fd.c				\
				$(EXEC_DIR)/connect_fd.c			\
				$(EXEC_DIR)/find_path.c				\
				$(EXEC_DIR)/check_files.c			\
				$(EXEC_DIR)/ms_execve.c						# added jaka 10 jun
EXEC_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(EXEC_SRC:.c=.o))

#for setting file information to cmd-struct from (<, <<, > , >>)
FILE_DIR		= 	./heredoc
FILE_SRC		= 	$(FILE_DIR)/heredoc.c			\
					$(FILE_DIR)/heredoc_util.c
FILE_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(FILE_SRC:.c=.o))

# for SYNTAX ERRORS 
SYNTAX_DIR		= 	./check_syntax
SYNTAX_SRC		= 	$(SYNTAX_DIR)/check_syntax.c			\
					$(SYNTAX_DIR)/check_syntax_quotes.c		\
					$(SYNTAX_DIR)/check_syntax_pipes.c		\
					$(SYNTAX_DIR)/check_syntax_redirects.c	\
					$(SYNTAX_DIR)/check_syntax_redirects_utils.c
SYNTAX_OBJ		=	$(patsubst %, $(OBJ_DIR)/%, $(SYNTAX_SRC:.c=.o))


# for PARSING
PARSING_DIR		= 	./parsing
PARSING_SRC		= 	$(PARSING_DIR)/make_commands.c 				\
					$(PARSING_DIR)/make_commands_utils.c 		\
					$(PARSING_DIR)/choose_correct_array.c 		\
					$(PARSING_DIR)/store_to_redirect_arr.c 		\
					$(PARSING_DIR)/ft_lstadd_back.c \
					$(PARSING_DIR)/print_commands_info.c		# can be later removed
PARSING_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(PARSING_SRC:.c=.o))

# for BUILTINS
BUILTINS_DIR		= 	./builtins
BUILTINS_SRC		= 	$(BUILTINS_DIR)/echo.c 		\
						$(BUILTINS_DIR)/pwd.c 		\
						$(BUILTINS_DIR)/cd.c 		\
						$(BUILTINS_DIR)/cd_utils.c 	\
						$(BUILTINS_DIR)/exit.c 		\
		 				$(BUILTINS_DIR)/export.c	\
						$(BUILTINS_DIR)/env.c		\
						$(BUILTINS_DIR)/unset.c	 
BUILTINS_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(BUILTINS_SRC:.c=.o))


# JAKA_UTILS
JAKA_UTILS_DIR		= 	./jaka_utils
JAKA_UTILS_SRC		= 	$(JAKA_UTILS_DIR)/utils.c	\
						$(JAKA_UTILS_DIR)/print_err_msg.c
#						$(JAKA_UTILS_DIR)/from_libft.c
JAKA_UTILS_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(JAKA_UTILS_SRC:.c=.o))


#for main and running commands (fork and execte)
#MAIN_SRC	=	testmain_jaka.c
MAIN_SRC	=	main.c
MAIN_OBJ	=	$(patsubst %, $(OBJ_DIR)/%, $(MAIN_SRC:.c=.o))

OBJ			=	$(UTIL_OBJ)			\
				$(INIT_OBJ)			\
				$(EXEC_OBJ)			\
				$(FILE_OBJ)			\
				$(SYNTAX_OBJ)		\
				$(PARSING_OBJ)		\
				$(BUILTINS_OBJ)		\
				$(JAKA_UTILS_OBJ)	\
				$(MAIN_OBJ)

HEADER		=	minishell.h

CFLAGS		=	-Wall -Wextra -Werror 

RLFLAG = $(shell brew --prefix readline)

# all:		$(NAME)
all:		make_obj_dir	$(NAME)  # added Jaka

### for folders for temporary object files
make_obj_dir:
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/util/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/init/ 
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/exec/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/heredoc/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/check_syntax/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/parsing/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/builtins/
	@mkdir -p $(OBJ_DIR)/ $(OBJ_DIR)/jaka_utils/

libft:		$(LIBFT_A)

$(LIBFT_A):	$(LIBFT_OBJ) $(LIBFT_DIR)/libft.h
	cd $(LIBFT_DIR); make ; cd ../

# %.o:		%.c $(HEADER)
# 	gcc -c $(CFLAGS) -I$(RLFLAG)/include -o $@ $<

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
	rm -rf $(OBJ_DIR)

fclean: libftclean clean
	rm -f $(NAME)

libftclean:
	cd $(LIBFT_DIR); make fclean; cd ../

re: fclean all