#!/bin/sh
#
# Average two data files
#

lines=$(wc -l $1 | awk '{ print $1 }')

for (( lineno=1; lineno <= $lines; lineno++ )) do
  line1=$(head -n $lineno $1 | tail -n 1)
  line2=$(head -n $lineno $2 | tail -n 1)

  # ignore comment lines
  if [ "$(echo $line | cut -b1)" = ";" ]; then continue; fi

  meg1=$(echo $line1 | cut -d\  -f1)
  meg2=$(echo $line2 | cut -d\  -f1)
  x_MB1=$(echo $line1 | cut -d\  -f2)
  x_MB2=$(echo $line2 | cut -d\  -f2)
  x_Mb1=$(echo $line1 | cut -d\  -f3)
  x_Mb2=$(echo $line2 | cut -d\  -f3)
  time1=$(echo $line1 | cut -d\  -f4)
  time2=$(echo $line2 | cut -d\  -f4)

  meg=$(echo "scale=3; ($meg1 + $meg2) / 2" | bc)
  x_MB=$(echo "scale=3; ($x_MB1 + $x_MB2) / 2" | bc)
  x_Mb=$(echo "scale=3; ($x_Mb1 + $x_Mb2) / 2" | bc)
  time=$(echo "scale=6; ($time1 + $time2) / 2" | bc)

  echo -e "$meg\t$x_MB\t\t$x_Mb\t$time"
done
