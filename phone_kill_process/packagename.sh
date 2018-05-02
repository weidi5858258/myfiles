#! /bin/bash

# function getCurrentRunningAppPackageName(){
#     currentRunningAppPackageName=""

#     # mSurface=Surface(name=com.tencent.mm/com.tencent.mm.ui.LauncherUI)
#     result=`dumpsys window w | grep \/ | grep name=`
#     result=${result##*=};
#     currentRunningAppPackageName=${result%\/*};

#     if [[ "com.android.launcher3" != ${currentRunningAppPackageName} ]]; then
#         currentRunningAppPackageName=${currentRunningAppPackageName}
#         # echo "currentRunningAppPackageName = "${currentRunningAppPackageName}

#         cp /dev/null ${currentRunningPackageNameTxt}
#         echo ${currentRunningAppPackageName} > ${currentRunningPackageNameTxt}
#     fi
# }

# start run
desktopApp="com.android.launcher3";
currentRunningPackageNameTxt="/data/data/com.weidi.suspensiontools/files/CurrentRunningPackageName.txt";

while :
do
    # mSurface=Surface(name=com.tencent.mm/com.tencent.mm.ui.LauncherUI)
    currentRunningAppPackageName=`dumpsys window w | grep \/ | grep name=`;
    currentRunningAppPackageName=${currentRunningAppPackageName##*=};
    currentRunningAppPackageName=${currentRunningAppPackageName%\/*};

    if [[ ${currentRunningAppPackageName} != ${desktopApp} ]]; then
        # echo "currentRunningAppPackageName = "${currentRunningAppPackageName}
        # cp /dev/null ${currentRunningPackageNameTxt}
        echo ${currentRunningAppPackageName} > ${currentRunningPackageNameTxt}
    fi

	sleep 1;
done
