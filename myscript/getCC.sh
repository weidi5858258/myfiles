#! /bin/bash

currentDir=`pwd`
sourceDir="/mnt/d/AOSP"
androidSourceDirName="AOSP"
destinationDir="${currentDir}/AOSP"

function handleFile() {
    # 文件路径
    filePath=${1}
	if [[ -e ${filePath} ]]; then
	    # 文件名
        tempFileName=${filePath##*/}
        # 除去文件名后的文件路径
        tempFilePath=${filePath%/*}
        # tempFilePath=${tempFilePath##*${androidSourceDirName}}
        # if [ "${tempFileName##*.}" = "h" ] \
        #     || [ "${tempFileName##*.}" = "c" ] \
        #     || [ "${tempFileName##*.}" = "cc" ] \
        #     || [ "${tempFileName##*.}" = "cpp" ] \
        #     || [ "${tempFileName##*.}" = "mk" ]; then
        #     tempFilePath=${destinationDir}"/"${tempFilePath}
        #     if [[ ! ${tempFilePath} =~ "cmake-build-debug" ]] \
        #           && [[ ! ${tempFilePath} =~ "test" ]]; then
        #         # 如果目录不存在,就创建目录
        #         if [[ ! -e ${tempFilePath} ]]; then
        #             mkdir -p ${tempFilePath}
        #         fi
        #         # 复制文件
        #         cp ${filePath} ${tempFilePath}
        #         # echo ${tempFilePath}
        #     fi
        # fi

        if [ "${tempFileName##*.}" = "h" ]; then
            # head.txt文件中如果不存在字符串${tempFilePath}时成立
            if [ `grep -c "${tempFilePath}" head.txt` -eq '0' ]; then
                echo ${tempFilePath} >> head.txt
            fi
        fi
	fi
}

function digui() {
	if [ -d ${1} ]; then
		for dir in $(ls ${1})
		do
			local tempDir="${1}/${dir}"
			if [ -d ${tempDir} ]; then
				# It's directory.
				digui ${tempDir}
			else
				# It's not directory.
				
				# doSomething
				# echo ${tempDir}
				handleFile ${tempDir}
			fi
		done
	fi
}
digui ${sourceDir}