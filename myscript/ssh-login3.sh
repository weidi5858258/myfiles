#!/bin/bash

# run me

# source ssh-login3.sh dtcp-ip-player

if [ $# -eq 1 ]
then

	folder=$1
	loginuser=""
	loginpass=""
	ipaddr="192.168.0.100"

	do_command="source .semc.sh ${folder}"

	echo "start..."

	if [ $? -eq 0 ]
	then
		# ./ssh_auto_login_dosomething.sh 用户名 密码 ip 命令
		./ssh_auto_login_dosomething.sh "${loginuser}" "${loginpass}" "${ipaddr}" "${do_command}" > result.txt
		# var=`cat result.txt | grep '^[0-9]' | tail -n 1` | grep '[[:blank:][0-9][:blank:]]'
		var=`cat result.txt | grep -w -e '0' | tail -n 2 | head -n 1`
		#var=$?
		echo "==========>$var"
		#&& 
		
		if [ "${var}" = "0
" ]
		then
			cd ~
			#rm -rf result.txt
			echo "ERROR,远程编译失败！！！"
			#exit 1;
		fi

		# file_apk=""
		if [ ${folder} = "dtcp-ip-player" ];
		then
			file_apk="SemcDTCPIPPlayer.apk"
		elif [ ${folder} = "video-player" ];
		then
			file_apk="SemcVideo.apk"
		elif [ ${folder} = "Album" ];
		then
			file_apk="SemcAlbum.apk"
		elif [ ${folder} = "walkman-music-player" ];
		then
			file_apk="SemcMusic.apk"
		else
			file_apk=""
		fi
	else
		echo "failed:$?"
	fi

else
	echo "Failed!At least parameter has one:$?"
fi
# 接下去的命令已经在本地执行了

if [ -z ${file_apk} ]
then
	echo "亲，俺找不到您想要的文件哦！！！"
else
	# ./scp_pull.sh 用户名 密码 ip 源文件 目的文件
	#./scp_pull.sh "${loginuser}" "${loginpass}" "${ipaddr}" "/home/${loginuser}/${file_apk}" "/root/mydev/tools/SignTool/unsignedapk" 
	./ssh_auto_login_dosomething.sh "${loginuser}" "${loginpass}" "${ipaddr}" "test -e /home/he/${file_apk}" > result.txt
	# var=`cat result.txt | grep '^[0-9]' | tail -n 1`
	var=`cat result.txt | grep -w -e '0' | tail -n 2 | head -n 1`
	# var=${var}
	#var=$?
	echo "---------->$var"
	
	if [ "${var}" = "0
" ] 
	then
		cd ~
		#rm -rf result.txt
		echo "ERROR,远程没有这样的文件！！！"
		#exit 1;
	fi

	./scp_pull.sh "${loginuser}" "${loginpass}" "${ipaddr}" "/home/${loginuser}/${file_apk}" "/root/mydev/tools/SignTool/unsignedapk" &&
	cd /root/mydev/tools/SignTool &&
	java -jar signapk.jar -w platform.x509.pem platform.pk8 unsignedapk/${file_apk} signedapk/${file_apk} &&
	cd ~ 

	./ssh_auto_login_dosomething.sh ${loginuser} ${loginpass} ${ipaddr} "rm -rf ${file_apk}" > result.txt
	# var=`cat result.txt | grep '^[0-9]' | tail -n 1`
	var=`cat result.txt | grep -w -e '0' | tail -n 2 | head -n 1`
	# var=${var}
	echo "**********>$var"
	
	if [ "${var}" = "0
" ]
	then
		cd ~
		#rm -rf result.txt
		echo "ERROR,删除远程文件失败！！！"
		#exit 1;
	fi

	#rm -rf result.txt

	if [ $? -eq 0 ]
	then
		echo "operator succeeded:$?"
	else
		echo "operator failed:$?"
	fi
fi

































