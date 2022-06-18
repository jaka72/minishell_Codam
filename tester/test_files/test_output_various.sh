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
OUT="output_files"
> $OUT/out_various_examples_all.txt
j=0

test_syntax_error()
{
	echo "Test $j" >> $OUT/out_various_examples_all.txt

	filename=$OUT/"out_orig"
	while read -r line; do
		# if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		then
			echo $line >> $OUT/out_orig2
		else
			: echo $line >> $OUT/out_else
		fi
	done < "$filename"


	filename=$OUT/"out_temp"
	while read -r line; do
		# if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		then
			echo $line >> $OUT/out_mini
			echo $line >> $OUT/out_various_examples_all.txt
		else
			: echo $line >> $OUT/out_else
		fi
	done < "$filename"
	msg=$3
#	DIFF=$(diff $1 $2)
	DIFF=$(diff <(head -n 10 $OUT/$1)  <(head -n 10 $OUT/$2))
	if [ "$DIFF" == "" ] 
	then
		echo -e $GRN"[ OK ] " $GRE $msg $RES 
	else
		echo -e $RED"[ KO ]"$RES 
	fi
	echo "" >> $OUT/out_various_examples_all.txt
	((j=j+1))
}


#############################################################################


echo -e $YEL"\nTest output VARIOUS EXAMPLES"$RES

 inputlines=(
			"env"	# JUST CHECKS FIRST 10 LINES OF OUTPUT
			"ls | head -n 10 | tail -n 9 | rev | sort | rev | cat -e"
			"ls|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e"
			"< infile grep 'a' | grep 'b' | wc -l | cat -e"

			# 'cat "aaa > > bbb"'		# should be error, but message does not match with orig
			# 'cat "aaa < < < bbb"'
			)

nr_elements=${#inputlines[@]}
i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]}
	printf "  Test %3d:   %-30s   " $i "[$input]"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig; > $OUT/out_orig2 
	./minishell "$input" | cat -e > $OUT/out_temp
	eval $input | cat -e > $OUT/out_orig
	test_syntax_error "out_orig2" "out_mini" "valid"
	((i=i+1))
done
echo ""


##################################################################################






