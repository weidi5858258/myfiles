#! /bin/bash

# source push.sh 用户名 密码 ip 源文件 命令
loginuser=$1
loginpass=$2
ipaddr=$3
source_file=$4
do_command=$5

./scp_push.sh ${loginuser} ${loginpass} ${ipaddr} ${source_file} 
# ./ssh_auto_login_dosomething.sh 用户名 密码 ip 命令
./ssh_auto_login_dosomething.sh ${loginuser} ${loginpass} ${ipaddr} "${do_command}" > result.txt
sed -i 's///g' result.txt
var=`cat result.txt | grep -x -e '0'`

if [ "${var}" = "0" ]; then
	rm -rf result.txt
	./ssh_auto_login_dosomething.sh ${loginuser} ${loginpass} ${ipaddr} "rm -rf dtcp-ip-player.zip" > result.txt
	sed -i 's///g' result.txt
 	var=`cat result.txt | grep -x -e '0'`
	if [ ${var} = "0" ]; then
	        rm -rf result.txt
	        echo "Operator Succeeded:$?"
	else
		echo "Operator Failed:$?"
	fi
else
        cd ${currentDir}
        echo "Unzip Failed:$?"
fi
