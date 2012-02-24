#!/bin/sh
adb push /tmp/$1 /sdcard/
adb shell "cd /sdcard ; /data/limare/mali_compile -v $1"
adb pull /sdcard/shader.bin /tmp/$2
