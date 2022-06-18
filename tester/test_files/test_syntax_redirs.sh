#!/bin/bash

RED="\033[0;31m"
GRE="\033[0;38m"
CYN="\033[0;36m"
GRN="\033[0;32m"
BLU="\033[0;34m"
YEL="\033[0;33m"
RES="\033[0m"

############################################################

# error_message="Minishell: Syntax error with REDIRECTS"
error_message="Minishell: Syntax error"

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

OUT="output_files"							# folder for all output files
> $OUT/out_redirs_stxerr_all.txt			# wipe content before start

test_syntax_error()
{
	filename=$OUT/"out_temp"
	while read -r line; do
		if [[ $line != ^[[* ]] && [[ $line != $ ]]   ;
		then
			echo $line >> $OUT/out_mini
			echo $line >> $OUT/out_redirs_stxerr_all.txt
		else
			: #echo $line >> out_mini
		fi
	done < "$filename"
	msg=$3
	DIFF=$(diff $OUT/$1 $OUT/$2)
	if [ "$DIFF" == "" ] 
	then
		echo -e $GRN"[ OK ] " $GRE $msg $RES
	else
		echo -e $RED"[ KO ]"$RES 
	fi
}


### SYNTAX ERRORS ############################################
echo -e $YEL"\nTest INPUT redirects: syntax errors"$RES

inputlines=(
			"<"
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
	> $OUT/out_temp; >$OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp
	echo $error_message | cat -e > $OUT/out_orig
	test_syntax_error "out_orig" "out_mini" "error" 
	((i=i+1))
done



### SYNTAX ERRORS, ARROW WITH PIPE #######################################################

echo -e $YEL"\nTest INPUT redirects: syntax errors, arrow with pipe"$RES


inputlines=(
			"ls | < | outfile"
			"< infile cat | < | outfile"
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp
	echo $error_message | cat -e > $OUT/out_orig
	test_syntax_error "out_orig" "out_mini" "error" 
	((i=i+1))
done

### SYNTAX ERRORS,HEREDOC #######################################################

echo -e $YEL"\nREDIRECTIONS: Syntax errors with heredoc"$RES
echo -e $YEL"              Test not working yet yet"$RES


inputlines=(
			## HEREDOC not handled yet
			# "<< here >< outfile"
			# "<< here cat | wc >< outfile"
			# "<> here cat | wc > outfile"
			# "<< here > < outfile"
			# "<< here >> < outfile"
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp
	echo $error_message | cat -e > $OUT/out_orig
	test_syntax_error "out_orig" "out_mini" "error" 
	((i=i+1))
done




#### VALID INPUT ###################################################################

echo -e $YEL"\nREDIRECTIONS: valid input "$RES
#SOMETHING IS NOT CORRECT HERE 

inputlines=(
			"< infile"
			"< infile < infile"
			"<infile"
			"< infile"
	 		)


nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp
#	echo $input | cat -e > $OUT/out_orig # WHY ECHO ???
	eval $input | cat -e 2> $OUT/out_orig	# EVAL ???
	#echo "syntax error from initial check: redirections; exit" | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid" 
	((i=i+1))
done


############################################################

echo -e $YEL"\nREDIRECTIONS: pipes with arrows, valid input "$RES

inputlines=(
			"ls | < infile"
			"ls|<infile"
			"< infile cat -e > out"
			"< infile cat -e | < infile | < infile"
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp

	#echo "$input" | cat -e > out_orig
	eval $input | cat -e > $OUT/out_orig
	#echo "syntax error from initial check: redirections; exit" | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid" 
	((i=i+1))
done

rm out
echo ""




#### VALID INPUT, HEREDOC ###################################################################

echo -e $YEL"\nREDIRECTIONS: valid input with HEREDOC "$RES
echo -e $YEL"              Test not working yet"$RES

#SOMETHING IS NOT CORRECT HERE 

inputlines=(
			### HEREDOC not handled yet
			# "<<abc"
			# "<< abc"
			# "<< abc << abc"
			# "<< abc < abc"
			# "<< here cat | wc > outfile"
			# "<< here cat | wc >> outfile"
	 		)


nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-40s   " $i  "'$input'"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp
#	echo $input | cat -e > $OUT/out_orig # WHY ECHO ???
	eval $input | cat -e 2> $OUT/out_orig	# EVAL ???
	#echo "syntax error from initial check: redirections; exit" | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid" 
	((i=i+1))
done




############################################################

echo -e $YEL"\nREDIRECTIONS: pipes with arrows, Error message: No such file or directory "$RES

inputlines=(
			"ls | < xxxyyy"
			"ls|<xxxyyy"
			"< xxxyyy cat -e > out"
			"< xxxyyy cat -e | < infile | < infile"
			"< infile cat -e | < ls | < infile"	
			)

nr_elements=${#inputlines[@]}

i=0
while (( $i < $nr_elements ))
do
	input=${inputlines[$i]} # is next form array
	printf "  Test %3d:   %-20s   " $i  "'$input'"
	> $OUT/out_temp; > $OUT/out_mini; > $OUT/out_orig
	./minishell "$input" | cat -e > $OUT/out_temp
	#echo "$input" | cat -e > out_orig
	eval $input | cat -e > $OUT/out_orig
	#echo "syntax error from initial check: redirections; exit" | cat -e > out_orig
	test_syntax_error "out_orig" "out_mini" "valid" 
	((i=i+1))
done

rm out
echo ""


############################################################
