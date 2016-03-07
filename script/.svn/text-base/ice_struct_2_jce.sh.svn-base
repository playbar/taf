#!/bin/sh

if [ $# -lt 1 ]
then
    echo "<Usage:  $0  ice_file>"
    exit 0
fi

awk ' {if($1=="struct"||$1=="{"||$1=="};"){total=-1;print $0;if($1=="struct")stin=1;if($1=="};")stin=0}else {if(stin==0||$1 ==""){print $0}else{total=total+1;print "\t\t"total"\trequire"$0}}}'  $1
