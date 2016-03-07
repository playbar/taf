#!/bin/sh

bin="/usr/local/taf/bin/tafregistry"

PID=`ps -eopid,cmd | grep "$bin"| grep "tafregistry" |  grep -v "grep" |awk '{print $1}'`

echo $PID

if [ "$PID" != "" ]; then
    kill -9 $PID
    echo "kill -9 $PID"
fi

ulimit -c unlimited

$bin  --config=/usr/local/app/taf/tafregistry/conf/registry.config.conf &

