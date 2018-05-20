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
-L. -ljpeg -lturbojpeg \
${codeFiles} \
-lsqlite3 \
-lpthread \

if [[ -e ${currentDir}"/"run_me ]]; then
	# run
	./run_me
fi

#把C/C++代码编译成动态库的过程
#1.
#首先组织好代码,需要有头文件,这个头文件是公开的
#2.
#只对所有的.c/.cpp文件进行编译,打包成so库
#3.
#命令:
#g++ ${codeFiles} -fpic -shared -o libweidi.so
#g++ ${codeFiles} -fPIC -shared -o libweidi.so
#4.
#使用
#在其他的代码中按平时一样引用公开的头文件,写好代码后
#g++ ${codeFiles} -L. -lweidi -o run_me
#说明:
#-L.表示so库与源码的相对位置
#-lweidi表示so库的名称(去掉lib和.so)
#-o run_me表示生成的可执行文件
#5.
#如果运行失败,那么
#export LD_LIBRARY_APTH=so库的位置/weidi(去掉lib和.so)
#或者
#cp so库 /usr/lib
#再次运行











#查看引用了哪些库
#ldd run_me
#不提倡使用静态方式编译,生成的文件很大,编译时间又长
#g++ ${codeFiles} -o run_me_static -static



