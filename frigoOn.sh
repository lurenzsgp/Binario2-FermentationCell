#!/bin/bash

echo 20 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio20/direction
echo 0 > /sys/class/gpio/gpio20/value
echo "frigo On"
