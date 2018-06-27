#! /usr/bin/expect -f

# 这个脚本的作用是登录远程后执行一些命令

# set folder $1 #error
# 接受第一个参数
set folder [lindex $argv 0]
set loginuser "" 
set loginpass ""
# set loginpass {weidi}
set ipaddr {192.168.0.100}
set script_name "semc.sh"
set do_command "source ${script_name} ${folder}"

# 单位：秒(下面参数基本不变)
set timeout 300
set cmd_prompt "]#|~]?"

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
		#exit
	} 
	-re "Connection refused" 
	{
		#exit
	} 
	timeout 
	{
		#exit
	} 
	eof 
	{
		#exit
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
		# send "echo 'bye'\r"
		# send "sleep 1\r"
		# send "source semc.sh dtcp-ip-player\r"
		send "${do_command}\r"
		send "exit\r"
	}
}

# exit(不能在这里写)
interact
