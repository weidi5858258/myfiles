#! /bin/bash

# 参数为空
if [[ -z $1 ]]; then
    cat user_apps.txt;

elif [[ $1 == "all" ]]; then
    IFS_old=${IFS};
    IFS=$'\n';

    result=`cat user_apps.txt`;
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            number=`echo ${aline} | awk  -F '@@@' '{print $1}'`
            packageName=`echo ${aline} | awk  -F '@@@' '{print $2}'`
            pm hide ${packageName} > /dev/null 2>&1
            # echo ${packageName};
        fi
    done

    IFS=${IFS_old};
    
else
    parameter=${1}"@@@"${2}"@@@"${3}"@@@"${4}"@@@"${5}"@@@"${6}"@@@"${7}"@@@"${8}"@@@"${9}"@@@"${10}

    IFS_old=${IFS};
    IFS=$'\n';

    result=`cat user_apps.txt`;
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            number=`echo ${aline} | awk  -F '@@@' '{print $1}'`
            if [[ ${parameter} == *${number}* ]]; then
                packageName=`echo ${aline} | awk  -F '@@@' '{print $2}'`
                pm hide ${packageName} > /dev/null 2>&1
                # echo ${packageName};
            fi
        fi
    done

    IFS=${IFS_old};

fi
