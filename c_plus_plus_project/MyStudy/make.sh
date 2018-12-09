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

#-l��̬������
#����û��libpostproc.so�����̬��,�����Ҫ��������������һ��
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












# ����Ĵ����Ǳ����һ��������android�ֻ������еĿ�ִ���ļ�run_me
# "adb push"���ֻ��Ϻ�ʹ��"./run_me"����Ϳ����ж���
# "-static"������,���˾����в���
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
#Ubuntu�ϰ�װ������뻷��(arm-linux-gcc 4.9.3)
#�������ز���ѹ������:
#1��git clone https://github.com/friendlyarm/prebuilts.git
#�õ�prebuilts�ļ���
#2��mkdir -p /opt/FriendlyARM/toolschain
#3����prebuilts/gcc-x64/arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
#����ļ����Ƶ����潨�õ��ļ����£�
#��ѹ��tar xf arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
#4��gedit /etc/profile
#���������ϣ�
#export ARM_LINUX_GCC=/mydev/tool/opt/FriendlyARM/toolschain/4.9.3
#export PATH=$ARM_LINUX_GCC/bin:$PATH
#�������û����
#export PATH=/opt/FriendlyARM/toolchain/4.9.3/bin:$PATH
#export GCC_COLORS=auto
#5��ʹ������Ч
#source /etc/profile
#�����Ƿ�ɹ����������а汾����Ϣ����ô˵����װ�ɹ�
#arm-linux-gcc -v
###############################################



# ����SDL2������Լ��ĳ���
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

#��C/C++�������ɶ�̬��Ĺ���
#1.
#������֯�ô���,��Ҫ��ͷ�ļ�,���ͷ�ļ��ǹ�����
#2.
#ֻ�����е�.c/.cpp�ļ����б���,�����so��
#3.
#����:
#g++ ${codeFiles} -fpic -shared -o libweidi.so
#g++ ${codeFiles} -fPIC -shared -o libweidi.so
#4.
#ʹ��
#�������Ĵ����а�ƽʱһ�����ù�����ͷ�ļ�,д�ô����
#g++ ${codeFiles} -L. -lweidi -o run_me
#˵��:
#-L.��ʾso����Դ������λ��
#-lweidi��ʾso�������(ȥ��lib��.so)
#-o run_me��ʾ���ɵĿ�ִ���ļ�
#5.
#�������ʧ��,��ô
#export LD_LIBRARY_APTH=so���λ��/weidi(ȥ��lib��.so)
#����
#cp so�� /usr/lib
#�ٴ�����











#�鿴��������Щ��
#ldd run_me
#���ᳫʹ�þ�̬��ʽ����,���ɵ��ļ��ܴ�,����ʱ���ֳ�
#g++ ${codeFiles} -o run_me_static -static



