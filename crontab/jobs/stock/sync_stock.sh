#!/bin/sh

job_name="sync_stock"
job_dir="/app/log/${job_name}"
if [ ! -d "${job_dir}" ]; then
    mkdir ${job_dir}
fi

today=$(date +%Y-%m)
log_file="${job_dir}/${today}.log"

host=$STOCK_RA_HOST
port=$STOCK_RA_PORT
# shellcheck disable=SC2129
echo "[$(date '+%Y-%m-%d %H:%M:%S')] starting ${job_name} job" >> "${log_file}"

echo "[$(date '+%Y-%m-%d %H:%M:%S')] starting handle dead stocks" >> "${log_file}"
curl_output=$(curl --request GET -sL \
                    --url "http://${host}:${port}/sync/handleDeadStocks"\
                    -w '%{http_code}\n')
echo "[$(date '+%Y-%m-%d %H:%M:%S')] curl output: ${curl_output}" >> ${log_file}

echo "[$(date '+%Y-%m-%d %H:%M:%S')] starting sync newest data" >> "${log_file}"
curl_output2=$(curl --request GET -sL \
                    --url "http://${host}:${port}/sync/newest"\
                    -w '%{http_code}\n')
echo "[$(date '+%Y-%m-%d %H:%M:%S')] curl output: ${curl_output2}" >> ${log_file}

echo "[$(date '+%Y-%m-%d %H:%M:%S')] finished ${job_name} job" >> "${log_file}"
echo "" >> "${log_file}"