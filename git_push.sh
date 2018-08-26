#! /usr/bin/expect -f

# set current_time `date "+%Y%m%d%H%M%S"`
set current_time "201808262258"
set username "weidi5858258@sina.com"
set Password "wangliwei1986"
#
spawn git add .
#
spawn git commit -m "${current_time}"
#
spawn git push
#
expect {
	-re "Username for 'https://github.com'" 
	{
		send "${username}\r"
	} 
	-re "Password for 'https://weidi5858258@sina.com@github.com'" 
	{
		send "${Password}\r"
	} 
}




