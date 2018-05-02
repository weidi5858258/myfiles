#!/bin/bash

folder=$1
loginuser=""
loginpass=""
ipaddr="192.168.0.1"

timeout=300
cmd_prompt="]#|~]?"

function spawn()
{

	/usr/bin/expect <<-EOF 	

	case $1 in
		"ssh")
			spawn ssh $loginuser@$ipaddr
		;;
		"scp")
			spawn scp -r $loginuser@$ipaddr:/home/${loginuser}/${file_apk} /root/mydev/tools/SignTool/unsignedapk
		;;
		*)
			return -1;
		;;
	esac

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
			# exit
		} 
		-re "Connection refused" 
		{
			# exit
		} 
		timeout 
		{
			# exit
		} 
		eof 
		{
			# exit
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

	# now,we do some commands
	expect {
		-re $cmd_prompt 
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
	
}













if [ $# -eq 1 ]
then

	folder=$1
	loginuser="he"
	loginpass="he"
	ipaddr="43.31.108.45"

	timeout=300
	cmd_prompt="]#|~]?"

	echo "start..."

	if [ $? -eq 0 ]
	then

	/usr/bin/expect <<-EOF 

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
				send "echo '1'\r"
			}
			-re $cmd_prompt 
			{
				send "\r"
				send "source semc.sh ${folder}\r"
				send "echo '2'\r"
			}
			-re "Last login:"
			{
				send "source semc.sh ${folder}\r"
				send "echo '3'\r"
				# send "sleep 10\r"
				# send "exit\r"		
			}
		}

		# now,we do some commands
		expect {
			-re $cmd_prompt 
			{
				# send "sleep 1\r"
				# send "echo ${folder}\r"
				send "source semc.sh ${folder}\r"
				send "echo '4'\r"
				# send "sleep 10\r"
				# 如果想要继续往下走，则必须退出
				# send "exit\r"
			}
			-re "jpx20140039:"
			{
				# send "echo ${folder}\r"
				send "source semc.sh ${folder}\r"
				send "echo '5'\r"
				# send "sleep 10\r"
				# send "exit\r"		
			}
		}

		interact
	expect eof
	EOF

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

	else
		echo "failed:$?"
	fi

else
	echo "Failed!At least parameter has one:$?"
fi
# 接下去的命令已经在本地执行了

echo "hahahaha"

































