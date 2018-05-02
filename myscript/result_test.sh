#! /bin/bash

result=`cat result.txt | grep -w -e '0' | tail -n 2 | head -n 1`

if [ $result = "0" ]; then
	echo "1"
else
	echo "2"
fi
