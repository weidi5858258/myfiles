@echo off

cls

color 0d
::让变量的值即刻生效,需要使用!var!这种方式来引用变量
setlocal enabledelayedexpansion

::init注释
::当前目录
set current_dir=%~dp0
echo %current_dir%
set log_file_path=%current_dir%test_result.log
echo %log_file_path%
::赋空
set previous_language="
set current_language="
::赋值都要使用set关键字
set is_usb_exist=false
set is_usb_content_empty=true
set wifi_enable_run_one_time=false
set wifi_disable_run_one_time=false
set result=0
set adb_success=0
set adb_root_failure=-1
set adb_remount_failure=-2
set adb_mount_rw_failure=-3
set language_file_path=/data/autotest/capture_pkg/language_info.xml
set language_file_path_temp=/data/autotest/capture_pkg/language_info.xml.bak
set "no_such_file=No such file or directory"

:::::::::::::::::::::::::start run:::::::::::::::::::::::::

::调用函数
call:handle_adb
::变量值相等时,如果不相等为if not !result!==%adb_success% ()
if !result!==%adb_success% (
    adb shell setenforce 0
    call:judge_usb_is_exist_and_contents_is_empty
    if !is_usb_exist!==false (
        ::显示内容,不需要加引号,加了引号后引号也会输出
        echo No USB disk insertion was found
    ) else (
        set run_one_time=0
        for /F "delims=" %%i in ('adb shell cat %language_file_path%') do (
            if !run_one_time!==0 (
                ::显示结果.命令执行后返回的结果,一行一行返回
                rem echo %%i
                set run_one_time=1
                adb shell mv %language_file_path% %language_file_path_temp%
                echo adb shell mv %language_file_path% %language_file_path_temp%
            ) 
        ) 

        call:do_something

        for /F "delims=" %%i in ('adb shell cat %language_file_path_temp%') do (
            rem echo %%i
            adb shell mv %language_file_path_temp% %language_file_path%
            echo adb shell mv %language_file_path_temp% %language_file_path%

            setlocal disabledelayedexpansion

            goto :eof
        )
    )
) 

setlocal disabledelayedexpansion

goto :eof
:::::::::::::::::::::::::end run:::::::::::::::::::::::::

:::::::::::::::::::::::::函数实现:::::::::::::::::::::::::

::函数名
:stop_smartmediaapp_and_autotest
    rem echo stop_smartmediaapp_and_autotest
    adb shell am force-stop package_name
    adb shell am force-stop package_name.autotest
    ::函数结束,不使用的话不管函数调用与否,都会执行下面的代码
    goto :eof

:language_init
    echo language_init
    call:stop_smartmediaapp_and_autotest
    adb shell am instrument -w -r -e debug false -e class package_name.auto.launcher.ChangeLanguage#testLanguageInit package_name.autotest/android.support.test.runner.AndroidJUnitRunner >> %log_file_path%
    goto :eof

:select_next_language
    echo select_next_language
    call:stop_smartmediaapp_and_autotest
    adb shell am instrument -w -r -e debug false -e class package_name.auto.launcher.ChangeLanguage#testChangeLanguages package_name.autotest/android.support.test.runner.AndroidJUnitRunner >> %log_file_path%
    goto :eof

:get_current_language
    echo get_current_language
    rem [persist.sys.locale]: [zh-CN]
    rem Don't delete
    for /F "delims=" %%i in ('adb shell getprop ^| findstr /i /c:"persist.sys.locale"') do (
        set temp_language=%%i
        set current_language=%temp_language:~-6,5%
    )
    rem Don't delete
    for /F "delims=" %%i in ('adb shell getprop ^| findstr /i /c:"persist.sys.locale"') do (
        set temp_language=%%i
        ::取[persist.sys.locale]: [zh-CN]这个字符串中的内容zh-CN(从后面第6位开始往后取,取5位)
        set current_language=%temp_language:~-6,5%
    )
    echo current_language: !current_language!        previous_language: !previous_language!
    goto :eof

:delete_captured_pic
    echo delete_captured_pic
    call:stop_smartmediaapp_and_autotest
    adb shell am instrument -w -r -e debug false -e class package_name.auto.launcher.ChangeLanguage#deleteCapturedPic package_name.autotest/android.support.test.runner.AndroidJUnitRunner >> %log_file_path%
    goto :eof

