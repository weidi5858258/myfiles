#! /bin/bash

echo -e "\n"

exit

########################array########################

# bash支持一维数组（不支持多维数组），并且没有限定
# 数组的大小。类似与C语言，数组元素的下标由0开始编号。
# 获取数组中的元素要利用下标，下标可以是整数或算术
# 表达式，其值应大于或等于0。在Shell中，用括号来表示
# 数组，数组元素用“空格”符号分割开。定义数组的一般
# 形式为：array_name=(value1 ... valuen)
array_name=(value0 value1 value2 value3)

array_name=(
value0
value1
value2
value3
)

array_name[0]=value0
array_name[1]=value1
array_name[2]=value2
for ignPath in "${array_name[@]}"
do
	echo ${ignPath}
done

# 读取数组元素值的一般格式是：${array_name[index]}
# valuen=${array_name[2]}

NAME[0]="Zara"
NAME[1]="Qadir"
NAME[2]="Mahnaz"
NAME[3]="Ayan"
NAME[4]="Daisy"
echo "First Index: ${NAME[0]}"
echo "Second Index: ${NAME[1]}"

# 使用@ 或 * 可以获取数组中的所有元素
# ${array_name[*]}
# ${array_name[@]}

NAME[0]="Zara"
NAME[1]="Qadir"
NAME[2]="Mahnaz"
NAME[3]="Ayan"
NAME[4]="Daisy"
echo "First Method: ${NAME[*]}"
echo "Second Method: ${NAME[@]}"

# 获取数组长度的方法与获取字符串长度的方法相同
# 取得数组元素的个数
length=${#array_name[@]}
# 或者
length=${#array_name[*]}
# 取得数组单个元素的长度
lengthn=${#array_name[n]}

a[0]="0"
a[10]="10"
echo "${a[*]}"
echo "${a[@]}"

########################array########################

########################break########################

# break命令允许跳出所有循环
# （终止执行后面的所有循环）。
# 在嵌套循环中，break 命令后面还可以跟一个整数，表示
# 跳出第几层循环。
# break n:表示跳出第 n 层循环。

for var1 in 1 2 3
do
   for var2 in 0 5
   do
        if [ $var1 -eq 2 -a $var2 -eq 0 ]
        then
			break 2
        else
			echo "$var1 $var2"
        fi
   done
done

# continue命令与break命令类似，只有一点差别，它不会
# 跳出所有循环，仅仅跳出当前循环。
while :
do
    echo -n "Input a number between 1 to 5: "
    read aNum
    case $aNum in
        1|2|3|4|5) 
			echo "Your number is $aNum!"
        ;;
        *) 
			echo "You do not select a number between 
			1 to 5!"
            continue
            echo "Game is over!"
        ;;
    esac
done

# continue 后面也可以跟一个数字，表示跳出第几层循环。
NUMS="1 2 3 4 5 6 7"
for NUM in $NUMS
do
	Q=`expr $NUM % 2`
	if [ $Q -eq 0 ]
	then
		echo "Number is an even number!!"
		continue
	fi
	echo "Found odd number"
done

########################break########################

########################case########################

# case 值 in
# 模式1)
#     command1
#     command2
#     command3
#     ;;
# 模式2）
#     command1
#     command2
#     command3
#     ;;
# *)
#     command1
#     command2
#     command3
#     ;;
# esac

echo 'Input a number between 1 to 4'
echo 'Your number is:\c'
read aNum
case $aNum in
    1)
		echo 'You select 1'
    ;;
    2)  
		echo 'You select 2'
    ;;
    3)  
		echo 'You select 3'
    ;;
    4)  
		echo 'You select 4'
    ;;
    *)  
		echo "You do not select a number between 
		1 to 4"
    ;;
esac

option="${1}"
case ${option} in
   -f) 
	   FILE="${2}"
       echo "File name is $FILE"
    ;;
   -d) 
	   DIR="${2}"
       echo "Dir name is $DIR"
    ;;
    *) 
       echo "`basename ${0}`:usage: [-f file] | 
       [-d directory]"
       # Command to come out of the program 
       # with status 1
       exit 1 
    ;;
esac

########################case########################

########################echo########################

# echo是Shell的一个内部指令，用于在屏幕上打印出指定
# 的字符串。命令格式：echo arg
# 显示转义字符
echo "\"It is a test\""
# 双引号也可以省略

# 如果变量与其它字符相连的话，需要使用大括号{ }
mouth=8
echo "${mouth}-1-2009"

# 显示换行
echo "OK!\n"
echo "It is a test"
# 显示不换行
echo "OK!\c"
echo "It is a test"

# 显示结果重定向至文件
echo "It is a test" > myfile

# 若需要原样输出字符串（不进行转义），请使用单引号
echo '$name\"'

# 显示命令执行结果
echo `date`

# 双引号可有可无，单引号主要用在原样输出中

########################echo########################

########################for########################

# for 变量 in 列表
# do
#     command1
#     command2
#     ...
#     commandN
# done
# 列表是一组值（数字、字符串等）组成的序列，每个值
# 通过空格分隔。每循环一次，就将列表中的下一个值赋
# 给变量。in 列表是可选的，如果不用它，for 循环使用
# 命令行的位置参数。

for loop in 1 2 3 4 5
do
	echo "The value is: $loop"
done

# 顺序输出字符串中的字符
for str in 'This is a string'
do
    echo $str
done

# 显示主目录下以 .bash 开头的文件
for FILE in $HOME/.bash*
do
   echo $FILE
done

for((i=0;i<10;i++)); # 后面有没有";"分号都没关系
do
	echo ${i}
done

########################for########################

#######################function#######################

# Shell 函数的定义格式如下：
# function_name () {
#     list of commands
#     [ return value ]
# }
# 或
# function function_name () {
#     list of commands
#     [ return value ]
# }
# 函数返回值，可以显式增加return语句；如果不加，
# 会将最后一条命令运行结果作为返回值。
# Shell 函数返回值只能是整数，一般用来表示函数执行成功
# 与否，0表示成功，其他值表示失败。
# 如果 return 其他数据，比如一个字符串，往往会得到错误
# 提示：“numeric argument required”。
# 调用函数只需要给出函数名，不需要加括号。

funWithReturn(){
    echo "The function is to get the sum of \
    two numbers..."
    echo -n "Input first number: "
    read aNum
    echo -n "Input another number: "
    read anotherNum
    echo "The two numbers are $aNum and $anotherNum !"
    return $(($aNum+$anotherNum))
}
funWithReturn
# Capture value returnd by last command
ret=$?
echo "The sum of two numbers is $ret !"
# 函数返回值在调用该函数后通过 $? 来获得。

# 像删除变量一样，删除函数也可以使用 unset 命令，
# 不过要加上 .f 选项。
$unset .f funWithReturn

# 如果希望直接从终端调用函数，可以将函数定义在主目录下
# 的.profile 文件，这样每次登录后，在命令提示符后面
# 输入函数名字就可以立即调用。
# 在Shell中，调用函数时可以向其传递参数。在函数体内部，
# 通过 $n 的形式来获取参数的值，例如，$1表示第一个参数，
# $2表示第二个参数...
funWithParam(){
    echo "The value of the first parameter is $1 !"
    echo "The value of the second parameter is $2 !"
    echo "The value of the tenth parameter is $10 !"
    echo "The value of the tenth parameter is ${10} !"
    echo "The value of the eleventh parameter is \
    ${11} !"
    # 参数个数
    echo "The amount of the parameters is $# !"  
    # 传递给函数的所有参数
    echo "The string of the parameters is $* !"  
}
funWithParam 1 2 3 4 5 6 7 8 9 34 73
# $10 不能获取第十个参数，获取第十个参数需要${10}。
# 当n>=10时，需要使用${n}来获取参数。

# $#	传递给函数的参数个数。
# $*	显示所有传递给函数的参数。
# $@	与$*相同，但是略有区别。
# $?	函数的返回值。

#######################function#######################

########################if########################

# if [ expression ]
# then
#   Statement(s) to be executed if expression is true
# fi
# expression 和方括号[ ]之间必须有空格，
# 否则会有语法错误
if [ -z ${temp} ]
then
	echo "0"
fi

if [ 1 = 1 ]
then
	echo "0"
else
	echo "1"
fi

if [ 1 != 1 ]
then
	echo "0"
elif [ 1 != 2 ]
then
	echo "0"
else
	echo "0"
fi

# if ... else 语句也可以写成一行，以命令的方式来运行
if test $[2*3] -eq $[1+5]; then echo "The two numbers 
	are equal!"; fi;

# if ... else 语句也经常与 test 命令结合使用
num1=$[2*3]
num2=$[1+5]
if test $[num1] -eq $[num2]
then
    echo 'The two numbers are equal!'
