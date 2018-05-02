如果要查看或者编辑run-me.sh这个文件，
那么用vim命令打开这个文件，
用图形窗口工具打开可能会出问题。

因为里面有个“^M”这样的符号，只有在vim模式下它才能正常工作。
（“^M”：在vim模式下，按下“Ctrl+v+m”）

run-me.sh的作用
（需要信赖ssh_auto_login_dosomething.bak.sh，scp_pull.sh这两个文件）
1）登录到远程服务器，然后编译相关应用
2）如果编译成功，那么把apk文件copy到本地
3）copy到本地后，进行签名
使用： source run-me.sh dtcp-ip-player

push.sh的作用
把本地的压缩文件（*.zip）上传到远程服务器上后进行解压，
解压成功后删除压缩文件。
使用： source push.sh 用户名 密码 ip 源文件 命令

semc.sh的作用：
这个文件是要放到远程服务器上的，
并且改名为“.semc.sh”。
里面就是一些编译命令。

