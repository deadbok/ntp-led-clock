#!/bin/sh

picocom -b 115200 /dev/ttyUSB0 | tee ./test/run-"$(date +%Y-%m-%d-%H-%M-%S)".log

