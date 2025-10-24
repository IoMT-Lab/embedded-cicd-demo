#!/bin/bash

docker run --rm -it --privileged -v /dev/ttyACM0:/dev/ttyACM0 \
  embedded:latest \
  /scripts/deploy.sh broken