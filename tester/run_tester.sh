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


bash test_syntax_quotes.sh

bash test_syntax_pipes.sh

bash test_output_cat.sh


# bash test_syntax_redirs.sh		## some issues with output, neeed to check

# bash test_output_various.sh		## check what is here

bash test_output_echo.sh



# NOT USED
# bash test_builtin_echo.sh		# SOME COLOR IS BLOCKING IT FROM GOING INTO out_mini
