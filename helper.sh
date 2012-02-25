#!/bin/sh
rm /tmp/$2
adb push /tmp/$1 /sdcard/
adb shell "cd /sdcard ; /data/limare/mali_compile $3 $1"
adb pull /sdcard/shader.bin /tmp/$2
