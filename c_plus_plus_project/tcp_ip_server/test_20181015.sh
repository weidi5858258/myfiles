#! /bin/bash

current_dir=`pwd`

src_dir="/root/mydev/workspace_android/smartmediaapp"
src_dir_app_res="${src_dir}/app/src/main/res"
src_dir_controlpanel_res="${src_dir}/controlpanel/src/main/res"
src_dir_music_res="${src_dir}/music/src/main/res"
src_dir_musicslideshow_res="${src_dir}/musicslideshow/src/main/res"
src_dir_searchhomenetwork_res="${src_dir}/searchhomenetwork/src/main/res"
src_dir_video_res="${src_dir}/video/src/main/res"

dest_dir="/root/mydev/workspace_android/smartmediaapp/app/src/androidTest/res"

function judge_content_contain_digit() {
    string=${1}
    if [[ -z ${string} ]]; then
        return 1
    fi

    for i in `seq 1 ${#string}`
    do
        a_char=`echo ${string} | cut -c $i`
        if [[ ${a_char} == "0" ]] \
            || [[ ${a_char} == "1" ]] \
            || [[ ${a_char} == "2" ]] \
            || [[ ${a_char} == "3" ]] \
            || [[ ${a_char} == "4" ]] \
            || [[ ${a_char} == "5" ]] \
            || [[ ${a_char} == "6" ]] \
            || [[ ${a_char} == "7" ]] \
            || [[ ${a_char} == "8" ]] \
            || [[ ${a_char} == "9" ]]; then
            return 0
        fi
    done

    return 1
}

function handle_string_src() {
    # /root/mydev/workspace_android/smartmediaapp/app/src/main/res/values-pt-rBR
    # echo ${1}
    res_string_path="${1}/strings.xml"
    dest_res_values_path="${dest_dir}/${values_country}"
    dest_res_values_strings_path="${dest_res_values_path}/strings.xml"
    if [[ ! -e ${dest_res_values_path} ]]; then
        mkdir -p ${dest_res_values_path}
    fi
    if [[ ! -e ${dest_res_values_strings_path} ]]; then
        touch ${dest_res_values_strings_path}
        echo '<?xml version="1.0" encoding="utf-8"?>' >> ${dest_res_values_strings_path}
        echo '<resources xmlns:android="http://schemas.android.com/apk/res/android" xmlns:xliff="urn:oasis:names:tc:xliff:document:1.2">' >> ${dest_res_values_strings_path}
        echo '</resources>' >> ${dest_res_values_strings_path}
    fi

    IFS_old=${IFS}
    IFS=$'\n'

    result=`grep '<string' ${res_string_path} | grep 'sony_id="M'`
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            # <string name="album_actionbar_ownalbums_txt" sony_id="M10225">个人相册</string>
            # echo ${aline}

            sony_id=`echo ${aline} | awk  -F 'sony_id="' '{print $2}'`
            sony_id=${sony_id%%'"'*}

            # head.txt文件中如果不存在字符串${tempFilePath}时成立
            if [[ `grep -c "${sony_id}" "${dest_res_values_strings_path}"` -eq '0' ]]; then
                name_string=`echo ${aline} | awk  -F ' ' '{print $2}'`
                name_string=${name_string%'"'*}
                name_string=${name_string#*'"'}
                name_string=`echo "${name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`

                content_string=${aline%"<"*}
                content_string=${content_string##*">"}
                content_string=`echo "${content_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`

                # echo ${name_string} ${sony_id} ${content_string}
                new_string='<string name="'${sony_id}'"''>'"${content_string}"'</string>'
                if [[ ${content_string} =~ '\n' ]]; then
                    echo ${1}
                    echo ${new_string}
                    new_string=`echo ${new_string} | sed 's/\\\\n/######/g'`
                    echo ${new_string}
                    sed -i -e "/resources>/i\\    ${new_string}" ${dest_res_values_strings_path}
                    sed -i -e 's/######/\\n/g' ${dest_res_values_strings_path}
                    continue
                fi

                sed -i -e "/resources>/i\\    ${new_string}" ${dest_res_values_strings_path}
            fi
        fi
    done

    if [[ `grep -c 'name="M' "${dest_res_values_strings_path}"` -eq '0' ]]; then
        rm -rf ${dest_res_values_path}
    fi

    IFS=${IFS_old}
}

function digui_src_res() {
    if [ -d ${1} ]; then
        for dir in $(ls ${1})
        do
            tempDir="${1}/${dir}"
            if [ -d ${tempDir} ]; then
                values_country=${tempDir##*"/"}
                # echo ${values_country}
                if [[ ${values_country} =~ "values" ]] \
                    && [[ ! ${values_country} =~ "dp" ]] \
                    && [[ ! ${values_country} =~ "land" ]]; then
                    judge_content_contain_digit ${values_country}
                    if [[ $? -ne 0 ]]; then
                        handle_string_src ${tempDir}
                    fi
                else
                    digui_src_res ${tempDir}
                fi
            fi
        done
    fi
}

#digui_src_res ${src_dir_app_res}
#digui_src_res ${src_dir_controlpanel_res}
#digui_src_res ${src_dir_music_res}
#digui_src_res ${src_dir_musicslideshow_res}
#digui_src_res ${src_dir_searchhomenetwork_res}
digui_src_res ${src_dir_video_res}

#new_string="%1$d av"
#if [[ ${new_string} =~ "$d" ]]; then
##    new_string=`echo ${new_string} | sed 's/$d/######/g'`
#    new_string1=`echo ${new_string} | awk  -F "$d" '{print $1}'`
#    new_string2=`echo ${new_string} | awk  -F '$d' '{print $2}'`
#    echo "--->"${new_string1} ${new_string2}
#fi
#printf "%s" ${new_string}
#for i in `seq 1 ${#new_string}`
#do
#    a_char=`echo ${new_string} | cut -c $i`
#    if [[ ${a_char} == "\$" ]]; then
#        a_char="######"
#    fi
##    echo ${a_char}
##    echo -e "${a_char}\c" >> test.txt
#    new_string_temp=${new_string_temp}${a_char}
#done
#new_string=`echo ${new_string} | sed 's/$/######/g'`
#echo ${new_string_temp}
#sed -i -e "/resources>/i\\    ${new_string}" ${dest_res_values_strings_path}
#sed -i -e 's/######/$/g' ${dest_res_values_strings_path}

#echo who am i | grep -q "^Kenny$d"
#if [ $? -ne 0 ] ;then
#    echo "grep return a none-zero value, not find Kenny"
#else
#    echo "grep return zero value, find Kenny"
#fi
#
#new_string="%1######abcd"
#new_string=`echo ${new_string} | sed 's/######/$d/g'`
#echo ${new_string}
#echo "$to already exists. overwrite (yes/no)? \c"