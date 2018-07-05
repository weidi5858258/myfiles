#! /bin/bash

# adb shell input input keyevent KEYCODE_POWER
# adb shell input input keyevent KEYCODE_HOME
# adb shell input input keyevent KEYCODE_BACK
# adb shell input input keyevent KEYCODE_DPAD_UP
# adb shell input input keyevent KEYCODE_DPAD_DOWN
# adb shell input input keyevent KEYCODE_DPAD_LEFT
# adb shell input input keyevent KEYCODE_DPAD_RIGHT
# adb shell input input keyevent KEYCODE_DPAD_CENTER

activity="com.alipay.mobile.chatapp.ui.PersonalChatMsgActivity_"
countTxt="send_count.txt"

function init(){
	count=`cat ${countTxt}`
	if [[ -z ${count} ]]; then
		count=0
	fi
}

function toDo(){
	sleep 1 
	currentActivity=`adbd shell dumpsys window w | grep \/ | grep name=`
	if [[ ${currentActivity} =~ ${activity} ]]; then
		adbd shell input tap 455 750
		adbd shell input tap 450 1180
		adbd shell input tap 645 1212
		let count++
		echo ${count} > ${countTxt}
	fi
}

init

while :
do
	toDo;
done

# IMEI MEID
# mbinput_service@baidu.com
# 275568230 郭伟
# IMEI: 353919025680137 75 BC F8 E0