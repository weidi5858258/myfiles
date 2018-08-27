#! /usr/bin/expect -f

set current_time [lindex $argv 0]
# send "${current_time}\n"
set username "weidi5858258@sina.com"
set password "wangliwei1986"
#
spawn git add .
sleep 15
#
spawn git commit -m "${current_time}"
sleep 15
#
spawn git push
#
expect {
	# -re "Username for 'https://github.com'"
	# {
	# 	send "${username}\r"
	# }
	-re "Username for"
	{
		send "${username}\r"
	}
	-re "Password for" 
	{
		send "${password}\r"
	}
}
#
expect {
	# -re -re "Password for 'https://weidi5858258@sina.com@github.com'"
	# {
	# 	send "${password}\r"
	# }
	-re "Password for" 
	{
		send "${password}\r"
	}
}

interact
