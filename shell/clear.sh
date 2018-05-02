#! /bin/bash

# function sed_hide() {
# 	sed -i 's/@hide/#####H#####/g' ${1}
# }

function digui()
{	
	if [ -d ${1} ]; then
		for dir in $(ls ${1})
		do
			if [ -d "${1}/${dir}" ]; then
				# echo "It's directory."
				digui "${1}/${dir}"

			else
				# doSomething

				file_path=${1}/${dir}

				# filter file
				if [[ ${file_path} =~ "src" ]]; then

					# name=`basename "${1}/${dir}"`
					name=${file_path##*/}
					# echo "${name}"

					if [[  ${find_file} = "true" ]]; then

						if [[ ${name} =~ "java" ]] || [[ ${name} =~ "xml" ]]; then

							result=`cat -n ${file_path}`
							for aline in ${result};
							do
								if [[ ${aline} =~ ${java_anim_name} ]] || [[ ${aline} =~ ${java_drawable_name} ]] || [[ ${aline} =~ ${xml_drawable_name} ]] || [[ ${aline} =~ ${java_layout_name} ]] || [[ ${aline} =~ ${xml_layout_name} ]]; then
									do_delete="true"
									# echo ${need_to_delete_file_path}
									break
								fi
							done
							result=""
						fi

					fi

					if [[ -z ${find_file} ]]; then


						if [[ ${name} =~ "png" ]] || [[ ${name} =~ "jpg" ]] || [[ ${name} =~ "xml" ]]; then
							# name=home_indicator.png
							
							cut_name=${name%.*}
							if [[ ${cut_name} =~ "9" ]]; then
								cut_name=${cut_name%.*}
							fi

							# set mark
							find_file="true"

							# cut_name=home_indicator
							java_anim_name="R.anim.${cut_name}"

							java_drawable_name="R.drawable.${cut_name}"
							xml_drawable_name="@drawable/${cut_name}"

							java_layout_name="R.layout.${cut_name}"
							xml_layout_name="@layout/${cut_name}"

							# java_raw_name="R.raw.${cut_name}"

							need_to_delete_file_path="${file_path}"

							# echo "${xml_drawable_name}"
							# echo "${java_drawable_name}"
							# echo "${file_path}"

							# echo ${need_to_delete_file_path}

							# xml: @drawable/
							# java: R.drawable.

							digui ${currentDir}

							if [[ -z ${do_delete} ]]; then
							# if [[ -z ${do_delete} ]]; then
								if [[ ! ${need_to_delete_file_path} =~ "AndroidManifest.xml" ]] && [[ ! ${need_to_delete_file_path} =~ "values" ]] && [[ ! ${need_to_delete_file_path} =~ "color" ]]; then
									if [[ -n ${need_to_delete_file_path} ]] && [[ -e ${need_to_delete_file_path} ]]; then
										echo ${need_to_delete_file_path} >> delete.txt
										rm -rf ${need_to_delete_file_path}
									fi
								fi


							fi

							clearVariable
							
						fi

					fi
					
				fi

			fi
		done
	fi
}

function clearVariable(){
	cut_name=""
	find_file=""
	java_anim_name=""
	java_drawable_name=""
	xml_drawable_name=""
	java_layout_name=""
	xml_layout_name=""
	need_to_delete_file_path=""
	do_delete=""
}

clearVariable

currentDir=`pwd`
if [ $# -eq 1 ]; then
	digui ${1}
else
	digui ${currentDir}
fi
