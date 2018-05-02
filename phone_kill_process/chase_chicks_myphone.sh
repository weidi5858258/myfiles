#! /bin/bash

function toDo(){
	sleep 20 
	input keyevent KEYCODE_POWER
	sleep 30 
	input keyevent KEYCODE_HOME 
	sleep 10 
	input tap 520 1166
	sleep 5 
	input tap 288 687
	sleep 10 
	input tap 253 980
	input tap 526 988
	input tap 635 1155
	input keyevent KEYCODE_BACK
	input keyevent KEYCODE_BACK
	sleep 10 
	input keyevent KEYCODE_POWER
	sleep 540 
}

while :
do
	toDo;
done