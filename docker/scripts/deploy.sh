#!/bin/bash

case $1 in 
  "broken")
    python3 /opt/controller/programmer.py -w /binaries/broken.elf
    ;;
  "fixed")
    python3 /opt/controller/programmer.py -w /binaries/fixed.elf
    ;;
    *)
    python3 /opt/controller/programmer.py -w /source/P3.elf
    ;;
esac