else
    echo 'The two numbers are not equal!'
fi

########################if########################

#######################operator#######################

# Bash 支持很多运算符，包括算数运算符、关系运算符、
# 布尔运算符、字符串运算符和文件测试运算符
# 原生bash不支持简单的数学运算，但是可以通过其他命令
# 来实现，例如 awk 和 expr，expr 最常用
val=`expr 2 + 2`
echo "Total value : $val"
# 表达式和运算符之间要有空格，例如 2+2 是不对的，
# 必须写成 2 + 2
# 完整的表达式要被 ` ` 包含，注意这个字符不是常用的
# 单引号，在 Esc 键下边

echo -e "\n"

a=10
b=20
val=`expr $a + $b`
echo "a + b : $val"
val=`expr $a - $b`
echo "a - b : $val"
val=`expr $a \* $b`
echo "a * b : $val"
val=`expr $b / $a`
echo "b / a : $val"
val=`expr $b % $a`
echo "b % a : $val"
if [ $a == $b ]
then
	   echo "a is equal to b"
fi

if [ $a != $b ]
then
	echo "a is not equal to b"
fi
# 乘号(*)前边必须加反斜杠(\)才能实现乘法运算
# 条件表达式要放在方括号之间，并且要有空格，
# 例如 [$a==$b] 是错误的，必须写成 [ $a == $b ]
# +	加法	`expr $a + $b` 结果为 30
# -	减法	`expr $a - $b` 结果为 10
# *	乘法	`expr $a \* $b` 结果为  200
# /	除法	`expr $b / $a` 结果为 2
# %	取余	`expr $b % $a` 结果为 0
# =	赋值	a=$b 将把变量 b 的值赋给 a
# ==	相等。用于比较两个数字，相同则返回 true。	
# [ $a == $b ] 返回 false
# !=	不相等。用于比较两个数字，不相同则返回 true。	
# [ $a != $b ] 返回 true

# 关系运算符只支持数字，不支持字符串，除非字符串的值
# 是数字
echo -e "\n"
a=10
b=20
if [ $a -eq $b ]
	then
	echo "$a -eq $b : a is equal to b"
else
	echo "$a -eq $b: a is not equal to b"
fi

if [ $a -ne $b ]
	then
	echo "$a -ne $b: a is not equal to b"
else
	echo "$a -ne $b : a is equal to b"
fi

if [ $a -gt $b ]
	then
	echo "$a -gt $b: a is greater than b"
else
	echo "$a -gt $b: a is not greater than b"
fi

if [ $a -lt $b ]
	then
	echo "$a -lt $b: a is less than b"
else
	echo "$a -lt $b: a is not less than b"
fi

if [ $a -ge $b ]
	then
	echo "$a -ge $b: a is greater or  equal to b"
else
	echo "$a -ge $b: a is not greater or equal to b"
fi

if [ $a -le $b ]
	then
	echo "$a -le $b: a is less or  equal to b"
else
	echo "$a -le $b: a is not less or equal to b"
fi
# -eq	检测两个数是否相等，相等返回 true。	
# [ $a -eq $b ] 返回 true
# -ne	检测两个数是否相等，不相等返回 true。	
# [ $a -ne $b ] 返回 true
# -gt	检测左边的数是否大于右边的，如果是，
# 则返回 true。	[ $a -gt $b ] 返回 false
# -lt	检测左边的数是否小于右边的，如果是，
# 则返回 true。	[ $a -lt $b ] 返回 true
# -ge	检测左边的数是否大于等于右边的，如果是，
# 则返回 true。	[ $a -ge $b ] 返回 false
# -le	检测左边的数是否小于等于右边的，如果是，
# 则返回 true。	[ $a -le $b ] 返回 true

echo -e "\n"

a=10
b=20
if [ $a != $b ]
	then
	echo "$a != $b : a is not equal to b"
else
	echo "$a != $b: a is equal to b"
fi

if [ $a -lt 100 -a $b -gt 15 ]
	then
	echo "$a -lt 100 -a $b -gt 15 : returns true"
else
	echo "$a -lt 100 -a $b -gt 15 : returns false"
fi

if [ $a -lt 100 -o $b -gt 100 ]
	then
	echo "$a -lt 100 -o $b -gt 100 : returns true"
else
	echo "$a -lt 100 -o $b -gt 100 : returns false"
fi

if [ $a -lt 5 -o $b -gt 100 ]
	then
	echo "$a -lt 100 -o $b -gt 100 : returns true"
else
	echo "$a -lt 100 -o $b -gt 100 : returns false"
fi
# !	非运算，表达式为 true 则返回 false，
# 否则返回 true。
# [ ! false ] 返回 true
# -o	或运算，有一个表达式为 true 则返回 true。	
# [ $a -lt 20 -o $b -gt 100 ] 返回 true
# -a	与运算，两个表达式都为 true 才返回 true。	
# [ $a -lt 20 -a $b -gt 100 ] 返回 false

echo -e "\n"

a="abc"
b="efg"
if [ $a = $b ]
	then
	echo "$a = $b : a is equal to b"
else
	echo "$a = $b: a is not equal to b"
fi

if [ $a != $b ]
	then
	echo "$a != $b : a is not equal to b"
else
	echo "$a != $b: a is equal to b"
fi

if [ -z $a ]
	then
	echo "-z $a : string length is zero"
else
	echo "-z $a : string length is not zero"
fi

if [ -n $a ]
	then
	echo "-n $a : string length is not zero"
else
	echo "-n $a : string length is zero"
fi

if [ $a ]
	then
	echo "$a : string is not empty"
else
	echo "$a : string is empty"
fi
# =		检测两个字符串是否相等，相等返回 true。	
# [ $a = $b ] 返回 false
# !=	检测两个字符串是否相等，不相等返回 true。	
# [ $a != $b ] 返回 true
# -z	检测字符串长度是否为0，为0返回 true。	
# [ -z $a ] 返回 false
# -n	检测字符串长度是否为0，不为0返回 true。	
# [ -n $a ] 返回 true
# str	检测字符串是否为空，不为空返回 true。	
# [ $a ] 返回 true

echo -e "\n"

file="/var/www/tutorialspoint/unix/test.sh"
if [ -r $file ]
	then
	echo "File has read access"
else
	echo "File does not have read access"
fi

if [ -w $file ]
	then
	echo "File has write permission"
else
	echo "File does not have write permission"
fi

if [ -x $file ]
	then
	echo "File has execute permission"
else
	echo "File does not have execute permission"
fi

if [ -f $file ]
	then
	echo "File is an ordinary file"
else
	echo "This is sepcial file"
fi

if [ -d $file ]
	then
	echo "File is a directory"
else
	echo "This is not a directory"
fi

if [ -s $file ]
	then
	echo "File size is zero"
else
	echo "File size is not zero"
fi

if [ -e $file ]
	then
	echo "File exists"
else
	echo "File does not exist"
fi
# -b file	检测文件是否是块设备文件，如果是，
# 则返回 true。	[ -b $file ] 返回 false
# -c file	检测文件是否是字符设备文件，如果是，
# 则返回 true。	[ -b $file ] 返回 false
# -d file	检测文件是否是目录，如果是，则返回 true。	
# [ -d $file ] 返回 false
# -f file	检测文件是否是普通文件（既不是目录，
# 也不是设备文件），如果是，则返回 true。	
# [ -f $file ] 返回 true
# -g file	检测文件是否设置了 SGID 位，如果是，
# 则返回 true。	[ -g $file ] 返回 false
# -k file	检测文件是否设置了粘着位(Sticky Bit)，
# 如果是，则返回 true。	[ -k $file ] 返回 false
# -p file	检测文件是否是具名管道，如果是，
# 则返回 true。	[ -p $file ] 返回 false
# -u file	检测文件是否设置了 SUID 位，如果是，
# 则返回 true。	[ -u $file ] 返回 false
# -r file	检测文件是否可读，如果是，则返回 true。	
# [ -r $file ] 返回 true
# -w file	检测文件是否可写，如果是，则返回 true。	
# [ -w $file ] 返回 true
# -x file	检测文件是否可执行，如果是，则返回 true。	
# [ -x $file ] 返回 true
# -s file	检测文件是否为空（文件大小是否大于0），
# 不为空返回 true。	[ -s $file ] 返回 true
# -e file	检测文件（包括目录）是否存在，如果是，
# 则返回 true。	[ -e $file ] 返回 true

#######################operator#######################

########################read########################

# 命令输出重定向的语法为：$ command > file
# 输出重定向会覆盖文件内容
# 如果不希望文件内容被覆盖，可以使用 >> 追加到文件末尾

