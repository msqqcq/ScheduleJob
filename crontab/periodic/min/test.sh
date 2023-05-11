#!/bin/sh

echo "[$(date '+%Y-%m-%d %H:%M:%S')] testing" >> /app/log/$(date +%Y-%m-%d).log

#curl --request GET -sL \
#     --url 'http://www.baidu.com'\
#     --output '/app/log/test.log'

echo "[$(date '+%Y-%m-%d %H:%M:%S')] test finished" >> /app/log/$(date +%Y-%m-%d).log
echo "" >> /app/log/$(date +%Y-%m-%d).log