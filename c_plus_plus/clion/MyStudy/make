#! /bin/bash

currentDir=`pwd`

# echo ${currentDir}

function init() {
	codeFiles=""
}

function digui() {	
	if [ -d ${1} ]
	then
		for dir in $(ls ${1})
		do
			local tempDir="${1}/${dir}"
			if [ -d ${tempDir} ]; then
				# echo "It's directory."
				digui ${tempDir}
			else
				# doSomething
				# echo "It's not directory." 
				if [[ ! ${tempDir} =~ "cmake-build-debug" ]] && \
					 [[ ${tempDir} =~ ".c" || ${tempDir} =~ ".cpp" ]]; then

					if [[ ${tempDir} =~ ${currentDir} ]]; then
						tempDir=${tempDir#*${currentDir}}
						codeFile=${tempDir#*/}
						codeFiles=${codeFiles}" "${codeFile}
					fi
				fi
			fi
		done
	fi
}

# start
if [[ -e ${currentDir}"/"run_me ]]; then
	rm -rf ${currentDir}"/"run_me
fi

init

digui ${currentDir}

# echo ${codeFiles}
g++ \
-x c++ \
-o run_me \
${codeFiles} \
-lsqlite3 \
-lpthread \

if [[ -e ${currentDir}"/"run_me ]]; then
	# run
	./run_me
fi

