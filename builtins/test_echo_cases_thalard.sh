#!/bin/bash

RED="\033[0;31m"
CYN="\033[0;36m"
GRN="\033[0;32m"
BLU="\033[0;34m"
YEL="\033[0;33m"
GRE="\033[0;38m"
RES="\033[0m"

cd ../
make

cp minishell ./builtins/minishell
cd ./builtins



############################################################

error_message="Minishell: builtins: echo error"

############################################################


#make

test_syntax_error()
{
	filename="out_temp"
	while read -r line; do
		if [[ $line != ^[[* ]] && [[ $line != $ ]] && [[ $line != exit* ]]  ;
		then
			echo $line >> out_mini
		else
			: echo $line >> out_else
		fi
	done < "$filename"
	msg=$3
	DIFF=$(diff $1 $2)
	if [ "$DIFF" == "" ] 
	then
		echo -e $GRN"[ OK ] " $GRE $msg $RES 
	else
		echo -e $RED"[ KO ]"$RES 
	fi
}


#############################################################################


echo -e $YEL"\nTest builtin ECHO"$RES

 inputlines=(
            #"echo hello"
            # "echo echo"
            # "echo rhobebou"
            # "echo stop barking"
            # 'echo "bonjour"'
            # "echo bonjour"
            # "echo 'bonjour'"
			)

# nr_elements=${#inputlines[@]}

# i=0
# while (( $i < $nr_elements ))
# do
# 	input=${inputlines[$i]}
# 	printf "  Test %3d:   %-30s   " $i "'$input'"
# 	> out_temp; >out_mini; > out_orig
# 	./minishell "$input" | cat -e > out_temp
# 	echo $error_message | cat -e > out_orig
# 	test_syntax_error "out_orig" "out_mini" "error"
# 	((i=i+1))
# done





##############################################################################


echo -e $YEL"\nTest valid input"$RES

  inputlines=(
            "hello"
            "echo"
            "rhobebou"
            "stop barking"
            "bonjour"
            # "bonjour"
            # "'bonjour'"  			# not possible in this tester 
            # ""bonjour""			# not possible in this tester 

			# "-n bonjour"			# not possible in this tester 
			# "-nn bonjour"			# not possible in this tester 
			# "-n -n -n bonjour"	# not possible in this tester 
			# "-n -n -n bonjour"	# not possible in this tester 
			# '"-n" bonjour'		# not possible in this tester 
			# '"-nnnn" bonjour'		# not possible in this tester 
 			)

nr_elements=${#inputlines[@]}


i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]}
	printf "  Test %3d:   %-30s   " $i "[$input]"
	> out_temp; >out_mini; > out_orig
	./minishell "echo $input" | cat -e > out_temp
	#echo "syntax error from initial check: pipes; exit" | cat -e > out_orig
	#echo $input | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid"
	((i=i+1))
done


echo ""


##################################################################################






