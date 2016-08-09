#!/bin/bash

frigo=0
serpe=0

while [ true ] 
do
timestamp=$(date +"%Y-%m-%d %R") 
temper=$(grep 't=' /sys/bus/w1/devices/w1_bus_master1/28-0315921cdbff/w1_slave | awk -F 't=' '{print $2}')
echo $timestamp" temperatura --> "$temper # >> temperatura.txt

if [ $temper -gt 21000 ]  
then
	if [ $frigo -eq 0 ] 
	then
		frigo=1
		# accendiamo il frigo
		echo "frigo on"
	fi
elif [ $frigo -eq 1 ] 
then
		frigo=0
		# spegni frigo
		echo "frigo off"
fi


if [ $temper -lt 19000 ]  
then
        if [ $serpe -eq 0 ] 
	then
                serpe=1
                # accendiamo la serpentina
		echo "serpentina on"
        fi
elif [ $serpe -eq 1 ] 
then
        serpe=0
        # spegni  serpentina
	echo "serpentina off"
fi

sleep 1
done
