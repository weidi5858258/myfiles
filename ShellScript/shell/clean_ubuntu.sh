#! /bin/bash

function clean(){
	echo "Y" | apt-get autoremove && 
	# 列出这个目录下所有文件所占用的空间大小
	# du -sh /var/cache/apt
	apt-get clean && 
	rm -rf ~/.cache/thumbnails/*
}

clean

