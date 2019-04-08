#!/usr/bin/sh

# VOLUME => STDOUT

MAS=$(amixer get Master  | grep 'Right:')
MTE=$(echo $MAS | awk -F '[][]' '{ print $4 }')
VOL=$(echo $MAS | awk -F '[][]' '{ print $2 }')

if [ $MTE == "on" ]
then
    MTE=""
else
    MTE=""
fi

echo -e "$MTE $VOL"
