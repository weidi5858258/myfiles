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
-g -Wall -std=c++11 \
-ljpeg \
${codeFiles} \
-lsqlite3 \
-lpthread \
-L /opt/libjpeg-turbo/lib32/*.a \
#/opt/libjpeg-turbo/lib32/libjpeg.so \
#/opt/libjpeg-turbo/lib32/libturbojpeg.so \
#g++ -o openpicture openpicture.cpp -I /usr/local/include -I /usr/local/include/opencv -I /usr/local/include/opencv2 -L /usr/local/lib /usr/local/lib/*.so

# -rdynamic -lpthread -levent -lcrypt -ldl \
# g++ main.cpp -lrt -Wl,-Bstatic -lboost_system -lboost_thread -Wl,-Bdynamic -Wl,--as-needed

if [[ -e ${currentDir}"/"run_me ]]; then
	# run
	./run_me
fi

