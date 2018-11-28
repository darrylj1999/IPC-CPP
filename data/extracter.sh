#!/bin/bash
# This script extracts data points according to a desired legend
# To change what the legend is compared to, you should change the
# index in awk '{ print }'

infile=$1
legendfile=$2

legend=`cat $legendfile`

while read line
do
# Comparing by b
cmp=`echo $line | awk '{print $1}'`

for check in $legend
do

if [ "$cmp" -eq "$check" ]; then
echo $line
break
fi

done

done < $infile
