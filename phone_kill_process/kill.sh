#! /bin/bash

function handleJobs(){
	IFS_old=${IFS};
	IFS=$'\n';

	needToKillPorcessAgain="false";
	local result=`jobs`;
	for aline in ${result};
	do
	    if [[ -n ${aline} ]]; then
	        local jobsId=`echo ${aline} | awk  -F ' ' '{print $1}'`
	        # echo "jobsId="${jobsId};
	        if [[ ${jobsId} == *"1"* ]]; then
				jobsId="1";
	        elif [[ ${jobsId} == *"2"* ]]; then
	        	jobsId="2";
        	elif [[ ${jobsId} == *"3"* ]]; then
        		jobsId="3";
        	elif [[ ${jobsId} == *"4"* ]]; then
        		jobsId="4";
        	elif [[ ${jobsId} == *"5"* ]]; then
        		jobsId="5";
    		elif [[ ${jobsId} == *"6"* ]]; then
    			jobsId="6";
    		elif [[ ${jobsId} == *"7"* ]]; then
    			jobsId="7";
    		elif [[ ${jobsId} == *"8"* ]]; then
    			jobsId="8";
    		elif [[ ${jobsId} == *"9"* ]]; then
    			jobsId="9";
    		elif [[ ${jobsId} == *"10"* ]]; then
    			jobsId="10";
	        fi
	        # echo "jobsId="${jobsId};
	        local Terminated=`echo ${aline} | awk  -F ' ' '{print $3}'`
	        # echo "Terminated="${Terminated};
	        if [[ ${Terminated} != "Terminated" ]]; then
	        	kill %${jobsId} > /dev/null 2>&1
	        	# echo 'kill %'${jobsId};
	        	needToKillPorcessAgain="true";
	        fi
	    fi
	done
	# echo "needToKillPorcessAgain="${needToKillPorcessAgain}

	IFS=${IFS_old};
}

# start run
needToKillPorcessAgain="true";

while :
do
    if [[ ${needToKillPorcessAgain} == "true" ]]; then
    	# echo "handleJobs()";
		handleJobs;
    else
    	echo "while break()";
    	break;
    fi
done
