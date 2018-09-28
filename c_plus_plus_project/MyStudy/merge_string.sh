#! /bin/bash

#python exec_strgen.py OSD_Text_List.xlsm

cur_dir="/root/temp_dir/merge_string"
#cur_dir="/media/root/Weidi/Users/weidi5858258/Documents/temp_dir"
#cur_dir="/root/下载/translation_list/Translation_Tool/Translation_Tool"
#cur_dir=`pwd`
# src_dir="/root/下载/translation_list/Translation_Tool/Translation_Tool/TestCase/res"
# src_string_file_path="${src_dir}/values/strings.xml"
# dest_dir="/root/mydev/workspace_android/smartmediaapp/app/src/main/res"

src_dir="${cur_dir}/src/res"
dest_dir="${cur_dir}/dest/res"
cache_file_path="${cur_dir}/cache"

temp_src_string_file_path="${src_dir}/values/temp_strings.xml"
name_string_file_path="${src_dir}/values/name.txt"
content_string_file_path="${src_dir}/values/content.txt"
name_string_count=0
declare -a name_string_array
declare -a content_string_array
declare -a temp_name_string_array
declare -a temp_content_string_array
file_count=0

handling_dir_name=""
handling_dir_src=""
handling_dir_dest=""
add_notes='/*********************************************************************'

