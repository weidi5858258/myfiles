#! /usr/bin/expect -f

# set current_time `date "+%Y%m%d%H%M%S"`
set current_time "201808262258"
set username "weidi5858258@sina.com"
set password "wangliwei1986"
set cmd_prompt "]#|~]?"
#
# spawn git add .
#
# spawn git commit -m "${current_time}"
#
spawn git push
#
expect {
	# -re "Username for 'https://github.com'"
	-re "Username for"
	{
		send "${username}\r"
		# send "echo 'bye'\r"
	}
	# -re "Password for 'https://weidi5858258@sina.com@github.com'" 
	-re "Password for" 
	{
		send "${password}\r"
		# send "echo '${password}'\r"
	}
	# eof 
	# {
	# 	send "\r"
	# }
}
#
expect {
	-re "Password for" 
	{
		send "${password}\r"
	}
	# -re $cmd_prompt 
	# {
	# 	send "\r"
	# }
}

interact
