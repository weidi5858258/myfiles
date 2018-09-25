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
				if [[ ! ${tempDir} =~ "cmake-build-debug" ]] \
				    && [[ ${tempDir} =~ ".c" || ${tempDir} =~ ".cc" || ${tempDir} =~ ".cpp" ]]; then
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

                   # start #
###############################################
if [[ -e ${currentDir}"/"run_me ]]; then
	rm -rf ${currentDir}"/"run_me
fi

init

digui ${currentDir}

#-l动态库名称
#发现没有libpostproc.so这个动态库,因此需要像下面这样操作一下
#ln -s libpostproc-ffmpeg.so.53.3.100 libpostproc.so
# echo ${codeFiles}

#-I"/mnt/d/mydev/ffmpeg/include"

g++ \
-x c++ \
-o run_me \
-g -Wall -std=c++11 \
${codeFiles} \
-lz -lrt -lm -lpthread \

#-lpostproc \
if [[ -e ${currentDir}"/"run_me ]]; then
	# run
	./run_me
fi
###############################################












# 下面的代码是编译出一个可以在android手机上运行的可执行文件run_me
# "adb push"到手机上后使用"./run_me"命令就可以行动了
# "-static"不能少,少了就运行不了
###############################################

#arm-cortexa9-linux-gnueabihf-g++ \
#-x c++ \
#-o run_me \
#-static \
#-g -Wall -std=c++11 \
#${codeFiles} \
#-lpthread \

###############################################

###############################################
#Ubuntu上安装交叉编译环境(arm-linux-gcc 4.9.3)
#首先下载并解压编译器:
#1，git clone https://github.com/friendlyarm/prebuilts.git
#得到prebuilts文件夹
#2，mkdir -p /opt/FriendlyARM/toolschain
#3，把prebuilts/gcc-x64/arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
#这个文件复制到上面建好的文件夹下，
#解压：tar xf arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
#4，gedit /etc/profile
#在最后面加上：
#export ARM_LINUX_GCC=/mydev/tool/opt/FriendlyARM/toolschain/4.9.3
#export PATH=$ARM_LINUX_GCC/bin:$PATH
#下面这个没有试
#export PATH=/opt/FriendlyARM/toolchain/4.9.3/bin:$PATH
#export GCC_COLORS=auto
#5，使环境生效
#source /etc/profile
#测试是否成功，如果输出有版本号信息，那么说明安装成功
#arm-linux-gcc -v
###############################################



# 引用SDL2库编译自己的程序
# gcc -o myprogram myprogram.c `sdl2-config --cflags --libs`

#backup
#g++ \
#-x c++ \
#-o run_me \
#-g -Wall -std=c++11 \
#-L. -ljpeg -lturbojpeg \
#${codeFiles} \
#-lsqlite3 \
#-lpthread \
#-L/usr/lib/x86_64-linux-gnu -lSDL2 \
#-L/usr/lib/x86_64-linux-gnu -lSDL2main \
#-L/usr/lib/x86_64-linux-gnu -lavformat \
#-L/usr/lib/x86_64-linux-gnu -lavcodec \
#-L/usr/lib/x86_64-linux-gnu -lavutil \
#-L/usr/lib/x86_64-linux-gnu -lswscale \

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



