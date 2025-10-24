#!/bin/bash

pushd `dirname "$0"`
pushd ../apr/darjeeling
pipenv run darjeeling repair ../repair.yml
popd
popd