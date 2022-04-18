#!/bin/bash

RED="\033[0;31m"
GRE="\033[0;38m"
CYN="\033[0;36m"
GRN="\033[0;32m"
BLU="\033[0;34m"
YEL="\033[0;33m"
RES="\033[0m"

############################################################

error_message="Minishell: Syntax error with REDIRECTS"

############################################################

## TO EXTRACT LINES , TEMP
# print_input_lines()
# {

# 	while read -r line; do
# 		if [[ $line == input=* ]] && [[ $line != $ ]]   ;
# 		then
# 			echo $line >> out_mini
# 			#msg=$line

# 		else
# 			: #echo $line >> out_mini
# 		fi
# 	done < "x_tester_synterr_redirs_in.sh"

# }

# print_input_lines


test_syntax_error()
{
	filename="out_temp"
	while read -r line; do
		if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		then
			echo $line >> out_mini
		else
			: #echo $line >> out_mini
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


### SYNTAX ERRORS ############################################
echo -e $YEL"\nTest INPUT redirects: syntax errors"$RES

inputlines=("<"
			"<<"
			"<<<"
			"< <<"
			"<< <"
			"< <"
			" <"
			" <<"
			" < <"
			" < < "
			"<< >"
			">< <"
			" << < "
			" < << "
			"< abc < < "
			"< abc <<"
			"<< abc < < abc"
			"< abc < abc <<"
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> out_temp; >out_mini; > out_orig
	./minishell "$input" | cat -e > out_temp
	echo $error_message | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "error" 
	((i=i+1))
done




### SYNTAX ERRORS, ARROW WITH PIPE #######################################################

echo -e $YEL"\nTest INPUT redirects: syntax errors, arrow with pipe"$RES


inputlines=(
			"ls | < | outfile"
			"< infile cat | < | outfile"
			"<< here >< outfile"
			"<< here cat | wc >< outfile"
			"<> here cat | wc > outfile"
			"<< here > < outfile"
			"<< here >> < outfile"
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> out_temp; >out_mini; > out_orig
	./minishell "$input" | cat -e > out_temp
	echo $error_message | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "error" 
	((i=i+1))
done





#### VALID INPUT ###################################################################

echo -e $YEL"\nTest redirects: valid input "$RES

inputlines=(
			"< abc"
			"< abc < abc"
			"<abc"
			"<<abc"
			"<< abc"
			"<< abc << abc"
			"<< abc < abc"
			"<< here cat | wc > outfile"
			"<< here cat | wc >> outfile"
	 		)


nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> out_temp; >out_mini; > out_orig
	./minishell "$input" | cat -e > out_temp
	#echo "syntax error from initial check: redirections; exit" | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid" 
	((i=i+1))
done


############################################################

echo -e $YEL"\nTest redirects: pipes with arrows, valid input "$RES

inputlines=(
			"ls | < infile"
			"ls|<infile"
			"< infile cat -e > out"
			#"< infile cat -e | < ls | < infile"
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> out_temp; >out_mini; > out_orig
	./minishell "$input" | cat -e > out_temp
	#echo "syntax error from initial check: redirections; exit" | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid" 
	((i=i+1))
done

echo ""
