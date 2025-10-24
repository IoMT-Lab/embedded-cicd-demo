#!/bin/bash

pushd /source
patch -p0 < /patches/0.diff
/scripts/deploy.sh build
popd