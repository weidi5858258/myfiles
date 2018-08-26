#! /usr/bin

#run command: . .git_push.sh
current_time=`date "+%Y%m%d%H%M%S"`
current_time='"'${current_time}'"'
# echo ${current_time}

#Don't change
./.git_push_impl.sh ${current_time}
