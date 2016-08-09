#!/bin/bash

echo "Avvio cella di fermentazione"

#configuro i pin gpio20 e gpio21 in output
echo 20 > /sys/class/gpio/export 
echo out > /sys/class/gpio/gpio20/direction 
echo 1 > /sys/class/gpio/gpio20/value

echo 21 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio21/direction 
echo 1 > /sys/class/gpio/gpio21/value

frigo=0
serpe=0
tmax=19500
tmin=16500
tmiddle=18000

while [ true ] 
do
timestamp=$(date +"%Y-%m-%d %R") 
temper=$(grep 't=' /sys/bus/w1/devices/w1_bus_master1/28-0315921cdbff/w1_slave | awk -F 't=' '{print $2}')
#echo $timestamp" temperatura --> "$temper # >> temperatura.txt

if [ $temper -gt $tmax ]  
then
if [ $frigo -eq 0 ] 
then
frigo=1
echo 0 > /sys/class/gpio/gpio20/value
echo "frigo on"
fi
fi
if [ $frigo -eq 1 ] 
then
if [ $temper -lt $tmiddle ]
then
frigo=0
echo 1 > /sys/class/gpio/gpio20/value
echo "frigo off"
fi
fi


if [ $temper -lt $tmin ]  
then
if [ $serpe -eq 0 ] 
then
serpe=1
echo 0 > /sys/class/gpio/gpio21/value
echo "serpentina on"
fi
fi
if [ $serpe -eq 1 ] 
then
if [ $temper -gt $tmiddle ]
then
serpe=0
echo 1 > /sys/class/gpio/gpio21/value
echo "serpentina off"
fi
fi

sleep 1
done
