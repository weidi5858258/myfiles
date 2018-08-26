#! /bin/bash

current_time=`date "+%Y%m%d%H%M%S"`
echo ${current_time}
#
git add .
#
git commit -m "${current_time}"
#
spawn git push
#
expect {
	-re "Username for 'https://github.com'" 
	{
		# send "yes\r"
		echo "Username"
	} 
	-re "Password for 'https://weidi5858258@sina.com@github.com'" 
	{
		# send "${loginpass}\r"
		echo "Username"
	} 
}




