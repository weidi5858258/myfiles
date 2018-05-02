#! /bin/sh

# cd ~/mydev/Sony/src/android/m-sony/vendor/mediatek/open/frameworks/cmpb
# mm -B
# ~/mydev/Sony_N/src/android/n-sony/vendor/sony/frameworks/NetLib/DataSource

# 需要改的路径
n_sony="/root/mydev/Sony_N/src/android/n-sony/"

cd ${n_sony}

datasource="${n_sony}vendor/sony/frameworks/NetLib/DataSource"
path="/system/lib/"
filename="libDataSource.so"
filepath="${path}${filename}"
out="${n_sony}out/target/product/Trinity_US/obj/lib/${filename}"

if [ ${source_lunch} = "true" ]
then

	mmm -B ${datasource}
	if [ $? -eq 0 ]
	then
		adb root &&
		adb remount &&
		adb shell rm -rf ${filepath} &&
		adb push ${out} ${path} &&
		adb shell chmod 777 ${filepath}
		if [ $? -eq 0 ]
		then
			echo "adb push succeeded:$?"
			cd ${n_sony}
		else
			echo "adb push failed:$?"
			cd ${n_sony}
		fi
	else
		echo "mmm -B ${datasource} failed:$?"
	fi
else
	cd ${n_sony}
	source build/envsetup.sh &&
	lunch Trinity_US-userdebug
	if [ $? -eq 0 ]
	then
		source_lunch="true"
		cd ${n_sony}
		source datasource.sh
	else
		echo "source / lunch failed:$?"
		cd ${n_sony}
	fi
fi
