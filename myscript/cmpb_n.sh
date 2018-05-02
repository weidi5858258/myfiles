#!/bin/sh
# cd ~/mydev/Sony/src/android/m-sony/vendor/mediatek/open/frameworks/cmpb
# mm -B
path="/root/mydev/Sony_N/src/android/n-sony"
cd $path &&
mmm -B $path/vendor/mediatek/open/frameworks/cmpb
if [ $? -eq 0 ]
then
	adb root
	if [ $? -eq 0 ]
	then
		adb remount
		if [ $? -eq 0 ]
		then
			adb push $path/out/target/product/Trinity_US/system/lib/libmtkcmpb.so /system/lib
			if [ $? -eq 0 ]
			then
				# echo "adb push succeeded:$?"
				adb shell chmod 777 /system/lib/libmtkcmpb.so
				cd $path
			else
				echo "adb push failed:$?"
				cd $path
			fi
		else
			echo  "adb remount failed:$?"
			cd $path
		fi
	else
		echo "adb root failed:$?"
		cd $path
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
			cd $path &&
			source cmpb.sh
		else
			echo "lunch failed:$?"
			cd $path
		fi
	else
		echo "source build/envsetup.sh failed:$?"
		cd $path
	fi
fi