:judge_usb_is_exist_and_contents_is_empty
    echo judge_usb_is_exist_and_contents_is_empty
    for /F "delims=" %%i in ('adb shell ls /mnt/media_rw') do (
        if not %%i==sdcard (
            echo USB Disk: %%i
            set is_usb_exist=true
            echo is_usb_exist=true

            for /F "delims=" %%j in ('adb shell ls /mnt/media_rw/%%i') do (
                rem echo %%j
                set temp_count=0
                if %%j==Android (
                    if !temp_count!==0 (
                        set temp_count=1
                    ) 
                ) 

                if %%j==LOST.DIR (
                    if !temp_count!==0 (
                        set temp_count=1
                    )
                ) 

                if "%%j"=="System Volume Information" (
                    if !temp_count!==0 (
                        set temp_count=1
                    )
                ) 

                if %%j==.Trash-0 (
                    if !temp_count!==0 (
                        set temp_count=1
                    )
                ) 

                if %%j==.Trash-10 (
                    if !temp_count!==0 (
                        set temp_count=1
                    )
                ) 

                if %%j==.Trash-100 (
                    if !temp_count!==0 (
                        set temp_count=1
                    )
                ) 

                if %%j==.Trash-1000 (
                    if !temp_count!==0 (
                        set temp_count=1
                    )
                ) 

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
            goto break_for
        ) else (
            echo is_usb_exist=false
            set is_usb_exist=false
        )
    )
    ::标记标签
    :break_for

    if !is_usb_exist!==true (
        if !is_usb_content_empty!==true (
            if %wifi_enable_run_one_time%==false (
                adb shell svc wifi enable;
                echo adb shell svc wifi enable;
                set wifi_enable_run_one_time=true
                ::暂停15秒
                ping 127.0.0.1 -n 15 > nul
            )
        ) else (
            if %wifi_disable_run_one_time%==false (
                adb shell svc wifi disable;
                echo adb shell svc wifi disable;
                set wifi_disable_run_one_time=true
                ping 127.0.0.1 -n 15 > nul
            )
        )
    )

    echo is_usb_exist: !is_usb_exist!        is_usb_content_empty: !is_usb_content_empty!
    goto :eof

:handle_adb
    echo handle_adb
    adb root
    ::%ERRORLEVEL%上面的命令执行后的情况,为0时表示命令执行成功,并不是命令执行后返回的结果
    set result=%ERRORLEVEL%
    if !result!==%adb_success% (
        adb remount
        set result=%ERRORLEVEL%
        if !result!==%adb_success% (
                adb shell mount -o rw,remount /
                set result=%ERRORLEVEL%
                if !result!==%adb_success% (
                    echo adb shell mount -o rw,remount / success!
                    set result=%adb_success%
                ) else (
                    echo adb shell mount -o rw,remount / failure!
                    set result=%adb_mount_rw_failure%
                )
        ) else (
                echo adb remount failure!
                set result=%adb_remount_failure%
        )
    ) else (
        echo adb root failure!
        set result=%adb_root_failure%
    )
    goto :eof

:do_something
    echo do_something
    ::如果文件存在
    if exist %log_file_path% (
        ::删除文件
        del %log_file_path%
    )

    call:language_init
    call:delete_captured_pic

    ::达到无限循环
    :infinite_loop

        echo.
        call:select_next_language
        call:get_current_language
        if !current_language!==!previous_language! (
            ::空行
            echo.
            echo Current language has been run.
            echo.
            goto :eof
        )

        call:stop_smartmediaapp_and_autotest
        call:judge_usb_is_exist_and_contents_is_empty
        if !is_usb_exist!==true (
            if !is_usb_content_empty!==true (
                set run_test_file=package_name.auto.launcher.NoContentInDevicesTest
            ) else (
                set run_test_file=package_name.auto.launcher.HaveContentInUSBAndUmountTest
            )
        ) else (
            echo No USB disk insertion was found
            goto :eof
        )

        echo !run_test_file!
        set previous_language=!current_language!
        adb shell am instrument -w -r -e debug false -e class !run_test_file! package_name.autotest/android.support.test.runner.AndroidJUnitRunner >> %log_file_path%

    goto infinite_loop

    goto :eof
