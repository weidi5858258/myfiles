#! /usr/bin/expect -f

# 这个脚本的作用是登录远程后执行一些命令

# 用户名 密码 ip 命令

# set folder $1 #ERROR
# 接受第一个参数
set loginuser [lindex $argv 0]
set loginpass [lindex $argv 1]
set ipaddr [lindex $argv 2]
set do_command [lindex $argv 3]

#set loginuser "" 
#set loginpass ""
#set ipaddr {192.168.0.100}
#set script_name "semc.sh"
#set folder [lindex $argv 1]
#set do_command "source ${script_name} ${folder}"

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
		send "${loginpass}\r"
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
		send "${do_command}\r"
		# 必须要这样子写
		send "result=`echo $?`\r"
		send "echo \${result}\r"
		send "exit \${result}\r"
	}
}

#exit(不能在这里写)
interact
