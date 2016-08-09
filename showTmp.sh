#!/bin/bash

timestamp=$(date +"%Y-%m-%d %R")
temper=$(grep 't=' /sys/bus/w1/devices/w1_bus_master1/28-0315921cdbff/w1_slave | awk -F 't=' '{print $2}')
echo $timestamp" temperatura --> "$temper # >> temperatura.txt

