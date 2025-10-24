#!/bin/bash

pushd `dirname "$0"`
docker run --rm -it --privileged -v /dev/ttyACM0:/dev/ttyACM0 \
  -v $(pwd)/../apr/patches:/patches \
  embedded:latest \
  /scripts/patch.sh

popd