#!/bin/bash

RED="\033[0;31m"
CYN="\033[0;36m"
GRN="\033[0;32m"
BLU="\033[0;34m"
YEL="\033[0;33m"
GRE="\033[0;38m"
RES="\033[0m"



############################################################

# error_message="Minishell: Syntax error: found char unsupported by Minishell"

############################################################


#make

test_syntax_error()
{
	filename="out_orig"
	while read -r line; do
		# if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		then
			echo $line >> out_orig2
		else
			: echo $line >> out_else
		fi
	done < "$filename"


	filename="out_temp"
	while read -r line; do
		# if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
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


echo -e $YEL"\nTest output CAT"$RES

 inputlines=(
	 		"cat infile"
	 		"cat -n infile"
	 		"cat -e infile"
	 		"cat -e -n infile"
	 		"cat -en infile"
			"cat infile > out1"
			"cat infile > out1 > out2"

			# "env"	# first few lines are different, random string
						)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]}
	printf "  Test %3d:   %-30s   " $i "'$input'"
	> out_temp; >out_mini; > out_orig; > out_orig2 
	./minishell "$input" | cat -e > out_temp
	eval $input | cat -e > out_orig
	test_syntax_error "out_orig2" "out_mini" "valid"
	((i=i+1))
done



echo ""


##################################################################################






