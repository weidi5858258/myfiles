#! /bin/bash

# input keyevent KEYCODE_BACK
sleep 5
saveDir="/storage/emulated/0/DCIM/screencap/";
currentTime=`date +%Y%m%d%H%M%S`;
screencap -p ${saveDir}${currentTime}".png";
