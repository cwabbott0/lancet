#!/bin/sh
malisc --frag /tmp/$1 -o /tmp/$2
./malisc-dump /tmp/$2 /tmp/$2
