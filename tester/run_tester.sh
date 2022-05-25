#!/bin/bash

RED="\033[0;31m"
GRE="\033[0;38m"
CYN="\033[0;36m"
GRN="\033[0;32m"
BLU="\033[0;34m"
YEL="\033[0;33m"
RES="\033[0m"

cd ../
make

cp minishell ./tester/minishell
cd ./tester

bash test_syntax_quotes.sh 				# quotes with expansion not working yet

bash test_syntax_pipes.sh

bash test_syntax_redirs.sh


# bash test_output_cat.sh
bash test_output_various.sh




# NOT USED
# bash test_builtin_echo.sh		# SOME COLOR IS BLOCKING IT FROM GOING INTO out_mini
#bash test_unsupported_chars_not_used.sh  	# All chars are supported, without special meaning