#!/bin/bash

# collects smartctl info from HDD/SDD drives
#
# install packets:
#	smartmontools: /usr/sbin/smartctl
#	sysstat: /usr/bin/iostat
#	hwinfo: /usr/sbin/hwinfo
#
# 1. put this script to dir where smart data will be collected
# 2. make symlink to /etc/cron.daily
# or run with sudo

rl=`readlink $0`
if [ $rl ]; then
	script_dir=`dirname "$rl"`
else
	script_dir=`dirname $0`
fi
cd "$script_dir"

dir_name=`date +%Y_%m_%d_%H_%M_%S`
mkdir $dir_name
cd $dir_name
hwinfo --disk --short | grep dev | awk '{print $1}' | while read -r disk; do
    dfile=${disk//\//_}
    echo disk = $disk $dfile
    smartctl -a $disk > $dfile.txt 2>&1
done
iostat > iostat.txt
cd ..
