#!/bin/bash

if [ $# -eq 1 ]
then

	folder=$1
	loginuser=""
	loginpass=""
	ipaddr="192.168.0.100"

	timeout=300
	cmd_prompt="]#|~]?"
	cmd_prompt2="\:\~\$"

	echo "start..."

	if [ $? -eq 0 ]
	then

		/usr/bin/expect  <<-EOF 

		# login by ssh 
		spawn ssh $loginuser@$ipaddr

		expect {
			-re "Are you sure you want to continue connecting (yes/no)?" 
			{
				send "yes\r"
			} 
			-re "assword:" 
			{
				send "${loginpass}\r"
			} 
			-re "Permission denied, please try again." 
			{
				exit
			} 
			-re "Connection refused" 
			{
				exit
			} 
			timeout 
			{
				exit
			} 
			eof 
			{
				exit
			}
		}

		expect {
			-re "assword:" 
			{
				send "$loginpass\r"
			}
			-re $cmd_prompt 
			{
				send "echo $cmd_prompt\r"
				send "\r"
			}
			-re 'he@jpx20140039:~$'
			{
				# send "sleep 1\r"
				send "source semc.sh ${folder}\r"
				# 如果想要继续往下走，则必须退出
				send "exit\r"
			}
		}

		# now,we do some commands
		expect {
			-re $cmd_prompt 
			{
				# send "sleep 1\r"
				send "source semc.sh ${folder}\r"
				# 如果想要继续往下走，则必须退出
				send "exit\r"
			}
			-re $cmd_prompt2
			{
				# send "sleep 1\r"
				send "source semc.sh ${folder}\r"
				# 如果想要继续往下走，则必须退出
				send "exit\r"
			}
			-re 'he@jpx20140039:~$'
			{
				# send "sleep 1\r"
				send "source semc.sh ${folder}\r"
				# 如果想要继续往下走，则必须退出
				send "exit\r"
			}
		}

interact
expect eof
EOF

		{
		# file_apk=""
		if [ ${folder} = "dtcp-ip-player" ];
		then
			file_apk="SemcDTCPIPPlayer.apk"
		elif [ ${folder} = "video-player" ];
		then
			rm -rf ivy_SemcVideo
		elif [ ${folder} = "Album" ];
		then
			echo "..."
		elif [ ${folder} = "walkman-music-player" ];
		then
			echo "..."
		else
			echo "..."
		fi
		}
	else
		echo "failed:$?"
	fi

else
	echo "Failed!At least parameter has one:$?"
fi
# 接下去的命令已经在本地执行了

if [ -z ${file_apk} ]
then
	echo "null"
else
	
	/usr/bin/expect <<-EOF 
	spawn scp -r $loginuser@$ipaddr:/home/${loginuser}/${file_apk} /root/mydev/tools/SignTool/unsignedapk

	expect {
		-re "Are you sure you want to continue connecting (yes/no)?" 
		{
			send "yes\r"
		} 
		-re "assword:" 
		{
			send "${loginpass}\r"
		} 
		-re "Permission denied, please try again." 
		{
			exit
		} 
		-re "Connection refused" 
		{
			exit
		} 
		timeout 
		{
			exit
		} 
		eof 
		{
			exit
		}
	}

	expect {
		-re "assword:" 
		{
			send "$loginpass\r"
		}
		-re $cmd_prompt 
		{
			send "\r"
		}
	}

	# interact
	# expect eof
EOF

	cd /root/mydev/tools/SignTool &&
	java -jar signapk.jar -w platform.x509.pem platform.pk8 unsignedapk/${file_apk} signedapk/${file_apk} &&
	cd ~
	
	if [ $? -eq 0 ]
	then
		echo "operator succeeded:$?"
	else
		echo "operator failed:$?"
	fi

fi

































