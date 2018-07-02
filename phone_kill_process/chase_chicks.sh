#! /bin/bash

# adb shell input input keyevent KEYCODE_POWER
# adb shell input input keyevent KEYCODE_HOME
# adb shell input input keyevent KEYCODE_BACK
# adb shell input input keyevent KEYCODE_DPAD_UP
# adb shell input input keyevent KEYCODE_DPAD_DOWN
# adb shell input input keyevent KEYCODE_DPAD_LEFT
# adb shell input input keyevent KEYCODE_DPAD_RIGHT
# adb shell input input keyevent KEYCODE_DPAD_CENTER

function toDo(){
	sleep 20 
	input keyevent KEYCODE_POWER
	sleep 30 
	input keyevent KEYCODE_HOME 
	sleep 10 
	input tap 630 1210
	sleep 5 
	input tap 297 391
	sleep 10 
	input tap 307 936
	input tap 600 936
	input tap 690 1146
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

# IMEI MEID
# mbinput_service@baidu.com
# 275568230 郭伟
# IMEI: 353919025680137