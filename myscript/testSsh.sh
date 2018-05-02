#!/bin/bash

folder=$1
loginuser=""
loginpass=""
ipaddr="192.168.0.100"

timeout=300
cmd_prompt="]#|~]?"

function spawn() {

	if [ $1 = "ssh" ]
	then
/usr/bin/expect <<-EOF 
		spawn ssh $loginuser@$ipaddr
	elif [ $1 = "scp" ]
	then
/usr/bin/expect <<-EOF 
		spawn scp -r $loginuser@$ipaddr:/home/${loginuser}/${file_apk} /root/mydev/tools/SignTool/unsignedapk
	fi

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
			send "source semc.sh ${2}\r"
			# 如果想要继续往下走，则必须退出
			send "exit\r"
		}
	}

#interact
#expect eof
EOF
	echo ""
}

spawn "ssh" "dtcp-ip-player";
