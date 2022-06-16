#!/bin/bash

RED="\033[0;31m"
CYN="\033[0;36m"
GRN="\033[0;32m"
BLU="\033[0;34m"
YEL="\033[0;33m"
GRE="\033[0;38m"
RES="\033[0m"



############################################################

error_message="Minishell: Syntax error"

############################################################


#make

test_syntax_error()
{
	filename="out_temp"
	while read -r line; do
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


echo -e $YEL"\nTest syntax error: QUOTES"$RES

 inputlines=(
	 		'abc"efg'
	 		'abc"""""efg'
	 		'abc"e"fg   "'
	 		 "abc'efg"
	 		"abc'''efg"
	 		"abc'e'f'g"
	 		"  ' abc efg"
	 		"  abc '  efg "
	 		"  abc ' ' '  efg "
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]}
	printf "  Test %3d:   %-30s   " $i "[$input]"
	> out_temp; >out_mini; > out_orig
	./minishell "$input" | cat -e > out_temp
	echo "Is this printing?"
	#./minishell "$input" | 2> out_temp0 | cat -e out_temp0 > out_temp
	echo $error_message | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "error"
	((i=i+1))
done




##############################################################################


echo -e $YEL"\nTest QUOTES: valid input: Should print error into stderr: Command not found"$RES
echo -e $YEL"\n ( Not handled yet )"$RES

 inputlines=(
	 		'cat in""file'
	 		# 'abc""efg'

	 		# 'ca""t infile'
			'ca""t infile'

	 		# 'abc "" efg'
			'cat "" infile'		# prints both stderror and output content into outfile

	 		# 'abc " " efg'

	 		# 'abc"x"efg'
			'cat in"f"ile'


	 		# 'abc"d"e"f"g'
			)

nr_elements=${#inputlines[@]}
# TURN ON/OFF 
#nr_elements=0


i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]}
	printf "  Test %3d:   %-30s   " $i "'$input'"
	> out_temp; >out_mini; > out_orig
	./minishell "$input" | cat -e > out_temp
	#echo "syntax error from initial check: pipes; exit" | cat -e > out_orig

	## eval "$input" | cat -e 2> out_orig ==> CAT get's no input after pipe
	eval "$input" > out_orig2
	cat -e out_orig2 > out_orig
	test_syntax_error "out_orig" "out_mini" "valid"
	((i=i+1))
done


echo ""


##################################################################################



