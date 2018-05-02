#! /bin/bash

# 1.首先安装root软件 
# 2.进行root
# 3.安装软件 
# 4.恢复文件 
# mkdir /storage/emulated/0/DCIM/screencap ./
# /storage/emulated/0/baidu/ime/
# /data/data
# /storage/emulated/0/Android/data

function restore_data_data(){
    IFS_old=${IFS}
    IFS=$'\n'

    result=`ls -al ${currentDir}${data_data}`
    for aline in ${result}
    do
        packageName=`echo ${aline} | awk  -F ' ' '{print $9}'`
        if [[ -n ${packageName} ]] && [[ ${packageName:0:1} != "." ]]; then
            adbd push ${currentDir}${data_data}${packageName} ${data_data}
        fi
    done

    IFS=${IFS_old}
}

function restore_Android_data(){
    IFS_old=${IFS}
    IFS=$'\n'

    result=`ls -al ${currentDir}${Android_data}`
    for aline in ${result}
    do
        packageName=`echo ${aline} | awk  -F ' ' '{print $9}'`
        if [[ -n ${packageName} ]] && [[ ${packageName:0:1} != "." ]]; then
            adbd push ${currentDir}${Android_data}${packageName} ${sdcard}${Android_data}
        fi
    done

    IFS=${IFS_old}
}

function install_apk(){
    IFS_old=${IFS}
    IFS=$'\n'

    result=`ls -al ${currentDir}"/pp/downloader/apk"`
    for aline in ${result}
    do
        apkName=`echo ${aline} | awk  -F ' ' '{print $9}'`
        if [[ -n ${apkName} ]] && [[ ${apkName:0:1} != "." ]]; then
            # adbd push ${currentDir}${Android_data}${packageName} ${sdcard}${Android_data}
            packageName=`echo ${apkName} | awk  -F '_' '{print $1}'`
            if [[ ${packageName} == "cn.wps.moffice" ]]; then
                packageName=${packageName}"_eng"
            fi
            isExist=`adbd shell ls ${data_data} | grep -e "${packageName}"`
            if [[ -z ${isExist} ]]; then
                # echo "${packageName} 这个应用没有安装"
                adbd install -r ${currentDir}"/pp/downloader/apk/"${apkName}
            fi
        fi
    done

    IFS=${IFS_old}
}

# start
# currentDir=`pwd`"/"
currentDir="./"
sdcard="/storage/emulated/0/"
data_data="/data/data/"
Android_data="/Android/data/"

install_apk
adbd push ${currentDir}"DCIM" ${sdcard}
adbd push ${currentDir}"Download" ${sdcard}
adbd push ${currentDir}"Music" ${sdcard}
adbd push ${currentDir}"Movies" ${sdcard}
adbd push ${currentDir}"Pictures" ${sdcard}
adbd push ${currentDir}"baidu" ${sdcard}
adbd push ${currentDir}"myfiles" ${sdcard}
adbd push ${currentDir}"contacts.vcf" ${sdcard}
adbd push ${currentDir}"/data/local/artifact" "/data/local/"
adbd push ${currentDir}"/kgmusic/download" ${sdcard}"/kgmusic/"
restore_data_data
restore_Android_data

