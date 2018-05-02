#! /bin/sh

if [ $# -eq 1 ]
then
	folder=$1
	cd ${folder}
	if [ $? -eq 0 ]
	then
		export ANDROIDSDK_HOME=/home/he/sdk/m-kitakami &&
		semc-build -offline clean && 
		semc-build -offline

		if [ $? -eq 0 ]
		then
			if [ ${folder} = "dtcp-ip-player" ];
			then
				cp ~/dtcp-ip-player/semc-build-output/SemcDTCPIPPlayer/bin/SemcDTCPIPPlayer.apk ~ &&
				rm -rf semc-build-output &&
				cd ~ &&
				rm -rf ivy_SemcDTCPIPPlayer

				if [ $? -eq 0 ]
				then
					echo "..."
				fi
			elif [ ${folder} = "video-player" ];
			then
				rm -rf ivy_SemcVideo
			elif [ ${folder} = "Album" ];
			then
				echo "..."
			elif [ ${folder} = "walkman-music-player" ];
			then
				echo "..."
			else
				echo "..."
			fi
		else
			cd ~
		fi
	else
		echo "cd $1 failed:$?"
	fi
else
	echo "at least parameter has one:$?"
fi
