rem 注释的关键字

adb root
adb remount
adb shell mount -o rw,remount /

::执行脚本的时候不显示执行的命令
@echo off

::清屏
cls

::改变命令颜色
color 0d
color 07

::让变量的值即刻生效,需要使用!var!这种方式来引用变量
setlocal enabledelayedexpansion
setlocal disabledelayedexpansion

::当前目录
set current_dir=%~dp0
::显示变量的值
echo %current_dir%

::暂停15秒
ping 127.0.0.1 -n 15 > nul

adb root
::%ERRORLEVEL%上面的命令执行后的情况,为0时表示命令执行成功,并不是命令执行后返回的结果
set result=%ERRORLEVEL%

::如果文件存在
if exist %log_file_path% (
    ::删除文件
    del %log_file_path%
)

::设置一个变量并初始化
set log_file_path=%current_dir%test_result.log

::赋空
rem set previous_language="
rem set current_language="

::赋值都要使用set关键字
set result=0
set is_usb_exist=false
set language_file_path=/data/autotest/capture_pkg/language_info.xml
::给变量设置有空格的字符串
set "no_such_file=No such file or directory"

tracert ip地址(用于判断ip地址是公网还是内网,结果返回一条数据的是公网)






















































::变量值相等时,如果不相等为if not !result!==%adb_success% (...) else (...)
if !result!==%adb_success% (
    adb shell setenforce 0
    call:judge_usb_is_exist_and_contents_is_empty
    if !is_usb_exist!==false (
        ::显示内容,不需要加引号,加了引号后引号也会输出
        echo No USB disk insertion was found
    ) else (
        ::在for语句中,变量的值即使变了引用时还是原来的值,并没有改变
        ::因此需要使用到
        ::setlocal enabledelayedexpansion
        ::setlocal disabledelayedexpansion
        set run_one_time=0
        ::执行括号中的命令后返回的结果
        for /F "delims=" %%i in ('adb shell cat %language_file_path%') do (
            if !run_one_time!==0 (
                ::显示结果.命令执行后返回的结果,一行一行返回
                rem echo %%i
                set run_one_time=1
                adb shell mv %language_file_path% %language_file_path_temp%
                echo adb shell mv %language_file_path% %language_file_path_temp%
            ) 
        ) 
    )
) 

::调用函数
call:do_something
::定义函数
:do_something
    for /F "delims=" %%i in ('adb shell getprop ^| findstr /i /c:"persist.sys.locale"') do (
        set temp_language=%%i
        ::对temp_language变量进行截取字符串
        ::取[persist.sys.locale]: [zh-CN]这个字符串中的内容zh-CN
        ::从后面第6位开始往后取,取5位
        set current_language=%temp_language:~-6,5%
    )
    ::需要执行两次同样的代码才能达到想要的结果
    for /F "delims=" %%i in ('adb shell getprop ^| findstr /i /c:"persist.sys.locale"') do (
        set temp_language=%%i
        set current_language=%temp_language:~-6,5%
    )
    echo current_language: !current_language!        previous_language: !previous_language!
    
    for /F "delims=" %%i in ('adb shell ls /mnt/media_rw') do (
        if not %%i==sdcard (
            for /F "delims=" %%j in ('adb shell ls /mnt/media_rw/%%i') do (
                if !temp_count!==1 (
                    rem echo is_usb_content_empty=true
                    set is_usb_content_empty=true
                ) else (
                    echo is_usb_content_empty=false
                    set is_usb_content_empty=false
                    goto break_for
                )
            )
            ::跳出循环(跳到标签处)
            ::好像在for语句中才有效
            goto break_for
        ) else (
            echo is_usb_exist=false
            set is_usb_exist=false
        )
    )

    ::标记标签
    :break_for

    ::函数结束,不使用的话不管函数调用与否,都会执行下面的代码
    ::所以,在windows下,定义函数放在代码执行块的后面
    ::每个函数都以"goto :eof"结尾
    goto :eof




















































