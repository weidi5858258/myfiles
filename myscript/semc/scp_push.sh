#!/usr/bin/expect -f

# 作用： 把本地文件上传到远程服务器上
# 注意： 必须要用"./"这种方式运行这个脚本
# 用法： ./scp_push.sh 用户名 密码 ip 源文件 

# 192.168.0.100
# 接受第一个参数
set loginuser [lindex $argv 0]
set loginpass [lindex $argv 1]
set ipaddr [lindex $argv 2] 
set source_file [lindex $argv 3]

# 单位：秒(下面参数基本不变)
set timeout 300
set cmd_prompt "]#|~]?"

spawn scp -r ${source_file} ${loginuser}@${ipaddr}:/home/${loginuser}

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
		# send "${loginpass}\r"
	} 
	-re "Connection refused" 
	{
		#exit
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
		send "${loginpass}\r"
	}
	-re $cmd_prompt 
	{
		send "\r"
	}
}