# 和输出重定向一样，Unix 命令也可以从文件获取输入，
# 语法为：command < file

# 一般情况下，每个 Unix/Linux 命令运行时都会打开三个
# 文件：
# 标准输入文件(stdin)：stdin的文件描述符为0，Unix程序
# 默认从stdin读取数据
# 标准输出文件(stdout)：stdout 的文件描述符为1，
# Unix程序默认向stdout输出数据
# 标准错误文件(stderr)：stderr的文件描述符为2，
# Unix程序会向stderr流中写入错误信息

# 如果希望 stderr 重定向到 file，可以这样写：
# $command 2 > file
# 如果希望 stderr 追加到 file 文件末尾，可以这样写：
# $command 2 >> file
# 如果希望将 stdout 和 stderr 合并后重定向到 file，
# 可以这样写：$command > file 2>&1
# 或 $command >> file 2>&1
# 如果希望对 stdin 和 stdout 都重定向，可以这样写：
# $command < file1 >file2
# command 命令将 stdin 重定向到 file1，将 stdout 
# 重定向到 file2

# command > file	将输出重定向到 file
# command < file	将输入重定向到 file
# command >> file	将输出以追加的方式重定向到 file
# n > file	将文件描述符为 n 的文件重定向到 file
# n >> file	将文件描述符为 n 的文件以追加的方式重定向
# 到 file
# n >& m	将输出文件 m 和 n 合并
# n <& m	将输入文件 m 和 n 合并
# << tag	将开始标记 tag 和结束标记 tag 之间的内容
# 作为输入

# Here Document 目前没有统一的翻译，这里暂译为”嵌入
# 文档“。Here Document 是 Shell 中的一种特殊的重定向
# 方式，它的基本的形式如下：
# command << delimiter
#     document
# delimiter
# 它的作用是将两个 delimiter 之间的内容(document) 
# 作为输入传递给 command。

# 结尾的delimiter 一定要顶格写，前面不能有任何字符，
# 后面也不能有任何字符，包括空格和 tab 缩进。
# 开始的delimiter前后的空格会被忽略掉。
wc -l << EOF
	This is a simple lookup program
	for good (and bad) restaurants
	in Cape Town.
EOF

echo -e "\n"

cat << EOF
	This is a simple lookup program
	for good (and bad) restaurants
	in Cape Town.
EOF

echo -e "\n"

