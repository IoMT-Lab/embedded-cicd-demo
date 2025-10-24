#!/bin/bash

pushd `dirname "$0"`

docker build -f ../docker/Dockerfile.arm -t arm:latest ..
docker build -f ../docker/Dockerfile -t embedded:latest ..

popd