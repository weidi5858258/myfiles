#! /bin/sh

if [ $# -lt 1 ]
then
	echo "at least parameter has one:$?"
	return 1
fi

folder=$1
if [ ${folder} != "dtcp-ip-player" -a ${folder} != "video-player" -a ${folder} != "Album" -a ${folder} != "walkman-music-player" ]
then
	#exit 1;
	echo ""
	return 1
fi

if [ ! -e ${folder} -o ! -d ${folder} ]
then
	#exit 1;
	echo ""
	return 1	
fi

cd ${folder} &&
export ANDROIDSDK_HOME=/home/he/sdk/m-kitakami &&
semc-build -offline clean && 
semc-build -offline

if [ $? -ne 0 ]
then
	if [ ${folder} = "dtcp-ip-player" ];
	then
		rm -rf semc-build-output &&
		cd ~ &&
		rm -rf ivy_SemcDTCPIPPlayer
	elif [ ${folder} = "video-player" ];
	then
		rm -rf semc-build-output &&
		cd ~ &&
		rm -rf ivy_SemcVideo
	elif [ ${folder} = "Album" ];
	then
		rm -rf semc-build-output &&
		cd ~ &&
		rm -rf ivy_SemcAlbum
	elif [ ${folder} = "walkman-music-player" ];
	then
		rm -rf semc-build-output &&
		cd ~ &&
		rm -rf ivy_SemcMusic
	fi

	return 1
fi

if [ ${folder} = "dtcp-ip-player" ];
then
	cp ~/dtcp-ip-player/semc-build-output/SemcDTCPIPPlayer/bin/SemcDTCPIPPlayer.apk ~ && 
	rm -rf semc-build-output &&
	cd ~ &&
	rm -rf ivy_SemcDTCPIPPlayer
elif [ ${folder} = "video-player" ];
then
	cp ~/video-player/semc-build-output/SemcVideo/bin/SemcVideo.apk ~ &&
	rm -rf semc-build-output &&
	cd ~ &&
	rm -rf ivy_SemcVideo
elif [ ${folder} = "Album" ];
then
	cp ~/Album/semc-build-output/Album/bin/SemcAlbum.apk ~ &&
	rm -rf semc-build-output &&
	cd ~ &&
	rm -rf ivy_SemcAlbum
elif [ ${folder} = "walkman-music-player" ];
then
	cp ~/walkman-music-player/semc-build-output/SemcMusic/bin/SemcMusic.apk ~ &&
	rm -rf semc-build-output &&
	cd ~ &&
	rm -rf ivy_SemcMusic
fi
