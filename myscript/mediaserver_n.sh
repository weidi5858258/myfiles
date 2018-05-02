#! /bin/sh

n_sony="/root/mydev/Sony_N/src/android/n-sony/"

cd ${n_sony}

sonypb="${n_sony}vendor/sony/frameworks/sonypb"
libmediaplayerservice="${n_sony}frameworks/av/media/libmediaplayerservice"
mediaserver="${n_sony}frameworks/av/media/mediaserver"
out="${n_sony}out/target/product/Trinity_US/system/bin/mediaserver"

if [ ${source_lunch} = "true" ]
then
	mmm -B ${sonypb}

	if [ $? -eq 0 ]
	then
		mmm -B ${libmediaplayerservice}

		if [ $? -eq 0 ]
		then
			mmm -B ${mediaserver}

			if [ $? -eq 0 ]
			then
				adb root &&
				adb remount &&
				adb shell rm -rf /system/bin/mediaserver &&
				adb push ${out} /system/bin &&
				adb shell chmod 777 /system/bin/mediaserver
				if [ $? -eq 0 ]
				then
					echo "adb push succeeded:$?"
					cd ${n_sony}
				else
					echo "adb push failed:$?"
					cd ${n_sony}
				fi
			else
				cd ${n_sony}
				echo "${mediaserver} failed:$?"
			fi
		else
			cd ${n_sony}
			echo "${libmediaplayerservice} failed:$?"
		fi
	else
		cd ${n_sony}
		echo "${sonypb} failed:$?"
	fi
else
	cd ${n_sony}
	source build/envsetup.sh &&
	lunch Trinity_US-userdebug
	if [ $? -eq 0 ]
	then
		source_lunch="true"
		cd ${n_sony}
		source mediaserver.sh
	else
		echo "source / lunch failed:$?"
		cd ${n_sony}
	fi
fi

