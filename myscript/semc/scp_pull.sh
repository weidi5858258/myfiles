#!/usr/bin/expect -f

# 用户名 密码 ip 源文件 目的文件

# 接受第一个参数
set loginuser [lindex $argv 0]
set loginpass [lindex $argv 1]
set ipaddr [lindex $argv 2]
set source_file [lindex $argv 3]
set dest_file [lindex $argv 4]

# 单位：秒(下面参数基本不变)
set timeout 300
set cmd_prompt "]#|~]?"

spawn scp -r $loginuser@$ipaddr:${source_file} ${dest_file}

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
		send "${loginpass}\r"
	}
	-re $cmd_prompt 
	{
		send "\r"
	}
}
