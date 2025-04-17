#!/bin/bash

max_score=100
max_id=1805125

if [[ $# -ge 1 ]]
then
	if [[ "$1" == *[0-9]* && $1 -ge 0 ]]
	then
		max_score=$1
	else
		echo "max score is invalid"
		exit 1
	fi
fi

if [[ $# -ge 2 ]]
then
	if [[ "$2" == *[0-9]* && $2 -ge 1 && $2 -le 9 ]]
	then
		max_id=$((1805120+$2))
	else
		echo "max id is invalid"
		exit 1
	fi
fi

echo 'student_id,score'>output.csv

for ((i=1805121;i<=$max_id;i++))
do
	score=$max_score
	if [[ -e Submissions/$i && -d Submissions/$i && -e Submissions/$i/$i.sh && -f Submissions/$i/$i.sh ]]
	then 
		
		bash Submissions/$i/$i.sh > tmp.txt
		(( err = $(diff -w AcceptedOutput.txt tmp.txt | grep -c -e "^<" -e "^>") ))
		(( score = $score - $err * 5 ))
		if [[ score -le 0 ]]
		then
			score=0
		fi
		rm tmp.txt
		
		copy_flag=0	
		for ((j=1805121;j<=$max_id;j++))
		do
			if [[ i -ne j &&  -e Submissions/$j && -d Submissions/$j && -e Submissions/$j/$j.sh && -f Submissions/$i/$i.sh && $(diff -ZB Submissions/$i/$i.sh Submissions/$j/$j.sh | wc -l) -eq 0 ]]
			then 
				copy_flag=1
			fi
		done
		
		if [[ $copy_flag -eq 1 ]]
		then
			score=$(( 0-$score ))
		fi
	else
		score=0
	fi
	
	echo "$i,$score">>output.csv
	
done

