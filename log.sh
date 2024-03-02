#!/bin/sh

picocom -b 9600 /dev/ttyUSB0 | tee ./test/run-"$(date +%Y-%m-%d-%H-%M-%S)".log