# 下面的脚本通过 vi 编辑器将 document 保存到 
# test.txt 文件：
filename=test.txt
vi $filename << EndOfCommands
i
This file was created automatically from
a shell script
^[
ZZ
EndOfCommands

# 如果希望执行某个命令，但又不希望在屏幕上显示
# 输出结果，那么可以将输出重定向到 
# /dev/null：command > /dev/null
# /dev/null 是一个特殊的文件，写入到它的内容都会
# 被丢弃；如果尝试从该文件读取内容，那么什么也
# 读不到。但是 /dev/null 文件非常有用，将命令的
# 输出重定向到它，会起到”禁止输出“的效果

# 如果希望屏蔽 stdout 和 stderr，可以这样写：
# command > /dev/null 2>&1

########################read########################

#######################redirect#######################

# 命令输出重定向的语法为：$ command > file
# 输出重定向会覆盖文件内容
# 如果不希望文件内容被覆盖，可以使用 >> 追加到文件末尾

# 和输出重定向一样，Unix 命令也可以从文件获取输入，
# 语法为：command < file

# 一般情况下，每个 Unix/Linux 命令运行时都会打开三个
# 文件：
# 标准输入文件(stdin)：stdin的文件描述符为0，
# Unix程序默认从stdin读取数据
# 标准输出文件(stdout)：stdout 的文件描述符为1，
# Unix程序默认向stdout输出数据
# 标准错误文件(stderr)：stderr的文件描述符为2，
# Unix程序会向stderr流中写入错误信息

# 如果希望 stderr 重定向到 file，可以这样写：
# $command 2 > file
# 如果希望 stderr 追加到 file 文件末尾，可以这样写：
# $command 2 >> file
# 如果希望将 stdout 和 stderr 合并后重定向到 file，
# 可以这样写：$command > file 2>&1
# 或 $command >> file 2>&1
# 如果希望对 stdin 和 stdout 都重定向，可以这样写：
# $command < file1 >file2
# command 命令将 stdin 重定向到 file1，将 stdout 
# 重定向到 file2

# command > file	将输出重定向到 file
# command < file	将输入重定向到 file
# command >> file	将输出以追加的方式重定向到 file
# n > file	将文件描述符为 n 的文件重定向到 file
# n >> file	将文件描述符为 n 的文件以追加的方式
# 重定向到 file
# n >& m	将输出文件 m 和 n 合并
# n <& m	将输入文件 m 和 n 合并
# << tag	将开始标记 tag 和结束标记 tag 之间的内容
# 作为输入

# Here Document 目前没有统一的翻译，这里暂译为
# ”嵌入文档“。Here Document 是 Shell 中的一种
# 特殊的重定向方式，它的基本的形式如下：
# command << delimiter
#     document
# delimiter
# 它的作用是将两个 delimiter 之间的内容(document) 
# 作为输入传递给 command。

# 结尾的delimiter 一定要顶格写，前面不能有任何字符，
# 后面也不能有任何字符，包括空格和 tab 缩进。
# 开始的delimiter前后的空格会被忽略掉。
wc -l << EOF
	This is a simple lookup program
	for good (and bad) restaurants
	in Cape Town.
EOF

echo -e "\n"

cat << EOF
	This is a simple lookup program
	for good (and bad) restaurants
	in Cape Town.
EOF

echo -e "\n"

# 下面的脚本通过 vi 编辑器将 document 保存到 
# test.txt 文件：
filename=test.txt
vi $filename << EndOfCommands
i
This file was created automatically from
a shell script
^[
ZZ
EndOfCommands

# 如果希望执行某个命令，但又不希望在屏幕上显示
# 输出结果，那么可以将输出重定向到 
# /dev/null：command > /dev/null
# /dev/null 是一个特殊的文件，写入到它的内容都会
# 被丢弃；如果尝试从该文件读取内容，那么什么也读不到。
# 但是 /dev/null 文件非常有用，将命令的输出重定向
# 到它，会起到”禁止输出“的效果

# 如果希望屏蔽 stdout 和 stderr，可以这样写：
# command > /dev/null 2>&1

#######################redirect#######################

#######################replace#######################

a=10
echo "Value of a is $a"
# 这里 表示对转义字符进行替换。如果不使用 选项，
# 将会原样输出：
# Value of a is 10

# 下面的转义字符都可以用在 echo 中：
#	反斜杠
# \a	警报，响铃
# \b	退格（删除键）
# \f	换页(FF)，将当前位置移到下页开头
#	换行
# \r	回车
# \t	水平制表符（tab键）# \v	垂直制表符 
# 可以使用 echo 命令的 -E 选项禁止转义，默认
# 也是不转义的；使用 -n 选项可以禁止插入换行符

# 命令替换是指Shell可以先执行命令，将输出结果
# 暂时保存，在适当的地方输出
DATE=`date`
echo "Date is $DATE"
USERS=`who | wc -l`
echo "Logged in user are $USERS"
UP=`date ; uptime`
echo "Uptime is $UP"

# 变量替换可以根据变量的状态（是否为空、是否定义等）
# 来改变它的值
# ${var}	变量本来的值
# ${var:-word}	如果变量 var 为空或已被删除(unset)，
# 那么返回 word，但不改变 var 的值。
# ${var:=word}	如果变量 var 为空或已被删除(unset)，
# 那么返回 word，并将 var 的值设置为 word。
# ${var:?message}	如果变量 var 为空或已被
# 删除(unset)，那么将消息 message 送到标准错误输出，
# 可以用来检测变量 var 是否可以被正常赋值。若此替换
# 出现在Shell脚本中，那么脚本将停止运行。
# ${var:+word}	如果变量 var 被定义，那么返回 word，
# 但不改变 var 的值。
echo -e "\n"

echo -e ${var:-"Variable is not set"}
echo -e "1 - Value of var is ${var} \n"

echo -e ${var:="Variable is not set"}
echo -e "2 - Value of var is ${var} \n"

unset var
echo -e ${var:+"This is default value"}
echo -e "3 - Value of var is $var \n"

var="Prefix"
echo -e ${var:+"This is default value"}
echo -e "4 - Value of var is $var \n"

echo -e ${var:?"Print this message"}
echo -e "5 - Value of var is ${var} \n"

#######################replace#######################

########################shell########################

IFS_old=${IFS}
IFS=$'\n' # 原来是以"空格"进行分隔的

result=`cat teraterm2.log | grep -i -e 
${action_down} -e ${constructor} -e ${end} -e ${mute}`
# ...

IFS=${IFS_old}

adb -s e79564213584621542 shell input keyevent 
KEYCODE_POWER

currentTime=`date +"%y/%m/%d %H:%M:%S"`

# 操作数与运算符之间不能有空格
# 两层括号
let result=$((${action_down4}*60*1000
+${action_down6}*1000+${action_down7}))
# 操作数与运算符之间必须要有空格
result=`expr ( ${mute5} * 60 * 1000 ) 
+ ( {mute6} * 1000 ) + ( ${mute7} )`

if [ ${temp} =~ ${action_down} ]
then
	# ${temp} 包含 ${action_down}
fi

# 提取action字符串从"0"位置开始的2个字符
temp=${action:0:2}

for(( ; ; ));
do
	sleep 2
done

while :
do
	sleep 2
done

action_down3=`echo ${action_down2} | 
awk -F ' ' '{print $2}'`
action_down4=`echo ${action_down3} | 
awk -F ':' '{print $2}'`

# 1) 局部变量
# 局部变量在脚本或命令中定义，仅在当前shell实例中有效，
# 其他shell启动的程序不能访问局部变量。
# 2) 环境变量
# 所有的程序，包括shell启动的程序，都能访问环境变量，
# 有些程序需要环境变量来保证其正常运行。必要的时候
# shell脚本也可以定义环境变量。
# 3) shell变量
# shell变量是由shell程序设置的特殊变量。shell变量中
# 有一部分是环境变量，有一部分是局部变量，这些变量
# 保证了shell的正常运行

# $0	当前脚本的文件名。
# $n	传递给脚本或函数的参数。n 是一个数字，
# 表示第几个参数。例如，第一个参数是$1，
# 第二个参数是$2。
# $#	传递给脚本或函数的参数个数。
# $*	传递给脚本或函数的所有参数。
# $@	传递给脚本或函数的所有参数。被双引号(" ")
# 包含时，与 $* 稍有不同，下面将会讲到。
# $?	上个命令的退出状态，或函数的返回值。
# $$	当前Shell进程ID。对于 Shell 脚本，
# 就是这些脚本所在的进程ID。

# 下面的转义字符都可以用在 echo 中：
# \\	反斜杠
# \a	警报，响铃
# \b	退格（删除键）
# \f	换页(FF)，将当前位置移到下页开头
# \n	换行
# \r	回车
# \t	水平制表符（tab键） 
# \v	垂直制表符 可以使用 echo 命令的 -E 
# 选项禁止转义，默认也是不转义的；使用 -n 选项可以
# 禁止插入换行符


# ${var}	变量本来的值
# ${var:-word}	如果变量 var 为空或已被删除(unset)，
# 那么返回 word，但不改变 var 的值。
# ${var:=word}	如果变量 var 为空或已被删除(unset)，
# 那么返回 word，并将 var 的值设置为 word。
# ${var:?message}	如果变量 var 为空或
# 已被删除(unset)，那么将消息 message 送到标准错误
# 输出，可以用来检测变量 var 是否可以被正常赋值。
# 若此替换出现在Shell脚本中，那么脚本将停止运行。
# ${var:+word}	如果变量 var 被定义，那么返回 word，
# 但不改变 var 的值。

# +	加法	`expr $a + $b` 结果为 30
# -	减法	`expr $a - $b` 结果为 10
# *	乘法	`expr $a \* $b` 结果为  200
# /	除法	`expr $b / $a` 结果为 2
# %	取余	`expr $b % $a` 结果为 0
# =	赋值	a=$b 将把变量 b 的值赋给 a
# ==	相等。用于比较两个数字，相同则返回 true。	
# [ $a == $b ] 返回 false
# !=	不相等。用于比较两个数字，
# 不相同则返回 true。	
# [ $a != $b ] 返回 true

# -eq	检测两个数是否相等，相等返回 true。	
# [ $a -eq $b ] 返回 true
# -ne	检测两个数是否相等，不相等返回 true。	
# [ $a -ne $b ] 返回 true
# -gt	检测左边的数是否大于右边的，如果是，
# 则返回 true。	[ $a -gt $b ] 返回 false
# -lt	检测左边的数是否小于右边的，如果是，
# 则返回 true。	[ $a -lt $b ] 返回 true
# -ge	检测左边的数是否大等于右边的，如果是，
# 则返回 true。	[ $a -ge $b ] 返回 false
# -le	检测左边的数是否小于等于右边的，如果是，
# 则返回 true。	[ $a -le $b ] 返回 true

# !	非运算，表达式为 true 则返回 false，
# 否则返回 true。	[ ! false ] 返回 true
# -o	或运算，有一个表达式为 true 则返回 true。	
# [ $a -lt 20 -o $b -gt 100 ] 返回 true
# -a	与运算，两个表达式都为 true 才返回 true。	
# [ $a -lt 20 -a $b -gt 100 ] 返回 false

# =	检测两个字符串是否相等，相等返回 true。	
# [ $a = $b ] 返回 false
# !=	检测两个字符串是否相等，不相等返回 true。	
# [ $a != $b ] 返回 true
# -z	检测字符串长度是否为0，为0返回 true。	
# [ -z $a ] 返回 false
# -n	检测字符串长度是否为0，不为0返回 true。	
# [ -z $a ] 返回 true
# str	检测字符串是否为空，不为空返回 true。	
# [ $a ] 返回 true

# -b file	检测文件是否是块设备文件，如果是，
# 则返回 true。	[ -b $file ] 返回 false
# -c file	检测文件是否是字符设备文件，如果是，
# 则返回 true。	[ -b $file ] 返回 false
# -d file	检测文件是否是目录，如果是，则返回 true。	
# [ -d $file ] 返回 false
# -f file	检测文件是否是普通文件（既不是目录，
# 也不是设备文件），如果是，则返回 true。	
# [ -f $file ] 返回 true
# -g file	检测文件是否设置了 SGID 位，如果是，
# 则返回 true。	[ -g $file ] 返回 false
# -k file	检测文件是否设置了粘着位(Sticky Bit)，
# 如果是，则返回 true。	[ -k $file ] 返回 false
# -p file	检测文件是否是具名管道，如果是，
# 则返回 true。	[ -p $file ] 返回 false
# -u file	检测文件是否设置了 SUID 位，如果是，
# 则返回 true。	[ -u $file ] 返回 false
# -r file	检测文件是否可读，如果是，则返回 true。	
# [ -r $file ] 返回 true
# -w file	检测文件是否可写，如果是，则返回 true。	
# [ -w $file ] 返回 true
# -x file	检测文件是否可执行，如果是，则返回 true。	
# [ -x $file ] 返回 true
# -s file	检测文件是否为空（文件大小是否大于0），
# 不为空返回 true。	[ -s $file ] 返回 true
# -e file	检测文件（包括目录）是否存在，如果是，
# 则返回 true。	[ -e $file ] 返回 true

# 一般情况下，每个 Unix/Linux 命令运行时都会打开
# 三个文件：
# 标准输入文件(stdin)：stdin的文件描述符为0，
# Unix程序默认从stdin读取数据
# 标准输出文件(stdout)：stdout 的文件描述符为1，
# Unix程序默认向stdout输出数据
# 标准错误文件(stderr)：stderr的文件描述符为2，
# Unix程序会向stderr流中写入错误信息



# command > file	将输出重定向到 file
# command < file	将输入重定向到 file
# command >> file	将输出以追加的方式重定向到 file
# n > file	将文件描述符为 n 的文件重定向到 file
# n >> file	将文件描述符为 n 的文件以追加的方式重定向
# 到 file
# n >& m	将输出文件 m 和 n 合并
# n <& m	将输入文件 m 和 n 合并
# << tag	将开始标记 tag 和结束标记 tag 之间的内容
# 作为输入

# Here Document 目前没有统一的翻译，这里暂译为
# ”嵌入文档“。Here Document 是 Shell 中的一种特殊
# 的重定向方式，它的基本的形式如下：
# command << delimiter
#     document
# delimiter
# 它的作用是将两个 delimiter 之间的内容(document) 
# 作为输入传递给 command。

# 结尾的delimiter 一定要顶格写，前面不能有任何字符，
# 后面也不能有任何字符，包括空格和 tab 缩进。
# 开始的delimiter前后的空格会被忽略掉。

# C语言printf()函数的不同点：
# printf 命令不用加括号
# format-string 可以没有引号，但最好加上，单引号
# 双引号均可
# 参数多于格式控制符(%)时，format-string 可以重用，
# 可以将所有参数都转换
# arguments 使用空格分隔，不用逗号

# format-string为双引号
printf "%d %s\n" 1 "abc"
# 单引号与双引号效果一样 
printf '%d %s\n' 1 "abc" 
# 没有引号也可以输出
printf %s abcdef
# 格式只指定了一个参数，但多出的参数仍然会按照该
# 格式输出，format-string 被重用
printf %s abc def
printf "%s\n" abc def
printf "%s %s %s\n" a b c d e f g h i j
# 如果没有 arguments，那么 %s 用NULL代替，%d 用 0 代替
printf "%s and %d \n"
# 如果以 %d 的格式来显示字符串，那么会有警告，
# 提示无效的数字，此时默认置为 0
printf "The first program always 
prints'%s,%d\n'" Hello Shell

# if [ expression ]
# then
#   Statement(s) to be executed if expression is true
# fi

# case 值 in
# 模式1)
#     command1
#     command2
#     command3
#     ;;
# 模式2）
#     command1
#     command2
#     command3
#     ;;
# *)
#     command1
#     command2
#     command3
#     ;;

########################shell########################

########################string########################

# 字符串是shell编程中最常用最有用的数据类型
# （除了数字和字符串，也没啥其它类型好用了），
# 字符串可以用单引号，也可以用双引号，也可以不用引号。
# 单引号里的任何字符都会原样输出，单引号字符串中
# 的变量是无效的
# 单引号字符串中不能再出现单引号（对单引号使用转
# 义符后也不行）
str='this is a string'
# 双引号里可以有变量
# 双引号里可以出现转义字符
your_name='qinjx'
str="Hello, I know your are \"$your_name\"! \n"
echo ${str}

echo -e "\n"
# 拼接字符串
your_name="qinjx"
greeting="hello, "$your_name" !"
greeting_1="hello, ${your_name} !"
echo $greeting $greeting_1

echo -e "\n"
# 获取字符串长度
string="abcd"
echo ${#string} #输出 4

echo -e "\n"
# 提取子字符串
string="alibaba is a great company"
echo ${string:1:4} #输出liba

echo -e "\n"
# 查找子字符串
string="alibaba is a great company"
echo `expr index "$string" is`

########################string########################

########################until########################

# until 循环执行一系列命令直至条件为 true 时停止。
# until command
# do
#    Statement(s) to be executed until command is true
# done
# command 一般为条件表达式，如果返回值为 false，
# 则继续执行循环体内的语句，否则跳出循环。

a=0
until [ ! $a -lt 10 ]
do
   echo $a
   a=`expr $a + 1`
done

########################until########################

#######################variable#######################

# 定义变量时，变量名不加美元符号（$）
# 使用变量的时候才加美元符（$）

# 变量名的命名须遵循如下规则：
# 变量名和等号之间不能有空格
# 首个字符必须为字母（a-z，A-Z）
# 中间不能有空格，可以使用下划线（_）
# 不能使用标点符号
# 不能使用bash里的关键字（可用help命令查看保留关键字）

# 推荐给所有变量加上花括号，这是个好的编程习惯
myUrl="http://see.xidian.edu.cn/cpp/linux/"
myNum=100
your_name="mozhiyan"
echo $your_name
echo ${your_name}

# 使用 readonly 命令可以将变量定义为只读变量，
# 只读变量的值不能被改变
# 第一次运行时没有问题,第二次运行时会提示myUrl是
# 只读变量.因为这个shell中已经有这个只读变量了,
# 第二次运行时,相当于在修改这个值了,所以会有提示
myUrl="http://see.xidian.edu.cn/cpp/shell/"
readonly myUrl

readonly myPath="http://192.168.1.1"
# error
# myPath="http" 

# 使用 unset 命令可以删除变量
# 变量被删除后不能再次使用；unset 命令不能删除只读变量
unset variable_name

# 运行shell时，会同时存在三种变量：
# 1) 局部变量
# 局部变量在脚本或命令中定义，仅在当前shell实例中有效，
# 其他shell启动的程序不能访问局部变量。
# 2) 环境变量
# 所有的程序，包括shell启动的程序，都能访问环境变量，
# 有些程序需要环境变量来保证其正常运行。必要的时候
# shell脚本也可以定义环境变量。
# 3) shell变量
# shell变量是由shell程序设置的特殊变量。shell变量中
# 有一部分是环境变量，有一部分是局部变量，这些变量
# 保证了shell的正常运行。

#######################variable#######################

###################variable-special###################

# $0	当前脚本的文件名。
# $n	传递给脚本或函数的参数。n 是一个数字，
# 表示第几个参数。例如，第一个参数是$1，
# 第二个参数是$2。
# $#	传递给脚本或函数的参数个数。
# $*	传递给脚本或函数的所有参数。
# $@	传递给脚本或函数的所有参数。被双引号(" ")
# 包含时，与 $* 稍有不同，下面将会讲到。
# $?	上个命令的退出状态，或函数的返回值。所谓退出
# 状态，就是上一个命令执行后的返回结果。退出状态是
# 一个数字，一般情况下，大部分命令执行成功会返回 0，
# 失败返回 1。
# $$	当前Shell进程ID。对于 Shell 脚本，就是这些
# 脚本所在的进程ID。

# 运行脚本时传递给脚本的参数称为命令行参数。命令行
# 参数用 $n 表示，例如，$1 表示第一个参数，$2 表示
# 第二个参数，依次类推。
echo "File Name: $0"
echo "First Parameter : $1"
echo "First Parameter : $2"
echo "Quoted Values: $@"
echo "Quoted Values: $*"
echo "Total Number of Parameters : $#"

# $* 和 $@ 都表示传递给函数或脚本的所有参数，
# 不被双引号(" ")包含时，都以"$1" "$2" … "$n" 
# 的形式输出所有参数。
# 但是当它们被双引号(" ")包含时，"$*" 会将所有
# 的参数作为一个整体，以"$1 $2 … $n"的形式输出
# 所有参数；"$@" 会将各个参数分开，以"$1" "$2" … "$n" 
# 的形式输出所有参数。

# 用这样的命令测试下面的结果:
# source variable-special.sh "a" "b" "c" "d"
echo "\$*=" $*
echo "\"\$*\"=" "$*"
echo "\$@=" $@
echo "\"\$@\"=" "$@"
echo "print each param from \$*"
for var in $*
do
    echo "$var"
done

echo "print each param from \$@"
for var in $@
do
    echo "$var"
done

echo "print each param from \"\$*\""
for var in "$*"
do
    echo "$var"
done

echo "print each param from \"\$@\""
for var in "$@"
do
    echo "$var"
done

##################variable-special##################

########################while########################

# while command
# do
#    Statement(s) to be executed if command is true
# done

COUNTER=0
while [ $COUNTER -lt 5 ]
do
    COUNTER='expr $COUNTER + 1'
    echo $COUNTER
done

echo -e "\n"

echo 'type <CTRL-D> to terminate'
echo -n 'enter your most liked film: '
while read FILM
do
    echo "Yeah! great film the $FILM"
done

while :
do
	toDo;
done

########################while########################

file test查看可执行文件test具体信息


# shell中常用的语法
# run start
IFS_old=${IFS}
IFS=$'\n'

result=`cat -n ${log_name}`
for aline in ${result};
do
	
done

IFS=${IFS_old}

# 注意：expression(表达式) 和方括号[ ]之间必须有空格，
# 否则会有语法错误。
if [ $a == $b ]; then # [ $a != $b ]
   echo "a is equal to b"
fi

if [ $a == $b ]; then
   echo "a is equal to b"
elif [ $a -gt $b ]; then
   echo "a is greater than b"
else
   echo "None of the condition met"
fi

if test $[2*3] -eq $[1+5]; then echo 
	'The two numbers are equal!'; fi;

num1=$[2*3]
num2=$[1+5]
if test $[num1] -eq $[num2]; then
    echo 'The two numbers are equal!'
else
    echo 'The two numbers are not equal!'
fi

if (( a > b )) && (( a < c ))
if [[ $a > $b ]] && [[ $a < $c ]] 
if [ $a -gt $b -a $a -lt $c ] 

if (( a > b )) || (( a < c ))
if [[ $a > $b ]] || [[ $a < $c ]] 
if [ $a -gt $b -o $a -lt $c ]

# 把@@@全部换成,
test_result=`echo ${test_result} | sed 's/@@@/,/g'` 
if [[ ${test_result} =~ "@@@" ]] # 字符串包含
# 先按@@@分隔,然后取分隔后的字符串中的第一个
line_temp=`echo ${right_temp} | 
awk -F '@@@' '{print $1}'` 
# 取第一个字符串,然后判断这个字符串跟"0"是否相等
if [[ ${action_down4:0:1} -eq "0" ]]; then 
	action_down4=`echo $action_down4 | cut -c 2`
	action_down7=`echo $action_down7 | cut -c 2-3`
fi
# 语法格式temp=$((需要计算的表达式))
let mute8=$((${mute4}*60*1000+${mute6}*1000+${mute7})) 

将iris.csv文件中“setosa”更改为“iris-setosa”
sed 's/setosa/iris-setosa/g' iris.csv > output.csv

${varible##*string} 从左向右截取最后一个string后的字符串
${varible#*string}  从左向右截取第一个string后的字符串
${varible%%string*} 从右向左截取最后一个string后的字符串
${varible%string*}  从右向左截取第一个string后的字符串
“*”只是一个通配符有时可以不要
例子：
$ MYVAR=foodforthought.jpg
$ echo ${MYVAR##*fo}
rthought.jpg
$ echo ${MYVAR#*fo}
odforthought.jpg

# /bin/sh是一个链接到/bin/bash的符号链接文件
file /bin/sh

# 清空文件中的内容，文件本身还是存在着的
cp /dev/null /mnt/d/myfiles/test.txt

使用openssh的登录法有3种：
1.
# ssh -l weidi 192.168.1.2
ssh -l 账号 主机名或IP
2.
# ssh weidi@192.168.1.2
ssh 账号@主机名或IP
3.
# ssh 192.168.1.2
ssh 主机名或IP

# *:代表任意的字符串，可以是空字符串
# ?:代表一个字符，但不可以为空
# 一对单引号中，不可以出现单引号，就算是转义字符\也不行
# 使用转义字符并去掉最外围的一对单引号，或改用双引号，
# 或使用连续的方式
echo This is Jack\'s book.
echo "This is Jack's book."
echo 'This is Jack'\''s book.'

# 下面的符号都具有特殊的含义，如果要显示这些字符的本身，
# 必须在其前面加上转义字符\
# ` ~ \ | ' " / ? ( ) [ ] { } < > ! # $ & * ; 

# 续行符号：\的后方，若接续的是换行字符（不可见字符），
# 即先键入\，再按“Enter”键，
# 则bash会将它视为“续行字符”，因此，上下输入的两行，
# 将视为一行
echo "line 1 \
lint 1 too"

# [a-z]: 代表26个英文字母中的小写字母中的一个
# [A-Z]: 大写中的其中一个
# [a-zA-Z]: 大小写中的其中一个
# [0-9]: 数字中的其中一个
# [a-zA-Z0-9]: 英文或者数字中的其中一个
# [xyz]: x或y或z

# 由于-字符代表字符集合的范围，因此，如果要把-也作为
# 集合的一份子，需要把-放置在集合
# 的开头或结尾
# [a-z_-]: 代表英文小写字母、下画线，以及-字符

# [!0-9]: 不是数字
# [!a-z]: 不是英文小写字母
# [!A-Z]: 不这英文大写字母
# [\!0-9]: 代表惊叹号或数字

# {g,nc,s}ftp: 代表gftp, ncftp, sftp
# sources.list{,.bak}: 
# 代表sources.list, sources.list.bak

# 每一个Shell Script执行时（称为“进程”或“处理程序”），
# 系统默认会开启3个标准的文件，分别是：
# 标准输入（stdin）、标准输出（stdout）及标准错误
# （stderr）。
# Script默认会由标准输入读取数据，通常是指键盘；
# 而由标准输出显示执行的结果，通常是指
# 屏幕；若有错误发生，则由标准错误显示信息，通常
# 也是屏幕。
# 系统开启这3个标准文件时，是以文件代码做连接。
# 标准输入的文件代码为0，标准输出的文件代码
# 为1，标准错误的文件代码为2.我们称这3文件为
# 标准I/O（Input/Output）。

cat hi.txt
more hi.txt
less hi.txt

currentTime=`date "+%Y-%m-%d %H:%M:%S"`
currentTime=`date +"%y/%m/%d %H:%M:%S"`
currentTimeStamp=`date -d "$currentTime" +%s`
echo $currentTimeStamp

grep keydl -rn *

adb kill-server && adb start-server 
&& adb root && adb remount
# mSurface=Surface(name=com.tencent.mm/
# com.tencent.mm.ui.LauncherUI)
adb shell dumpsys window w | grep \/ | grep name=
adb -s 24292001zzzzzzz shell am start -n 
com.sony.dtv.osat.video/com.sonyericsson.
video.browser.BrowserActivity
adb -s 24292001zzzzzzz shell input tap 630 1210
adb -s 24292001zzzzzzz shell input keyevent KEYCODE_HOME
adb shell input text "hello,world"
KEYCODE_POWER
KEYCODE_HOME
KEYCODE_BACK
KEYCODE_DPAD_CENTER
KEYCODE_DPAD_UP
KEYCODE_DPAD_DOWN
KEYCODE_DPAD_LEFT
KEYCODE_DPAD_RIGHT
#覆盖安装apk
adb install -r ${currentDir}/MySony-release.apk;
#卸载apk
adb uninstall ${packageName};
adb shell pm clear ${packageName};
adb shell am force-stop ${packageName};
adb shell svc wifi enable;
adb shell svc wifi disable;
# 查看安装应用的包名
adb shell pm list packages
# 找到apk的位置
adb shell pm path com.miui.fm

adb shell ls -alh /storage/3D5F-08A6/SonyTVCamera_photo
./system/vendor/bin/camera_recognition_daemon &

# 停用应用
adb shell pm hide   com.ss.android.article.video
# 启用应用
adb shell pm unhide com.ss.android.article.video
# 启动隐式Activity
adb shell am start -a android.settings.LOCALE_SETTINGS

#由App自行决定软件能否安装在SD卡 
pm set-Install-Location 0 
#强制全部App安装在ROM内 
pm set-Install-Location 1 
#强制全部App安装在SD卡 
pm set-Install-Location 2 

# 清理日志
adb logcat -c;
# 把字符串当命令使
local parameter2=$2;	
# adb logcat > ${currentDir}/${mySonyTempLog} &
adb logcat > ${currentDir}/${parameter2}".log" &

adb shell pm grant packagename android.permission.CHANGE_CONFIGURATION

adb shell am broadcast -a com.android.test --es test_string "this is test string" --ei test_int 100 --ez test_boolean true

adb shell sm mount VOLUME
adb shell sm unmount VOLUME

# test='u0_a87    509   413   1191524 69184 
# SyS_epoll_ 00f6fd7294 S com.qiyi.video'
processName=`echo ${aline} | awk  -F ' ' '{print $9}'`
# mytest="abbsdfk    asdkf  slkdf" 一个或多个空格
echo ${mytest} | awk  -F '[ ;]+' '{print $2}'
# 后面有特殊字符
processName=`echo ${processName} | 
awk  -F '\r' '{print $1}'`

result=`ping -c 5 -w 100 www.baidu.com`
echo "${result}"

host="192.168.0.100"
ping -c2 $host >>/dev/null
ping -c5 "www.baidus.com" >>/dev/null

if [[ $? -eq 0 ]]; then
    echo “network is ok,exit...”
else
    echo “network is down,now restart network services”
fi

if [[ ${whiteList} != *${processName2}* ]]; then
fi
if [[ ${processName} == *":"* ]]; then
fi

if [[ -n ${parameter2} ]]; then
	eval ${parameter2};
fi

result=`cat result.txt | grep -w -e '0' | 
tail -n 2 | head -n 1`

# 计算行数
wc -l < input.txt
sort < unsort.txt > sorted.txt
# lp：打印机程序
cat sorted.txt | lp

# 开启一个子Shell的方式运行Script
./test.sh
/root/tmp/test.sh
base test.sh
sh test.sh
# 在现行的Shell中运行Script
. /root/tmp/test.sh # "."与"/"之间有空格
source /root/tmp/test.sh

# 执行结果：显示所有内置命令的列表。
help      
# 显示内置命令的语法格式。
help -s 内置命令  

# 用来显示一行文字。后面的字符串可以用单引号或双引号
# 括起来。
echo 
# 默认会自动换行。如果不想让它自动换行，
# 可加上-n的选项
echo -n 'Hello world'
# 选项-e可让字符串中的特殊字符有特殊作用。
# 如\n为换行
echo -e "I am the king\nof the world."

# 执行结果：回到先前的目录
cd -

# 显示目前的工作目录
pwd

# 利用 : ,建立一个空文件
: > test.txt

# 在现行的Shell环境中执行Shell程序，之间要有空格。
# .和source命令的作用相同。
. myfun.sh
source myfun.sh

# 20171128102448年月日时分秒
date +%Y%m%d%H%M%S

# 显示、设定程序别名。执行结果：显示目前所有已经设定
# 的程序别名。
alias
# 执行结果：设定程序新的别名
alias 新的别名='组合的命令'
# 表示把cp用cp -f重新定义，执行cp即等于执行cp -f。
alias cp='cp -f'
# 把cp的别名取消，cp还是原来的cp。
alias cp=cp
# 表示把cp的别名设定取消。
unalias cp

# 离开Shell，并传回1的值。
exit 1
# 注销login Shell
logout
# 在命令行没有输入任何字符串的情况下，
# 若按Ctrl+D组合键，其功能等同于logout指令，
# 一样可以注销。

# 显示或设定：文件、目录建立时，显示文件权限的屏蔽值。
# 显示0022.
umask
# umask '8进制的权限值'
# 执行结果：设定建立新文件时，文件权限的屏蔽值。
# 通常在Linux/BSD等系统建立新的文件时，默认的
# 权限值是0666；
# 在建立新的目录时，默认的权限值是0777.不过，这样的
# 权限值并不安全。
# 为了让文件管理者能方便地开设文件及目录，又不必担心
# 权限的安全性问题，
# 因此，才有umask的机制。
umask 022
# 这表示新的文件建立时，其真正的权限应该是
# 0666-0022=0644，这样一来，
# 群组及其他人对这个新建文件都只有读取权，
# 而没有写入（修改）权；
# 新的目录建立时，其权限则是0777-0022=0755，
# 群组及其他人对
# 这个新建目录都只有查看和进入权，而不能对目录中
# 的文件进行删除、改名，或在其中建立新文件。
# 有了umask的机制，新建文件和目录，会比较安全一点。
# 因此，我们称umask为文件的安全滤网。

# 显示过去曾经执行过的Shell指令。
history
# 与history有关的变量有3个：
# 1.HISTFILE
# 执行结果：/root/.bash_history
echo $HISTFILE
# 这表示，该文件为root的历史脚本文件，也就是说，
# root执行过的指令，便是存储在这个文件中。
# 2.HISTFILESIZE
# 但并不是所有的历史指令都会全部保存进去。
# HISTFILESIZE这个变量，用来指定历史脚本
# 文件的行数大小，若历史指令的数目超过
# HISTFILESIZE的大小，排序在前的指令会被剔除掉，
# 但各历史指令的序号不变。
# 执行结果：500.
echo $HISTFILESIZE
# 这表示：若最后一个历史指令的编号为550的话，
# 那么.bash_history文件只存储编号51~550的
# 历史指令，编号1~50的指令会被删除。
# 3.HISTSIZE
# HISTSIZE设定在一个互动模式的Shell中，可记住的
# 历史指令数目。一旦该Shell结束，
# 历史脚本文件仅存入HISTSIZE行数的指令，但如果
# HISTFILESIZE比HISTSIZE小，
# 则历史脚本文件的行数上限以HISTFILESIZE的数目为主。

# 列出登录主机之后最近执行过的指令。
fc -l
# 执行结果：列出“大于或等于”指定编号的历史命令。
fc -l 497
# 执行结果：列出指定编号之间的历史命令。
fc -l 497 499
# 执行结果：列出两个关键记事（./up-cvs和sh）之间的
# 历史命令。
fc -l ./up-cvs wh
# 注意，在列出指令范围时，历史指令的编号和关键词
# 可以混用。如fc -l 497 sh
# 另外，若加上-n选项，则不会显示命令的编号。如fc -ln
# 若加上-r选项，则会以相反的顺序（编号由大到小）
# 显示命令。

# type 判断bash对某一指令如何解译。
# type fg:fg is a Shell builtin,表明fg是一个内置命令。
# type if:if is a Shell keywork,表明if是bash保留
# 的关键词。
# type ll:ll is aliased to 'ls -l --color=auto',
# 表明ll是一个程序别名。
# type show_name:show_name is a function,
# 表明show_name是一个Shell函数。
# type tr:tr is /usr/bin/tr,表明tr是一个
# 独立的执行文件，文件路径在/usr/bin/tr。

# set 设定Bash Shell的属性；若不加任何选项和参数，
# 它会显示所有的Shell变量和函数的内容。
# 显示目前Bash Shell所有的属性的开头状态。
set -o
# Bash的history功能是打开的，POSIX标准兼容模式
# 则是关闭的。
# 
# 
# 
# 
# 
# 
# 





























# printf "格式字符串" 参数

# 递归方法
currentDir=`pwd`
function digui() {	
	if [ -d ${1} ]; then
		for dir in $(ls ${1})
		do
			local tempDir="${1}/${dir}"
			if [ -d ${tempDir} ]; then
				# echo "It's directory."
				digui ${tempDir}
			else
				# doSomething
				echo ${tempDir}
				# echo "It's not directory." 
			fi
		done
	fi
}
digui ${currentDir}

# wget命令用于使用命令行下载网络文件，
# 格式为：“wget [参数] 下载地址”。
# 参数 	作用
# -b 		后台下载模式
# -O 		下载到指定目录
# -t 		最大尝试次数
# -c 		断点续传
# -P 		下载页面内所有资源，包括图片、视频等
# -r 		递归下载
wget -r -P http://www.linuxprobe.com 
wget -r -c url

# elinks用于实现一个纯文本界面的浏览器，
# 格式为：“elinks [参数] 网址”。
apt-get install elinks 
# elinks www.linuxprobe.com

uname -a 用于查看系统内核版本等信息

uptime 用于查看系统的的负载情况，
# 格式为：“uptime”。
# 用“watch -n 1 uptime”来每秒刷新一次获得当前的
# 系统负载情况，
# 输出内容分别为系统当前时间、系统已运行时间、
# 当前在线用户以及
# 平均负载值。而平均负载分为最近1分钟、5分钟、
# 15分钟的系统负载情况，
# 负载值越低越好（小于1是正常的）。

free 用于显示当前系统中内存的使用量情况，
# 格式为：“free [-m/-g]”。
# 以m为单位显示当前系统中内存的使用量情况：free -m 

who 用于查看当前登入主机的用户情况，
# 格式为：“who [参数]”。
# last 用于查看所有系统的登入记录，
# 格式为：“last [参数]”。
history 用于显示历史执行过的命令，
# 格式为：“history [-c]”。
# 历史命令会被保存到用户家目录中的“.bash_history”。
# history默认会保存1000条执行过的命令，
# 若要修改可直接编辑
# /etc/profile文件的HISTSIZE值。
# sosreport 用于收集系统配置并诊断信息后输出结论文档，
# 格式为：“sosreport”。
cd 用于切换工作路径，格式为：“cd [目录名称]”。
# 参数 		作用
# - 			切换到上一次的目录
# ~ 			切换到“家目录”
# ~username 	切换到其他用户的家目录
# .. 			切换到上级目录

ls 用于查看目录中有哪些文件，
# 格式为：“ls [选项] [文件]”。
# 参数 	作用
# -a 		查看全部文件
# -d 		公看目录本身
# -h 		易读的文件容量 
# -l 		显示文件的详细信息

cat 用于查看纯文本文件（较短的），
# 格式为：“cat [选项] [文件]”。
# 参数 	作用
# -n 		显示行号
# -b 		显示行号（不包括空行）
# -A 		显示出“不可见”的符号，如空格，tab键等
cat -e -t -v  makefile_name

more 用于查看纯文本文件（较长的），
# 格式为：“more [选项] 文件”。
# 参数 	作用
# -数字 	预先显示的行数（默认为一页）
# -d 		显示提示语句与报错信息

head 用于查看纯文本文档的前n行，
# 格式为：“head [选项] 文件”。
# 参数 	作用
# -n 10 	显示10行
# -n -10 	正常输出（如cat命令），但不显示最后的10行

tail 用于查看纯文本文档的后n行，
# 格式为：“tail [选项] 文件”。
# 参数 	作用
# -n 10 	显示10行
# -f 		持续刷新显示的内容

tr 用于转换文本文件中的字符，
# 格式为：“tr [原始字符] [目标字符]”。
cat tr.txt | tr [a-z] [A-Z]

cut 用于通过列来提取文本字符，
# 格式为：“cut [参数] 文本”。
# 参数 	  	作用
# -d 分隔符 	指定分隔符，默认为Tab
# -f 			指定显示的列数
# -c 			单位改为字符
cut -d: -f1 /etc/passwd
grep ^root /etc/passwd | cut -d: -f 7
# 使用逗号（“-d'，'”）作为字段分隔符输出iris.csv文件的第五列（“-f 5”）
cut -d ',' -f 5 iris.csv

wc 用于统计指定文本的行数、字数、字节数，
# 格式为：“wc [参数] 文本”。
# 参数 	作用
# -l 		只显示行数
# -w 		只显示单词数
# -c 		只显示字节数

diff 用于比较多个文本文件的差异，
# 格式为：“diff [参数] 文件”。
# 参数 			作用
# -b 				忽略空格引起的差异
# -B 				忽略空行引起的差异
# --brief或-q 	公报告是否存在差异
# -c 				使用上下文输出格式
diff diff_a.txt diff_b.txt

touch 用于创建空白文件与修改文件时间，
# 格式为：“touch [选项] 文件”。
# 对于在Linux中的文件有三种时间：
# 更改时间(mtime)：内容修改时间（不包括权限的）
# 更改权限(ctime)：更改权限与属性的时间
# 读取时间(atime)：读取文件内容的时间
# 参数 	作用
# -a 		仅修改“访问时间”(atime)
# -m 		仅修改“更改时间”(mtime)
# -d 		同时修改atime与mtime
# -t 		要修改的时间[YYMMDDhhmm]
touch -d "2 days ago" text.txt 

mkdir 用于创建空白的文件夹，
# 格式为：“mkdir [选项] 目录”。
# 参数 	作用
# -m=MODE	默认的文件目录权限，如“-m 755”
# -P 		连续创建多层目录（若文件夹已存在则忽略）
# -v 		显示创建的过程

# 变量!$或（键盘按键）代表上一条命令的参数。

cp 用于复制文件或目录，
# 格式为：“cp [选项] 源文件 目标文件”。
# 参数 	作用
# -P 		保留原始文件的属性
# -d 		若对象为“链接文件”，则保留该“链接文件”的属性
# -r 		递归持续复制（用于目录）
# -i 		若目标文件存在则询问是否覆盖
# -a 		相当于-pdr

mv 用于移动文件或改名，
# 格式为：“mv [选项] 文件名 目标路径加目标文件名”。

rm 用于删除文件或目录，
# 格式为：“rm [选项] 文件”。
# 参数 	作用
# -f 		忽略警告信息
# -i 		删除前先询问
# -r 		删除文件夹 
# rm -rf x.log 

dd 用于指定大小的拷贝的文件或指定转换文件，
# 格式为：“dd [参数]”。
# 参数 		作用
# if 			输入的文件名称
# of 			输出的文件名称
# bs 			设置每个“块”的大小
# count 		设置要拷贝“块”的个数
# conv=ucase 	将字母从小写转换为大写
# conv=lcase 	将字母从大写转换为小写
# 将光驱设备拷贝成镜像文件
dd if=/dev/cdrom of=
RHEL-server-7.0-x86_64-LinuxProbe.Com.iso 
# 生成一个560MB的空白文件
dd if=/dev/zero of=560_file count=1 bs=560MB
# 将感受的MBR信息拷贝出来
dd if=/dev/sda of=sda_image count=1 bs=512KB

useradd 用于创建新的用户，
# 格式为：“useradd [选项] 用户名”。
# 参数 	作用
# -d 		指定用户的家目录（默认为/home/username）
# -D 		展示默认值
# -e 		账号有效截至日期，格式：YYYY-MM-DD
# -g 		指定一个初始用户组（必须已存在）
# -G 		指定一个或多个扩展用户组
# -N 		不创建与用户同名的用户组
# -s 		指定默认的SHELL
# -u 		指定用户的UID
# 创建名为linuxprobe的用户，并定义家目录路径、UID
# 以及登陆解释器（不允许登陆）
useradd -d /home/linux -u 8888 -s 
/sbin/nologin linuxprobe 
# 查看linuxprobe用户的基本信息
id linuxprobe 

# passwd 用于修改用户的密码，
# 格式为：“passwd [选项] [用户名]”。
# 参数 		作用
# -l 			锁定用户禁止其登陆
# -u 			解除锁定，允许用户登陆
# --stdin 	允许从标准输入修改用户密码，
# 			如（echo "NewPassWord" | 
# 			passwd -stdin username）
# -d 			使账号无密码
# -e 			强制用户下次登陆时修改密码
# -S 			显示用户的密码状态

userdel 用于删除用户所有表格，
# 格式为：“userdel [选项] 用户名”。
# 参数 	作用
# -f 		强制删除用户，家目录与其相关文件
# -r 		同时删除用户，家目录与其相关文件
# 删除用户与其家目录
userdel -d 用户名 

usermod 用于修改用户的属性，
# 格式为：“usermod [选项] 用户名”。
# 参数 	作用
# -c 		赶写账号的备注信息
# -d -m 	-d与-m连用，可重新指定用户的家目录
# 并自动把旧的数据转移过去
# -e 		账户到期时间，格式“YYYY-MM-DD”
# -g 		变更所属用户组
# -G 		变更扩展用户组
# -L 		锁定用户禁止其登陆系统
# -U 		解锁用户，允许其登陆系统
# -s 		变更默认终端
# -u 		修改用户的UID

groupadd 用于创建群组，
# 格式为：“groupadd [选项] 群组名”。
# 创建名称为linuxprobe的用户群组
groupadd linuxprobe 

ping -m 8.8.8.8
nmap 192.168.1.7 -D 8.8.8.8 -ttl 13
tcpdump -i eth0 -v -nn 'host 8.8.8.8'

# 从当前目录（“.”）开始搜索以“iris”开始、
# 任意类型字符结尾（“-name'iris *'”）、
# 普通文件类型（“-type f“）的文件。
find . -name 'iris*' -type f

# 列出iris数据集类名第五列重复的次数
tail -n 150 iris.csv | cut -d ',' -f 5 | uniq -c

Version=$(lsb_release -r --short)
CodeName=$(lsb_release -c --short)
OSArch=$(uname -m)
echo "Version = $Version"
echo "CodeName = $CodeName"
echo "OS Architecture = $OSArch"

ps -A -Z | grep ""
grep "" -rn *

apt update 
apt upgrade 
apt dist-upgrade 
update-manager -c -d
do-release-upgrade -d
apt-get autoremove

# Bash Script排错的方法
# 
Bash -v test.sh
Bash -n test.sh
Bash -x test.sh

if [ "$#" -eq 1 ]; then
	case $1 in
		1)
			fun1
		;;
		2)
			fun2 # run one time
		;;
		3)
			fun3
		;;
		4)
			fun4
		;;
		*)
			echo "*"
		;;
	esac
fi

pushd /root/mydev/myfiles
// doSomething
popd

# 解压test.jar到dir目录
unzip test.jar -d dir
# 反编译dir/**/*.class文件到dir/src
jad -o -r -s java -d dir/src dir/**/*.class

ffmpeg -i input.mp4 output.mp4
ffplay aaaa.mp4
ffprobe -v quiet -print_format json -show_format -show_streams aaaaa.mp4 >> aaaaa.mp4.txt

# 查看SDL2的位置
locate SDL2

# head.txt文件中如果不存在字符串${tempFilePath}时成立
if [ `grep -c "${tempFilePath}" head.txt` -eq '0' ]; then
    echo ${tempFilePath} >> head.txt
fi

# vim 只需要:set invlist即可以将不可见的字符显示出来，例如，会以^I表示一个tab符，$表示一个回车符等。

# 需要这样子操作,不能直接用$?
result=$?
if [[ ${result} -eq 0 ]]; then
    echo "result = success"
elif [[ ${result} -eq 255 ]]; then
    echo "result = failure"
fi

# 路径中有空格时就这样使用
IFS_old=${IFS}
IFS=$'\n'

startHere

IFS=${IFS_old}


#授予某个应用有android.permission.CHANGE_CONFIGURATION权限
adb shell pm grant packagename android.permission.CHANGE_CONFIGURATION

#可以通过下面的命令来查找Zygote进程的源码：
find ./ -nameAndroid.mk -exec grep -l app_process {}\;

git config --global core.editor "subl -w"


function judge_content_is_same(){
    echo "5"
    return 10
}
result=`judge_content_is_same`
echo $?
echo ${result}
结果:
10
5

line='afdj dsflskjf sdlfk'
for i in `seq 1 ${#line}`
do
   a=`echo $line | cut -c $i`
   # echo -ne "$a"
   # 一个字符一个字符输出
   echo ${a}
done

# 特殊字符需要用到转义字符
# \' \" \* \? \\ \~ \` \! \# \$ \&  \| \{ \} \; \< \> \^
# 下面是特殊字符
# . * [ ] ^ $ { } \ + ? | ( )

# 在shell中判断单个字符是否相同,下面是两个看起来相同的字符进行比较的结果(如果! 与 !比是否相同)
# 相同: ` ~ ! @ # $ % ^ * ( ) _ + -  = { } | [ ] \ : " ; ' < > ? , . /
# 不同: \

# 去掉字符串前后的空格
name_string=`echo "${name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
# 去掉字符串首尾各一个字符
name_string=${name_string:1:-1}


























