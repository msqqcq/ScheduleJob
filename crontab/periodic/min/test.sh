#!/bin/sh

job_name="test"
job_dir="/app/log/${job_name}"
if [ ! -d "${job_dir}" ]; then
    mkdir ${job_dir}
fi

today=$(date +%Y-%m-%d)
log_file="${job_dir}/${today}.log"

# shellcheck disable=SC2129
echo "[$(date '+%Y-%m-%d %H:%M:%S')] testing" >> "${log_file}"

#curl_output=$(curl --request GET -sL \
#                    --url 'https://www.baidu.com'\
#                    -w '%{http_code}\n')
#echo "[$(date '+%Y-%m-%d %H:%M:%S')] curl output: ${curl_output}" >> ${log_file}

echo "[$(date '+%Y-%m-%d %H:%M:%S')] test finished" >> "${log_file}"
echo "" >> "${log_file}"