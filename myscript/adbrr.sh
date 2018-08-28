#! /bin/bash

adb_device_not_found=1
adb_success=0
adb_root_failure=-1
adb_remount_failure=-2

function handle_adb(){
	adb root
	result=$?
	if [[ ${result} -eq ${adb_success} ]]; then
		adb remount
		result=$?
		if [[ ${result} -eq ${adb_success} ]]; then
			return ${adb_success}
		else
			# echo "adb remount failure!"
			return ${adb_remount_failure}
		fi
	else
		if [[ ${result} -eq ${adb_device_not_found} ]]; then
			return ${adb_device_not_found}
		fi
		# echo "adb root failure!"
		return ${adb_root_failure}
	fi
}

for((i=0;i<10;i++))
do
	handle_adb
	result=$?
	if [[ ${result} -eq ${adb_success} ]]; then
		echo "adb root and adb remount success!"
		if [[ ${1} == "shell" ]]; then
			adb shell
		fi
		break
	elif [[ ${result} -eq ${adb_device_not_found} ]]; then
		# echo "error: device not found"
		# break
		sleep 2
	fi
done
