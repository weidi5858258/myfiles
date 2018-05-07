#! /bin/bash

function init(){
	pathCount=0
	spaceFlag=50
	currentDir=`pwd`
	# /storage/emulated/0/data/local/artifact
	# currentDir="/storage/emulated/0"
	# currentDir="/"

	# ignorePathArray[0]="/storage/emulated/0/Android"
	# ignorePathArray[1]="/storage/emulated/0/Android/data"
	# ignorePathArray[2]="/storage/emulated/0/Android/data/com.tencent.mobileqq"
	# ignorePathArray[3]="/storage/emulated/0/DCIM"
	# ignorePathArray[4]="/storage/emulated/0/DCIM/Camera"
	# ignorePathArray[5]="/storage/emulated/0/Record"
	# ignorePathArray[6]="/storage/emulated/0/Record/PhoneRecord"
	# ignorePathArray[7]="/storage/emulated/0/Tencent"
	# ignorePathArray[8]="/storage/emulated/0/Tencent/MicroMsg"
	# ignorePathArray[9]="/storage/emulated/0/Tencent/MobileQQ"
	# ignorePathArray[10]="/storage/emulated/0/TitaniumBackup"
	# ignorePathArray[11]="/storage/emulated/0/alipay"
	# ignorePathArray[12]="/storage/emulated/0/kgmusic"
	# ignorePathArray[13]="/storage/emulated/0/kgmusic/download"

	# for ignPath in "${ignorePathArray[@]}"
	# do
	# 	echo ${ignPath}
	# done
}

function addDir(){
	rmPathArray[pathCount]=${1}
	let pathCount++
}

function rmDir(){
	# echo ${1}
	local tempPathCount=0

	for ignPath in "${rmPathArray[@]}"
	do
		# echo ${ignPath}
		unset rmPathArray[tempPathCount] 
		local tempFlag="false"
		for tempIgnPath in "${rmPathArray[@]}"
		do
			if [[ ${tempIgnPath} == *"${ignPath}"* ]]; then
				tempFlag="true"
				break
			fi
		done
		if [[ ${tempFlag} == "false" ]]; then
			rmPathArray[tempPathCount]=${ignPath}
		fi
		let tempPathCount++
	done

	for ignPath in "${rmPathArray[@]}"
	do
		ignPath=`du -sh ${ignPath}`
		echo ${ignPath}
		# 实际功能代码
		# if [[ -e ${ignPath} ]]; then
		# 	if [[ -d ${ignPath} ]]; then
		# 		rm -rf ${ignPath}"/*"
		# 	else
		# 		rm -rf ${ignPath}
		# 	fi
		# fi
	done
}

function digui() {	
	if [[ -d ${1} ]]; then
		for dir in $(ls ${1})
		do
			local tempDir="${1}/${dir}"
			if [[ -d ${tempDir} ]]; then
				# echo "It's directory."

				local needToIgnore="false";
				for ignPath in "${ignorePathArray[@]}"
				do
					if [[ ${tempDir} == ${ignPath} ]]; then
						needToIgnore="true"
						break
					fi
				done
				if [[ ${needToIgnore} == "true" ]]; then
					digui ${tempDir}
					continue
				fi

				local tempDuResult=`du -sh ${tempDir}`
				local tempSpace=${tempDuResult%%/*}
				tempSpace=`echo ${tempSpace} | awk -F '[ ;]+' '{print $1}'`
				if [[ ${tempSpace} == *"M"* ]]; then
					tempSpace=${tempSpace%M*}
					if [[ ${tempSpace} == *"."* ]]; then
						tempSpace=`echo ${tempSpace} | awk -F '.' '{print $1}'`
					fi

					if [[ ${tempSpace} -ge ${spaceFlag} ]]; then
						# echo ${tempDuResult}
						addDir ${tempDir}
					fi
				elif [[ ${tempSpace} == *"G"* ]]; then
					# echo ${tempDuResult}
					addDir ${tempDir}
				fi

				digui ${tempDir}
			fi
		done
	fi
}

# start
init

digui ${currentDir}

rmDir
