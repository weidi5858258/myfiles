#! /bin/bash

# run on time
function getWhiteList(){
    IFS_old=${IFS};
    IFS=$'\n';

    whiteList=""
    result=`cat whitelist.txt`;
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            aline=`echo ${aline} | awk  -F '\r' '{print $1}'`
            whiteList=${whiteList}"@@@"${aline};
            # echo ${whiteList};
        fi
    done
    whiteList=${whiteList}"@@@";
    # echo "whiteList = "${whiteList};

    IFS=${IFS_old};
}

# function getTempWhiteList(){
#     tempWhiteList=""

#     IFS_old=${IFS};
#     IFS=$'\n';

#     result=`cat ${whileListTxt}`;
#     for aline in ${result};
#     do
#         if [[ -n ${aline} ]]; then
#             # echo "before: "${tempWhiteList};
#             aline=`echo ${aline} | awk  -F '\r' '{print $1}'`
#             tempWhiteList=${tempWhiteList}"@@@"${aline};
#             # echo "after: "${tempWhiteList};
#         fi
#     done
#     tempWhiteList=${tempWhiteList}"@@@";
#     # echo "tempWhiteList = "${tempWhiteList};

#     IFS=${IFS_old};
# }

# function getCurrentRunningAppPackageName(){
#     currentRunningAppPackageName=""

#     # mSurface=Surface(name=com.tencent.mm/com.tencent.mm.ui.LauncherUI)
#     result=`dumpsys window w | grep \/ | grep name=`
#     tempResult=${result##*=};
#     tempCurrentRunningAppPackageName=${tempResult%\/*};

#     if [[ "com.android.launcher3" != ${tempCurrentRunningAppPackageName} ]]; then
#         currentRunningAppPackageName=${tempCurrentRunningAppPackageName}
#         # echo "currentRunningAppPackageName = "${currentRunningAppPackageName}

#         cp /dev/null ${currentRunningPackageNameTxt}
#         echo ${currentRunningAppPackageName} > ${currentRunningPackageNameTxt}
#     fi
# }

function killAnnoyingApps(){
    IFS_old=${IFS};
    IFS=$'\n';

    tempWhiteList=""
    result=`cat ${whileListTxt}`;
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            # echo "before: "${tempWhiteList};
            aline=`echo ${aline} | awk  -F '\r' '{print $1}'`
            tempWhiteList=${tempWhiteList}"@@@"${aline};
            # echo "after: "${tempWhiteList};
        fi
    done
    tempWhiteList=${tempWhiteList}"@@@";

    # result=`cat user_apps.log`;
	result=`ps | grep -e 'u0_a'`;
	for aline in ${result};
	do
   		# aline='u0_a87    509   413   1191524 69184 SyS_epoll_ 00f6fd7294 S com.qiyi.video'

   		processPid=`echo ${aline} | awk  -F ' ' '{print $2}'`
   		processName=`echo ${aline} | awk  -F ' ' '{print $9}'`
        # 后面有特殊字符
   		processName=`echo ${processName} | awk  -F '\r' '{print $1}'`
        # echo "processPid = "${processPid}" processName = "${processName};
        currentRunningAppPackageName=`cat ${currentRunningPackageNameTxt}`
        if [[ ${processName} == *":"* ]]; then
            # processName 进程中有包含“:”
            # packageName进程中没有包含“:”
            packageName=${processName%:*};
            # 不在白名单且不是当前运行的应用，那么就要被kill tempWhiteList
            if [[ ${whiteList} != *${packageName}* ]] && [[ ${tempWhiteList} != *${packageName}* ]] && [[ ${packageName} != *${currentRunningAppPackageName}* ]]; then
                kill -9 ${processPid} > /dev/null 2>&1
                am force-stop ${processName} > /dev/null 2>&1
                am force-stop ${packageName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1
                am force-stop ${packageName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1
                am force-stop ${packageName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1
                am force-stop ${packageName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1
                am force-stop ${packageName} > /dev/null 2>&1
                # echo "processPid = "${processPid}" processName = "${processName};
            fi
        else
            if [[ ${whiteList} != *${processName}* ]] && [[ ${tempWhiteList} != *${processName}* ]] && [[ ${processName} != *${currentRunningAppPackageName}* ]]; then
                if [[ ${kugouSupport} == ${processName} ]]; then
                    continue;
                fi
                kill -9 ${processPid} > /dev/null 2>&1
                am force-stop ${processName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1

                am force-stop ${processName} > /dev/null 2>&1
                # echo "processPid = "${processPid}" processName = "${processName};
            fi
        fi
	done

    # if file exist
    if [[ -e ${userPackageNameTxt} ]]; then
        result=`cat ${userPackageNameTxt}`;
        for aline in ${result};
        do
            if [[ -n ${aline} ]]; then
                currentRunningAppPackageName=`cat ${currentRunningPackageNameTxt}`
                if [[ ${whiteList} != *${aline}* ]] && [[ ${tempWhiteList} != *${aline}* ]] && [[ ${aline} != *${currentRunningAppPackageName}* ]]; then
                    am force-stop ${aline} > /dev/null 2>&1
                    # echo "packageName = "${aline};
                fi
            fi
        done
    fi

	IFS=${IFS_old};
}

# start run
currentRunningPackageNameTxt="/data/data/com.weidi.suspensiontools/files/CurrentRunningPackageName.txt";
whileListTxt="/data/data/com.weidi.suspensiontools/files/WhileList.txt";
userPackageNameTxt="/data/data/com.weidi.suspensiontools/files/UserPackageName.txt";
kugouSupport="com.kugou.android.support";

source packagename.sh & 

# run one time
getWhiteList

while :
do

    killAnnoyingApps

    # sleep 1
done