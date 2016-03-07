#!/bin/sh

killall -9 HelloTransfer

ulimit -c unlimited

#./HelloTransfer --config=hello.conf &
./HelloTransfer --config=hello2.conf &
#./HelloTransfer --config=hello3.conf &
