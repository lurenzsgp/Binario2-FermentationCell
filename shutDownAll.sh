#!/bin/bash

#configuro i pin gpio20 e gpio21 in output
echo 20 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio20/direction
echo 1 > /sys/class/gpio/gpio20/value

echo 21 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio21/direction
echo 1 > /sys/class/gpio/gpio21/value

echo 1 > /sys/class/gpio/gpio20/value
echo 1 > /sys/class/gpio/gpio21/value
echo "Serpentina e frigo spenti"
