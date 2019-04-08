#!/usr/bin/sh

# MICROPHONE => STDOUT

CAP=$(amixer get Capture | grep 'Right:') 
MTE=$(echo $CAP | awk -F '[][]' '{ print $4 }')
VOL=$(echo $CAP | awk -F '[][]' '{ print $2 }')

if [ $MTE == "on" ]
then
    MTE=""
else
    MTE=""
fi

echo -e "$MTE $VOL"