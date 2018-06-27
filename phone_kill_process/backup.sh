#! /bin/bash

# 1.首先安装root软件 
# 2.进行root
# 3.安装软件 
# 4.恢复文件 
# mkdir /storage/emulated/0/DCIM/screencap ./
# /storage/emulated/0/baidu/ime/
# /data/data
# /storage/emulated/0/Android/data

function backup_data_data(){
    IFS_old=${IFS}
    IFS=$'\n'

    result=`cat ${currentDir}"backup_apps.txt"`
    for aline in ${result}
    do
        packageName=`echo ${aline} | awk  -F '@@@' '{print $2}'`
        # echo "adbd pull ${data_data}${packageName} ${currentDir}${data_data}"
        adbd pull ${data_data}${packageName} ${currentDir}${data_data}
    done

    IFS=${IFS_old}
}

function backup_Android_data(){
    IFS_old=${IFS}
    IFS=$'\n'

    result=`cat ${currentDir}"backup_apps.txt"`
    for aline in ${result}
    do
        packageName=`echo ${aline} | awk  -F '@@@' '{print $2}'`
        # echo "adbd pull ${sdcard}${packageName} ${currentDir}${Android_data}"
        adbd pull ${sdcard}${Android_data}${packageName} ${currentDir}${Android_data}
    done

    IFS=${IFS_old};
}

# start
# currentDir=`pwd`"/"
currentDir="./"
sdcard="/storage/emulated/0/"
data_data="/data/data/"
Android_data="/Android/data/"

if [[ ! -e ${currentDir}${data_data} ]]; then
    mkdir -p ${currentDir}${data_data}
fi

if [[ ! -e ${currentDir}${Android_data} ]]; then
    mkdir -p ${currentDir}${Android_data}
fi

adbd pull ${sdcard}"DCIM" ${currentDir}
adbd pull ${sdcard}"Download" ${currentDir}
adbd pull ${sdcard}"Music" ${currentDir}
adbd pull ${sdcard}"Movies" ${currentDir}
adbd pull ${sdcard}"Pictures" ${currentDir}
adbd pull ${sdcard}"baidu" ${currentDir}
adbd pull ${sdcard}"myfiles" ${currentDir}
adbd pull ${sdcard}"contacts.vcf" ${currentDir}
if [[ ! -e ${currentDir}"/data/local/" ]]; then
    mkdir -p ${currentDir}"/data/local/"
fi
adbd pull "/data/local/artifact" ${currentDir}"/data/local/"
if [[ ! -e ${currentDir}"/kgmusic/" ]]; then
    mkdir -p ${currentDir}"/kgmusic/"
fi
adbd pull ${sdcard}"/kgmusic/download" ${currentDir}"/kgmusic"
if [[ ! -e ${currentDir}"/pp/downloader/" ]]; then
    mkdir -p ${currentDir}"/pp/downloader"
fi
adbd pull ${sdcard}"/pp/downloader/apk" ${currentDir}"/pp/downloader"
backup_data_data
backup_Android_data
