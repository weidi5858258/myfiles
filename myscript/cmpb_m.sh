#!/bin/sh
# cd ~/mydev/Sony/src/android/m-sony/vendor/mediatek/open/frameworks/cmpb
# mm -B
cd /root/mydev/Sony/src/android/m-sony &&
mmm -B /root/mydev/Sony/src/android/m-sony/vendor/mediatek/open/frameworks/cmpb
if [ $? -eq 0 ]
then
	adb root
	if [ $? -eq 0 ]
	then
		adb remount
		if [ $? -eq 0 ]
		then
			adb push /root/mydev/Sony/src/android/m-sony/out/target/product/Trinity_US/system/lib/libmtkcmpb.so /system/lib
			if [ $? -eq 0 ]
			then
				# echo "adb push succeeded:$?"
				adb shell chmod 777 /system/lib/libmtkcmpb.so
				cd /root/mydev/Sony/src/android/m-sony
			else
				echo "adb push failed:$?"
				cd /root/mydev/Sony/src/android/m-sony
			fi
		else
			echo  "adb remount failed:$?"
			cd /root/mydev/Sony/src/android/m-sony
		fi
	else
		echo "adb root failed:$?"
		cd /root/mydev/Sony/src/android/m-sony
	fi
else
	echo "mmm -B cmpb failed:$?"
	source build/envsetup.sh
	if [ $? -eq 0 ]
	then
		lunch Trinity_US-userdebug
		if [ $? -eq 0 ]
		then
			# echo "lunch succeeded:$?"
			cd /root/mydev/Sony/src/android/m-sony &&
			source cmpb.sh
		else
			echo "lunch failed:$?"
			cd /root/mydev/Sony/src/android/m-sony
		fi
	else
		echo "source build/envsetup.sh failed:$?"
		cd /root/mydev/Sony/src/android/m-sony
	fi
fi
