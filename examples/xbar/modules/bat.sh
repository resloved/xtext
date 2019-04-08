#!/usr/bin/bash

# BATTERY => ROFI

BAT="/sys/class/power_supply/BAT0"
PCT=$(cat $BAT/capacity)

if [ "$(cat $BAT/status)" == "Discharging" ]
then
    if [ $PCT -lt 15 ]
    then
        CHG=""
    else
        CHG=""
    fi
else
    if [ $PCT -gt 95 ]
    then
        CHG=""
    else
        CHG=""
    fi
fi

echo -e "$CHG $PCT%"
