#!/bin/bash

pushd `dirname "$0"`
./build_docker.sh

docker run --rm -it --privileged -v /dev/ttyACM0:/dev/ttyACM0 \
  embedded:latest \
  /scripts/deploy.sh build

popd