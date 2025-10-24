#!/bin/bash

pushd `dirname "$0"`

case $1 in 
  "broken")
    python3 /opt/controller/programmer.py -w /binaries/broken.elf
    ;;
  "fixed")
    python3 /opt/controller/programmer.py -w /binaries/fixed.elf
    ;;
    *)
    ./build.sh
    python3 /opt/controller/programmer.py -w /source/P3.elf
    ;;
esac

popd