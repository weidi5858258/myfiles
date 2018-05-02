#!/bin/sh

folder=$1
if [ ${folder} != "a" -a ${folder} != "b" -a ${folder} != "c" ]
then
	exit 1;
else
	echo "..."
fi