function create_new_string() {
    string1=${1}
    string2=""
    if [[ -z ${string1} ]]; then
        return ${string2}
    fi
    for i in `seq 1 ${#string1}`
    do
        a_char=`echo ${string1} | cut -c $i`
        if [[ ${a_char} =~ '\' ]]; then
            string2=${string2}'___!@#$%___'
        else
            string2=${string2}${a_char}
        fi
    done
    echo ${string2}
}

#judge_content_is_same "${string1}" "${string2}"
#if [[ $? -eq 0 ]]; then
#    echo "字符串相同"
#else
#    echo "字符串不同"
#fi
function judge_content_is_same(){
    string1=${1}
    string2=${2}
    if [[ -z ${string1} ]] \
        && [[ -z ${string2} ]]; then
        return 1
    elif [[  -z ${string1}  ]] \
        && [[ -n ${string2} ]]; then
        return 1
    elif [[  -n ${string1}  ]] \
        && [[ -z ${string2} ]]; then
        return 1
    fi

    string1_length=`echo ${#string1}`
    string2_length=`echo ${#string2}`
    #echo ${string1_length}
    #echo ${string2_length}
    if [[ ${string1_length} -ne ${string2_length} ]]; then
        return 1
    fi

    for i in `seq 1 ${#string1}`
    do
        a_char=`echo ${string1} | cut -c $i`

        for j in `seq 1 ${#string2}`
        do
            if [[ ${i} -eq ${j} ]]; then
                b_char=`echo ${string2} | cut -c $j`
                #echo "${a_char} : ${b_char}"
                if [[ ${a_char} != ${b_char} ]]; then
                    #echo "${a_char} @ ${b_char}"
                    return 1
                else
                    break
                fi
            fi
        done
    done

    return 0
}

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

function handle_merge_judge_name_and_content_is_exist_at_array() {
    #echo "handle_merge_judge_name_and_content_is_exist_at_array()"
    # src
    aline=${1}
    # echo ${aline}
    # <string name="proto_popup_details_txt" sony_id="37694">信息</string>
    # 取出string内容
    content_string=${aline%"<"*}
    content_string=${content_string##*">"}
    content_string=`echo "${content_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
    if [[ ${content_string} =~ '\' ]]; then
        content_string=`create_new_string ${content_string}`
    fi
    # echo ${content_string} >> /root/temp_dir/strings.log
    # name="proto_popup_details_txt"
    name_string=`echo ${aline} | awk  -F ' ' '{print $2}'`
    name_string=${name_string%'"'*}
    name_string=${name_string#*'"'}
    name_string=`echo "${name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
    # echo ${name_string} >> /root/temp_dir/name_strings.log
    # ${name_string}字符串存在于目标文件中,但是内容不存在就表示翻译过了,如果还是英文的话就没有翻译过
    # 这里就是要处理翻译过的文言的
    is_name_string_exist="false"
    is_content_string_exist="false"
    for src_name_string in "${name_string_array[@]}"
    do
        #echo ${name_string_}
        if [[ ${src_name_string} == ${name_string} ]]; then
            # echo "handle_merge() ${name_string_} name存在于数组中"
            is_name_string_exist="true"
            break
        fi
    done
    for content_string_ in "${content_string_array[@]}"
    do
        #echo ${content_string_}
        judge_content_is_same ${content_string_} ${content_string}
        if [[ $? -eq 0 ]]; then
            # 表示内容没有翻译,还是英文,不需要移动
            # echo "handle_merge() ${content_string_} content存在于数组中,不需要merge"
            is_content_string_exist="true"
            break
        fi
    done
}

function handle_merge_judge_name_and_content_is_exit_at_dest() {
    #echo "handle_merge_judge_name_and_content_is_exit_at_dest()"
    # echo "handle_merge(): ${name_string} 需要merge的内容:${content_string}"
    is_name_string_exist="false"
    is_content_string_exist="false"
    result_=`grep '<string' ${string_file_path_dest} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
    for aline_ in ${result_};
    do
        if [[ -n ${aline_} ]]; then
            src_name_string=`echo ${aline_} | awk  -F ' ' '{print $2}'`
            src_name_string=${src_name_string%'"'*}
            src_name_string=${src_name_string#*'"'}
            src_name_string=`echo "${src_name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
            if [[ ${src_name_string} == ${name_string} ]]; then
                is_name_string_exist="true"
                break
            fi
        fi
    done
    result_=`grep '<string' ${string_file_path_dest} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
    for aline_ in ${result_};
    do
        content_string_=${aline_%"<"*}
        content_string_=${content_string_##*">"}
        content_string_=`echo "${content_string_}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
        if [[ ${content_string_} =~ '\' ]]; then
            content_string_=`create_new_string ${content_string_}`
        fi
        judge_content_is_same ${content_string_} ${content_string}
        if [[ $? -eq 0 ]]; then
            is_content_string_exist="true"
            break
        fi
    done
}

function handle_merge_name_is_not_exit_at_dest() {
    #echo "handle_merge_name_is_not_exit_at_dest()"
    echo "${name_string} 在目标文件中不存在,需要把内容复制到目标文件中去"
    aline=${1}
    temp_line=`cat -n ${string_file_path_src} | grep "name=\"${name_string}\""`
    if [[ -n ${temp_line} ]]; then
        # 行号
        temp_line=`echo ${temp_line} | awk  -F '[ ;]+' '{print $2}'`
        # sed -n “3p” filename
        temp_line2=${temp_line}
        temp_line3=${temp_line}
        echo "handle_merge_name_is_not_exit_at_dest() temp_line2:${temp_line2} temp_line3:${temp_line3}"
        is_name_string_finded="false"
        # 先往上找
        while :
        do
            let temp_line=${temp_line}-1
            temp_line_content=`sed -n "${temp_line}p" ${string_file_path_src}`
            echo "${temp_line} ${src_name_string} ${temp_line_content}"
            if [[ ${temp_line_content} =~ "<string" ]] \
                && [[ ${temp_line_content} =~ "</string>" ]]; then
                # <string name="proto_popup_details_txt" sony_id="37694">信息</string>
                # name="proto_popup_details_txt"
                temp_line_content=`echo "${temp_line_content}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                src_name_string=`echo ${temp_line_content} | awk  -F ' ' '{print $2}'`
                src_name_string=${src_name_string%'"'*}
                src_name_string=${src_name_string#*'"'}
                src_name_string=`echo "${src_name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                is_name_string_exist="false"
                result_=`grep '<string' ${string_file_path_dest} | grep '</string>'`
                for aline_ in ${result_};
                do
                    aline_=`echo "${aline_}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                    dest_name_string=`echo ${aline_} | awk  -F ' ' '{print $2}'`
                    dest_name_string=${dest_name_string%'"'*}
                    dest_name_string=${dest_name_string#*'"'}
                    dest_name_string=`echo "${dest_name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                    # echo "${src_name_string} ${dest_name_string}"
                    judge_content_is_same ${src_name_string} ${dest_name_string}
                    if [[ $? -eq 0 ]]; then
                        is_name_string_exist="true"
                        break
                    fi
                done
                if [[ ${is_name_string_exist} == "true" ]]; then
                    is_name_string_finded="true"
                    # 如果有注释,那么需要把注释也复制过去
                    temp_notes_content=""
                    while :
                    do
                        let temp_line3=${temp_line3}-1
                        temp_notes=`sed -n "${temp_line3}p" ${string_file_path_src}`
                        if [[ ${temp_notes} =~ "<string" ]] \
                            && [[ ${temp_notes} =~ "</string>" ]]; then
                            break
                        fi
                        if [[ ${temp_notes} =~ "<!--" ]] \
                            && [[ ${temp_notes} =~ "-->" ]]; then
                            temp_notes_content=${temp_notes}
                            break
                        elif [[ ! ${temp_notes} =~ "<!--" ]] \
                            && [[ ${temp_notes} =~ "-->" ]]; then
                            temp_notes_content=${temp_notes_content}${temp_notes}
                        elif [[ ! ${temp_notes} =~ "<!--" ]] \
                            && [[ ! ${temp_notes} =~ "-->" ]]; then
                            temp_notes_content=${temp_notes_content}${temp_notes}
                        elif [[ ${temp_notes} =~ "<!--" ]] \
                            && [[ ! ${temp_notes} =~ "-->" ]]; then
                            temp_notes_content=${temp_notes_content}${temp_notes}
                        fi
                        if [[ ${temp_notes_content} =~ "<!--" ]] \
                            && [[ ${temp_notes_content} =~ "-->" ]]; then
                            break
                        fi
                    done
                    src_name_string=${src_name_string:1:-1}
                    # echo ${aline} ${name_string_}
                    sed -i "/${src_name_string}/a ${aline}" ${string_file_path_dest}

                    name_string=${name_string:1:-1}
                    sed -i "/${name_string}/s/^/    /g" ${string_file_path_dest}
                    # sed -i -e "/\\${name_string_}/a\\${aline}" ${string_file_path_dest}

                    if [[ -n ${temp_notes_content} ]]; then
                        sed -i "/${name_string}/i ${temp_notes_content}" ${string_file_path_dest}

                        sed -i "/${temp_notes_content}/s/^/    /g" ${string_file_path_dest}
                    fi
                    echo "==========>handle_merge() 1 "${aline} ${temp_line_content} ${src_name_string}
                    break
                fi
            elif [[ ${temp_line_content} =~ "<resources" ]]; then
                break
            fi
        done
        # 往下找
        if [[ ${is_name_string_finded} == "false" ]]; then
            temp_line=${temp_line2}
            while :
            do
                let temp_line=${temp_line}+1
                temp_line_content=`sed -n "${temp_line}p" ${string_file_path_src}`
                echo "${temp_line} ${src_name_string} ${temp_line_content}"
                if [[ ${temp_line_content} =~ "<string" ]] \
                    && [[ ${temp_line_content} =~ "</string>" ]]; then
                    # <string name="proto_popup_details_txt" sony_id="37694">信息</string>
                    # name="proto_popup_details_txt"
                    temp_line_content=`echo "${temp_line_content}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                    src_name_string=`echo ${temp_line_content} | awk  -F ' ' '{print $2}'`
                    src_name_string=${src_name_string%'"'*}
                    src_name_string=${src_name_string#*'"'}
                    src_name_string=`echo "${src_name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                    is_name_string_exist="false"
                    result_=`grep '<string' ${string_file_path_dest} | grep '</string>'`
                    for aline_ in ${result_};
                    do
                        aline_=`echo "${aline_}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                        dest_name_string=`echo ${aline_} | awk  -F ' ' '{print $2}'`
                        dest_name_string=${dest_name_string%'"'*}
                        dest_name_string=${dest_name_string#*'"'}
                        dest_name_string=`echo "${dest_name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                        # echo "${src_name_string} ${dest_name_string}"
                        judge_content_is_same ${src_name_string} ${dest_name_string}
                        if [[ $? -eq 0 ]]; then
                            is_name_string_exist="true"
                            break
                        fi
                    done
                    if [[ ${is_name_string_exist} == "true" ]]; then
                        is_name_string_finded="true"
                        # 如果有注释,那么需要把注释也复制过去
                        temp_notes_content=""
                        while :
                        do
                            let temp_line3=${temp_line3}-1
                            temp_notes=`sed -n "${temp_line3}p" ${string_file_path_src}`
                            if [[ ${temp_notes} =~ "<string" ]] \
                                && [[ ${temp_notes} =~ "</string>" ]]; then
                                break
                            fi
                            if [[ ${temp_notes} =~ "<!--" ]] \
                                && [[ ${temp_notes} =~ "-->" ]]; then
                                temp_notes_content=${temp_notes}
                                break
                            elif [[ ! ${temp_notes} =~ "<!--" ]] \
                                && [[ ${temp_notes} =~ "-->" ]]; then
                                temp_notes_content=${temp_notes_content}${temp_notes}
                            elif [[ ! ${temp_notes} =~ "<!--" ]] \
                                && [[ ! ${temp_notes} =~ "-->" ]]; then
                                temp_notes_content=${temp_notes_content}${temp_notes}
                            elif [[ ${temp_notes} =~ "<!--" ]] \
                                && [[ ! ${temp_notes} =~ "-->" ]]; then
                                temp_notes_content=${temp_notes_content}${temp_notes}
                            fi
                            if [[ ${temp_notes_content} =~ "<!--" ]] \
                                && [[ ${temp_notes_content} =~ "-->" ]]; then
                                break
                            fi
                        done
                        src_name_string=${src_name_string:1:-1}
                        sed -i "/${src_name_string}/a ${aline}" ${string_file_path_dest}

                        name_string=${name_string:1:-1}
                        sed -i "/${name_string}/s/^/    /g" ${string_file_path_dest}
                        # sed -i -e "/\\${name_string_}/a\\${aline}" ${string_file_path_dest}

                        if [[ -n ${temp_notes_content} ]]; then
                            sed -i "/${name_string}/i ${temp_notes_content}" ${string_file_path_dest}

                            sed -i "/${temp_notes_content}/s/^/    /g" ${string_file_path_dest}
                        fi
                        echo "==========>handle_merge() 2 "${aline} ${temp_line_content} ${src_name_string}
                        break
                    fi
                elif [[ ${temp_line_content} =~ "</resources>" ]]; then
                    break
                fi
            done
        fi
    #else
        #sed -i "/<\/resources>/i ${aline}" ${string_file_path_dest}
        #echo "**********>handle_merge() "${aline}
    fi
}

function handle_merge_name_is_exist_at_dest() {
    #echo "handle_merge_name_is_exist_at_dest()"
    # ${name_string}存在的话,还需要判断内容是否有改变,不存在就表示有改变
    # if [[ -z `cat "${string_file_path_dest}" | grep "${content_string}">/dev/null` ]]; then
    # if [[ `grep -c -w ${content_string} ${string_file_path_dest}` -eq '0' ]]; then
    if [[ ${is_content_string_exist} == "false" ]]; then
        echo "${name_string} 在目标文件中存在,但是内容有变化,需要把修改后的内容复制到目标文件中去"
        name_string=${name_string:1:-1}
        # 在指定字段行的上一行增加一行内容
        # sed -i -e "/\\${name_string}/i\\${aline}" ${string_file_path_dest}
        sed -i "/${name_string}/i ${aline}" ${string_file_path_dest}
        # 删除指定字段行的下一行
        # sed -i "/\\${name_string}/{n;d}" ${string_file_path_dest}
        sed -i "/${name_string}/{n;d}" ${string_file_path_dest}
        echo "---------->handle_merge() "${aline} ${name_string}

        sed -i "/${name_string}/s/^/    /g" ${string_file_path_dest}
    fi
}

function handle_merge_src_name_is_exist_at_array() {
    #echo "handle_merge_name_is_exist_with_array()"
    aline=${1}
    handle_merge_judge_name_and_content_is_exit_at_dest ${aline}

    #echo "handle_merge_src_name_is_exist_at_array() dest is_name_string_exist:${is_name_string_exist} is_content_string_exist:${is_content_string_exist}"

    # if [[ -z `cat "${string_file_path_dest}" | grep "${name_string}">/dev/null` ]]; then
    # if [[ `grep -c -w ${name_string} ${string_file_path_dest}` -eq '0' ]]; then
    if [[ ${is_name_string_exist} == "false" ]]; then
        handle_merge_name_is_not_exit_at_dest ${aline}
    else
        # echo "----------------------------->${aline}"
        handle_merge_name_is_exist_at_dest ${aline}
    fi
}

function handle_merge() {
	IFS_old=${IFS}
    IFS=$'\n'

    string_file_path_src=${1}
    string_file_path_dest=${2}
    # echo "${string_file_path_src}"
    # echo "${string_file_path_dest}"

    result=`grep '<string' ${string_file_path_src} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            handle_merge_judge_name_and_content_is_exist_at_array ${aline}

            #echo "handle_merge() src name_string:${name_string} content_string:${content_string}"

            if [[ ${is_name_string_exist} == "true" ]] \
                && [[ ${is_content_string_exist} == "false" ]]; then
                handle_merge_src_name_is_exist_at_array ${aline}
            fi
        fi
    done

    IFS=${IFS_old}
}

function handle_merge2() {
	IFS_old=${IFS}
    IFS=$'\n'

    string_file_path_src=${1}
    string_file_path_dest=${2}
    # echo "${string_file_path_src}"
    # echo "${string_file_path_dest}"

    result=`grep '<string' ${string_file_path_src} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            # <string name="proto_popup_details_txt" sony_id="37694">信息</string>
            handle_merge_judge_name_and_content_is_exist_at_array ${aline}

            echo "handle_merge2() name_string:${name_string} content_string:${content_string}"
            echo "handle_merge2() is_name_string_exist:${is_name_string_exist} is_content_string_exist:${is_content_string_exist}"

            if [[ ${is_name_string_exist} == "true" ]] \
                && [[ ${is_content_string_exist} == "false" ]]; then
                # 找到源文件中需要被复制行的name_string
                aline=`echo "${aline}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                src_name_string=`echo ${aline} | awk  -F ' ' '{print $2}'`
                src_name_string=${src_name_string%'"'*}
                src_name_string=${src_name_string#*'"'}
                src_name_string=`echo "${src_name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                # 根据name_string找到行号
                temp_line=`cat -n ${string_file_path_src} | grep "name=\"${src_name_string}\""`
                # 行号
                temp_line=`echo ${temp_line} | awk  -F '[ ;]+' '{print $2}'`

                temp_notes_content=""
                while :
                do
                    let temp_line=${temp_line}-1
                    temp_notes=`sed -n "${temp_line}p" ${string_file_path_src}`
                    if [[ ${temp_notes} =~ "<string" ]] \
                        && [[ ${temp_notes} =~ "</string>" ]]; then
                        break
                    fi
                    if [[ ${temp_notes} =~ "<!--" ]] \
                        && [[ ${temp_notes} =~ "-->" ]]; then
                        temp_notes_content=${temp_notes}
                        break
                    elif [[ ! ${temp_notes} =~ "<!--" ]] \
                        && [[ ${temp_notes} =~ "-->" ]]; then
                        temp_notes_content=${temp_notes_content}${temp_notes}
                    elif [[ ! ${temp_notes} =~ "<!--" ]] \
                        && [[ ! ${temp_notes} =~ "-->" ]]; then
                        temp_notes_content=${temp_notes_content}${temp_notes}
                    elif [[ ${temp_notes} =~ "<!--" ]] \
                        && [[ ! ${temp_notes} =~ "-->" ]]; then
                        temp_notes_content=${temp_notes_content}${temp_notes}
                    fi
                    if [[ ${temp_notes_content} =~ "<!--" ]] \
                        && [[ ${temp_notes_content} =~ "-->" ]]; then
                        break
                    fi
                done

                sed -i "/<\/resources>/i ${aline}" ${string_file_path_dest}
                echo "==========>handle_merge2() "${src_name_string} ${content_string}

                sed -i "/${src_name_string}/s/^/    /g" ${string_file_path_dest}

                if [[ -n ${temp_notes_content} ]]; then
                    sed -i "/${src_name_string}/i ${temp_notes_content}" ${string_file_path_dest}

                    # 根据name_string找到行号
                    dest_temp_line=`cat -n ${string_file_path_dest} | grep "name=\"${src_name_string}\""`
                    # 行号
                    dest_temp_line=`echo ${dest_temp_line} | awk  -F '[ ;]+' '{print $2}'`
                    let dest_temp_line=${dest_temp_line}-1

                    sed -i "${dest_temp_line}s/^/    /g" ${string_file_path_dest}
                    # sed -i "/${temp_notes_content}/s/^/    /g" ${string_file_path_dest}
                fi
            fi
        fi
    done

    IFS=${IFS_old}
}

function handle_copy() {
#    if [[ "1" == "1" ]]; then
#        return
#    fi

    echo 'handle_copy()'
    string_file_path_src=${1}
    string_file_path_dest=${2}

    date=""
    result=`cat ${string_file_path_src}`
    for aline in ${result};
    do
        echo ${aline}
        echo ${aline} >> ${string_file_path_dest}
        if [[ ${aline} =~ "/initrd.img.old" ]]; then
            echo "handle_copy() 包含initrd.img.old目录"
            data="true"
        fi
#        if [[ ${aline} =~ "/data" ]]; then
#            data="true"
#        fi
        if [[ ${aline} =~ "<resources" ]]; then
            break
        fi
    done

#    if [[ -n ${data} ]]; then
#        # 特殊处理
#        # sed -i '3,27d' ${string_file_path_dest}
#        # sed -i -e "/<!--/a\\${add_notes}" ${string_file_path_dest}
#        echo "handle_copy() 特殊处理"
#        sed -i '4,35d' ${string_file_path_dest}
#        sed -i -e "/<!--/a\\${add_notes}" ${string_file_path_dest}
#        sed -i '3d' ${string_file_path_dest}
#    fi

    echo "</resources>" >> ${string_file_path_dest}

    # echo "handle_copy() ${string_file_path_src} ${string_file_path_dest}"
    handle_merge2 ${string_file_path_src} ${string_file_path_dest}
#    handle_merge ${string_file_path_src} ${string_file_path_dest}

}

function handle_copy_need() {
    handling_dir_dest=${1}
    string_file_path_src=${2}
    string_file_path_dest=${3}
    if [[ ! -e ${handling_dir_dest} ]]; then
        mkdir -p ${handling_dir_dest}
        touch ${string_file_path_dest}
    fi

    echo "${country} need to copy"
    handle_copy ${string_file_path_src} ${string_file_path_dest}

    if [[ `grep -c "sony_id=" ${string_file_path_dest}` -eq '0' ]]; then
        rm -rf ${handling_dir_dest}
    fi
}

function handle_one_country() {
    IFS_old=${IFS}
    IFS=$'\n'

    # values-af-rZA
    # values-bn
    country=${1}

    string_file_path_src="${src_dir}/${country}/strings.xml"
    handling_dir_dest="${dest_dir}/${country}"
    # echo ${string_file_path_src}
    # echo ${handling_dir_dest}
    string_file_path_dest="${handling_dir_dest}/strings.xml"

    if [[ -e ${string_file_path_dest} ]]; then
        # echo "handle_one_country() ${string_file_path_dest} 存在"
        handle_merge ${string_file_path_src} ${string_file_path_dest}
    else
        # echo "handle_one_country() ${string_file_path_dest} 不存在"
        # values-af
        language=${country%"-"*}
        if [[ ${language} != "values" ]]; then
            handling_dir_dest="${dest_dir}/${language}"
            string_file_path_dest="${handling_dir_dest}/strings.xml"
            if [[ -e ${string_file_path_dest} ]]; then
                #echo ${string_file_path_src} ${string_file_path_dest}
                handle_merge ${string_file_path_src} ${string_file_path_dest}
            else
                handling_dir_dest="${dest_dir}/${country}"
                string_file_path_dest="${handling_dir_dest}/strings.xml"
                handle_copy_need ${handling_dir_dest} ${string_file_path_src} ${string_file_path_dest}
            fi
        else
            handling_dir_dest="${dest_dir}/${country}"
            string_file_path_dest="${handling_dir_dest}/strings.xml"
            handle_copy_need ${handling_dir_dest} ${string_file_path_src} ${string_file_path_dest}
        fi
    fi

    IFS=${IFS_old}
}

function handle_more_country() {
    IFS_old=${IFS}
    IFS=$'\n'

    # .../cache/values-zh.txt
    cache_file=${1}

    # 先判断相同sony_id的内容是否相同
    is_content_same="true"

    temp_name_string_length=0

    # 循环各个国家
    result=`cat ${cache_file}`
    # values-zu-rZA
    for country in ${result};
    do
        string_file_path_src="${src_dir}/${country}/strings.xml"
        temp_name_string_length=`cat ${string_file_path_src} | grep '<string' | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M' | wc -l`
        break
    done

    let temp_name_string_length=$((${temp_name_string_length}*${file_count}))
    echo ${temp_name_string_length}
    if [[ ${temp_name_string_length} -ne 0 ]]; then
        i=0
        while [[ ${i} -lt ${temp_name_string_length} ]]
        do
            #echo "i = ${i}"
            temp_name_string_array[${i}]=""
            temp_content_string_array[${i}]=""
            let i=${i}+1
        done
    fi

    i=0
    # 循环各个国家
    result=`cat ${cache_file}`
    # values-zu-rZA
    for country in ${result};
    do
        # values-zu
        language=${country%"-"*}

        string_file_path_src="${src_dir}/${country}/strings.xml"

        echo ${string_file_path_src}
        result=`grep '<string' ${string_file_path_src} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
        for aline in ${result};
        do
            if [[ -n ${aline} ]]; then
                # <string name="proto_popup_details_txt" sony_id="37694">信息</string>
                # name="proto_popup_details_txt"
                name_string=`echo ${aline} | awk  -F ' ' '{print $2}'`
                name_string=${name_string%'"'*}
                name_string=${name_string#*'"'}
                name_string=`echo "${name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
                # 取出string内容
                content_string=${aline%"<"*}
                content_string=${content_string##*">"}
                if [[ ${content_string} =~ '\' ]]; then
                    content_string=`create_new_string ${content_string}`
                fi
                # echo ${country} ${name_string} ${content_string}

                is_name_string_exist="false"
                if [[ -n temp_name_string_array[0] ]]; then
                    for src_name_string in "${temp_name_string_array[@]}"
                    do
                        if [[ ${src_name_string} == ${name_string} ]]; then
                            is_name_string_exist="true"
                            break
                        fi
                    done
                fi

                is_content_string_exist="false"
                if [[ -n temp_content_string_array[0] ]]; then
                    for content_string_ in "${temp_content_string_array[@]}"
                    do
                        judge_content_is_same ${content_string_} ${content_string}
                        if [[ $? -eq 0 ]]; then
                            is_content_string_exist="true"
                            break
                        fi
                    done
                fi

                if [[ ${is_name_string_exist} == "true" ]]; then
                    if [[ ${is_content_string_exist} == "false" ]]; then
                        is_content_same="false"
                        break
                    fi
                else
                    # echo ${name_string} ${content_string}
                    temp_name_string_array[${i}]=${name_string}
                    temp_content_string_array[${i}]=${content_string}
                    let i=${i}+1
                    is_content_same="true"
                fi
            fi # if [[ -n ${aline} ]]; then
        done
        if [[ ${is_content_same} == "false" ]]; then
            break
        fi
    done

#    if [[ ${is_content_same} == "true" ]]; then
#        echo ${language} "内容相同"
#        return 0
#    else
#        echo ${language} "内容不同"
#        return 0
#    fi

    if [[ ${is_content_same} == "true" ]]; then
        echo "内容相同"
        # 内容相同
        result=`cat ${cache_file}`
        # values-zu-rZA
        for country in ${result};
        do
            string_file_path_src="${src_dir}/${country}/strings.xml"
            handling_dir_dest="${dest_dir}/${country}"
            string_file_path_dest="${handling_dir_dest}/strings.xml"

            # echo ${country}
            # values-zu
            country2=${country%"-"*}
            string_file_path_dest2=""
            if [[ ${country2} != "values" ]]; then
                handling_dir_dest2="${dest_dir}/${country2}"
                string_file_path_dest2="${handling_dir_dest2}/strings.xml"
            fi

            echo "${country} ${is_content_same} need to merge"
            # merge
            if [[ -e ${string_file_path_dest} ]]; then
                echo "handle_more_country() ${string_file_path_src} ${string_file_path_dest}"
                handle_merge ${string_file_path_src} ${string_file_path_dest}
                if [[ -n ${string_file_path_dest2} ]] \
                    && [[ -e ${string_file_path_dest2} ]]; then
                    handle_merge ${string_file_path_src} ${string_file_path_dest2}
                fi
            elif [[ -n ${string_file_path_dest2} ]] \
                && [[ -e ${string_file_path_dest2} ]]; then
                echo "handle_more_country() ${string_file_path_src} ${string_file_path_dest2}"
                handle_merge ${string_file_path_src} ${string_file_path_dest2}
            elif [[ ! -e ${string_file_path_dest} ]] \
                && [[ ! -e ${string_file_path_dest2} ]]; then
                if [[ -z ${string_file_path_dest2} ]]; then
                    handle_copy_need ${handling_dir_dest} ${string_file_path_src} ${string_file_path_dest}
                else
                    handle_copy_need ${handling_dir_dest2} ${string_file_path_src} ${string_file_path_dest2}
                fi
            fi
            # break
        done
    else
        echo "内容不同"
        # 内容不同
        echo "handle_more_country(): "${cache_file}
        result=`cat ${cache_file}`
        # values-zu-rZA
        for country in ${result};
        do
            string_file_path_src="${src_dir}/${country}/strings.xml"
            handling_dir_dest="${dest_dir}/${country}"
            # .../res/values-es-rUS/strings.xml
            string_file_path_dest="${handling_dir_dest}/strings.xml"

            # echo ${country}
            country2=${country%"-"*}
            string_file_path_dest2=""
            if [[ ${country2} != "values" ]]; then
                handling_dir_dest2="${dest_dir}/${country2}"
                string_file_path_dest2="${handling_dir_dest2}/strings.xml"
            fi

            echo "handle_more_country() ${country}: ${string_file_path_src} ${string_file_path_dest}"
            if [[ -e ${string_file_path_dest} ]]; then
                echo "handle_more_country() ${string_file_path_src} ${string_file_path_dest}"
                handle_merge ${string_file_path_src} ${string_file_path_dest}
            elif [[ -n ${string_file_path_dest2} ]] \
                && [[ -e ${string_file_path_dest2} ]]; then
                echo "handle_more_country() ${string_file_path_src} ${string_file_path_dest2}"
                handle_merge ${string_file_path_src} ${string_file_path_dest2}
            elif [[ ! -e ${string_file_path_dest} ]] \
                && [[ ! -e ${string_file_path_dest2} ]]; then
                handle_copy_need ${handling_dir_dest} ${string_file_path_src} ${string_file_path_dest}
            fi

#            if [[ -e ${string_file_path_dest} ]]; then
#                # merge
#                echo "${country} ${is_content_same} need to merge"
#                handle_merge ${string_file_path_src} ${string_file_path_dest}
#            else
#                handle_copy_need ${handling_dir_dest} ${string_file_path_src} ${string_file_path_dest}
#            fi
        done
    fi

    if [[ ${temp_name_string_length} -ne 0 ]]; then
        i=0
        while [[ ${i} -lt ${temp_name_string_length} ]]
        do
            #echo "i = ${i}"
            temp_name_string_array[${i}]=""
            temp_content_string_array[${i}]=""
            let i=${i}+1
        done
    fi
    
    IFS=${IFS_old}
}

function handle_cache_file() {
    echo "handle_cache_file()"

    current_time=`date +"%y/%m/%d %H:%M:%S"`
    echo ${current_time}

    for cache_file in $(ls ${1})
    do
        cache_file="${1}/${cache_file}"
        if [[ ${cache_file} =~ "values-" ]] \
            && [[ ! ${cache_file} =~ "land" ]] \
            && [[ ! ${cache_file} =~ "dp" ]]; then
            # echo ${cache_file}
            if [[ -n ${cache_file} ]]; then
                file_name=${cache_file##*"/"}
                judge_content_contain_digit ${file_name}
                if [[ $? -ne 0 ]]; then
                    file_count=`cat ${cache_file} | wc -l`
                    if [[ ${file_count} -eq 1 ]]; then
                        let test_count=${test_count}+1
                        # values-af-rZA
                        cache_file_content=`cat ${cache_file}`
                        echo "handle_cache_file() handle_one_country: "${cache_file_content}
                        handle_one_country ${cache_file_content}
                    else
                        echo "handle_cache_file() handle_more_country: "${cache_file}
                        # 相同语言多个国家
                        # .../cache/values-zh.txt
                        handle_more_country ${cache_file}
                    fi
                fi
            fi
        fi
    done

    current_time=`date +"%y/%m/%d %H:%M:%S"`
    echo ${current_time}
}

# 对源文件夹中的各个目录进行操作
# values-de-rAT
# values-de-rCH
function handle_string_src() {
    # .../values-zu-rZA/strings.xml
    string_file_path_src=${1}
    # .../values-zu-rZA
    handling_dir_src=${string_file_path_src%"/"*}
    # values-zu-rZA values-bn
    handling_dir_src2=${handling_dir_src##*"/"}
    # values-zu values
    handling_dir_src3=${handling_dir_src2%"-"*}
    # .../values-zu-rZA/strings.xml
    handling_dir_src=${string_file_path_src}

    # 把相同语言的不同国家放在一个文件中
    values_country_str_count=`echo ${#handling_dir_src2}`
    country_file_path=""
    if [[ ${values_country_str_count} -eq 13 ]]; then
        country_file_path="${cache_file_path}/${handling_dir_src3}.txt"
    else
        country_file_path="${cache_file_path}/${handling_dir_src2}.txt"
    fi
    if [[ -e ${country_file_path} ]]; then
        if [[ `grep -c "${handling_dir_src2}" ${country_file_path}` -eq '0' ]]; then
            echo "${handling_dir_src2}" >> ${country_file_path}
        fi
    else
        echo "${handling_dir_src2}" >> ${country_file_path}
    fi
}

function digui_src() {    
    if [ -d ${1} ]; then
        for dir in $(ls ${1})
        do
            tempDir="${1}/${dir}"
            if [ -d ${tempDir} ]; then
                # echo "It's directory."
                values_country=${tempDir##*"/"}
                # echo ${values_country}
                # 处理除values文件夹之外的其他所有文件夹
                if [[ ${values_country} != "values" ]]; then
                    # echo ${tempDir}
                    digui_src ${tempDir}
                fi
            else
                # doSomething
                handle_string_src ${tempDir}
                # echo "It's not directory." 
            fi
        done
    fi
}

notes='    <!-- Label name for searchable activity -->'
notes2='    <!-- dtcpip chapter title -->'

function last_add_notes() {
    echo "last_add_notes()"

	# dest_dir
    if [ -d ${1} ]; then
        for dir in $(ls ${1})
        do
            tempDir="${1}/${dir}"
            if [ -d ${tempDir} ]; then
                # echo "It's directory."
                values_country=${tempDir##*"/"}
                # echo ${values_country}
                # 处理除values文件夹之外的其他所有文件夹
                if [[ ${values_country} =~ 'values-' ]]; then
	                if [[ ${values_country} != "values" ]] \
	                	&& [[ ! ${values_country} =~ "dp" ]]; then
	                    # echo ${tempDir}
	                    last_add_notes ${tempDir}
	                fi
                fi
            else
                # doSomething
                echo ${tempDir}
                
                IFS_old=${IFS}
    			IFS=$'\n'

    			result=`cat ${tempDir}`
		        for aline in ${result};
		        do
		            # .../res/values-sl/strings.xml
		            # echo ${aline}

		            if [[ ${aline} =~ 'bdr_searchable_activity_label' ]]; then
		            	# 在指定字段行的上一行增加一行内容
                        sed -i -e "/bdr_searchable_activity_label/i\\${notes}" ${tempDir}
		            elif [[ ${aline} =~ 'video_player_chapter_title_txt' ]]; then
		            	# 在指定字段行的上一行增加一行内容
                        sed -i -e "/video_player_chapter_title_txt/i\\${notes2}" ${tempDir}
		            fi
		        done

				IFS=${IFS_old}
            fi
        done
    fi
}

function create_wanted_file() {
    echo "create_wanted_file()"

    IFS_old=${IFS}
    IFS=$'\n'

    result=`grep '<string' ${src_string_file_path} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            # 当前行有sony_id字符串,但是没有sony_id="M这样的字符串
            echo ${aline} >> ${temp_src_string_file_path}
        fi
    done

    result=`cat ${temp_src_string_file_path}`
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            aline=`echo "${aline}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
            name_string=`echo ${aline} | awk  -F ' ' '{print $2}'`
            name_string=${name_string%'"'*}
            name_string=${name_string#*'"'}
            name_string=`echo "${name_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
#            echo ${name_string} >> ${name_string_file_path}
            # 取出string内容
            content_string=${aline%"<"*}
            content_string=${content_string##*">"}
            if [[ ${content_string} =~ '\' ]]; then
                content_string=`create_new_string ${content_string}`
                echo "create_wanted_file() content_string: ${content_string}"
            fi

#            echo ${content_string} >> ${content_string_file_path}
            # 把数据保存到数组中
            name_string_array[${name_string_count}]=${name_string}
            content_string_array[${name_string_count}]=${content_string}
            let name_string_count=${name_string_count}+1
        fi
    done

#    echo "${name_string_array[@]}" | grep -wq "device_connection_check_msg_txt" &&  echo "Yes" || echo "No"
#    echo "${content_string_array[@]}" | grep -wq "%1$s needs your permission" &&  echo "Yes" || echo "No"
#
#    for name_string in "${name_string_array[@]}"
#    do
#        echo ${name_string}
#    done
#
#    for content_string in "${content_string_array[@]}"
#    do
#        echo ${content_string}
#    done

    IFS=${IFS_old}
}

function destroy() {
    echo "destroy()"

    if [[ -e ${temp_src_string_file_path} ]]; then
        rm -rf ${temp_src_string_file_path}
    fi
    if [[ -e ${name_string_file_path} ]]; then
        rm -rf ${name_string_file_path}
    fi
    if [[ -e ${content_string_file_path} ]]; then
        rm -rf ${content_string_file_path}
    fi
}

function my_test() {
    file1="${cache_file_path}/values-de-content.txt"
    file2="${cache_file_path}/values-de-content2.txt"

    IFS_old=${IFS}
    IFS=$'\n'

    result=`cat ${file1}`
    for aline in ${result};
    do
        if [[ -n ${aline} ]]; then
            # 取出string内容
            content_string=${aline%"<"*}
            content_string=${content_string##*">"}
            content_string=`echo "${content_string}" | grep -o "[^ ]\+\( \+[^ ]\+\)*"`

            echo ${content_string}
        fi
    done

    IFS=${IFS_old}
}



#############################start#############################

# 把缓存文件夹删除
if [[ -e ${cache_file_path} ]]; then
    rm -rf ${cache_file_path}
fi
mkdir -p ${cache_file_path}
destroy
src_string_file_path="${src_dir}/values/strings.xml"
dest_string_file_path="${dest_dir}/values/strings.xml"
create_wanted_file
digui_src ${src_dir}
handle_cache_file ${cache_file_path}
src_string_file_path="${src_dir}/values/strings.xml"
dest_string_file_path="${dest_dir}/values/strings.xml"
handle_merge ${src_string_file_path} ${dest_string_file_path}

#last_add_notes ${dest_dir}
#destroy

#############################test#############################

#my_test

#if [[ ${string1} =~ '\' ]]; then
#    echo "包含"
#else
#    echo "不包含"
#fi


#if [[ ${string1} =~ '\' ]]; then
#    echo "0"
#else
#    echo "1"
#fi
#
#string1=`echo ${string1} | sed -e 's/-/@/g'`
#echo ${string1}

#string1='Wählen Sie [Berechtigungen bearbeiten] &gt; [Berechtigungen], um die Berechtigung zu gewähren.'
#string2='Wählen Sie [Berechtigungen bearbeiten] &gt; [Berechtigungen], um die Berechtigung zu gewähren.'
#if [[ ${string1} =~ '\' ]]; then
#    string1=`create_new_string ${string1}`
#fi
#if [[ ${string2} =~ '\' ]]; then
#    string2=`create_new_string ${string2}`
#fi
#echo "string1: ${string1}"
#echo "string2: ${string2}"
#if [[ ${string1} == ${string2} ]]; then
#    echo "字符串相同"
#else
#    echo "字符串不同"
#fi
#judge_content_is_same "${string1}" "${string2}"
#if [[ $? -eq 0 ]]; then
#    echo "字符串相同"
#else
#    echo "字符串不同"
#fi

#COUNTER=0
#while [[ $COUNTER -lt 10 ]]
#do
#    echo $COUNTER
#    let COUNTER++
#done

#result=`grep '<string' ${src_string_file_path} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
#for aline in ${result};
#do
#    echo ${aline}
#done
#IFS_old=${IFS}
#IFS=$'\n'
#string_file_path_src='/mnt/c/Users/weidi5858258/Documents/temp_dir/src/res/values-de-rAT/strings.xml'
#result=`grep '<string' ${string_file_path_src} | grep 'sony_id="' | grep '</string>' | grep -v 'sony_id="M'`
#for aline in ${result};
#do
#    echo ${aline}
#done
#IFS=${IFS_old}