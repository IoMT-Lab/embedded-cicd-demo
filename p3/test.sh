#!/bin/bash

test_id=$1
here_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

test_dir="${here_dir}/tests"
outputs_dir="${here_dir}/outputs"

command=""
argument=""

prefix="${test_id:0:1}"
number="${test_id:1}"
number=$(( $number - 1))

if [ "$prefix" = "n" ]
then
    number=9
fi

temp="$(mktemp)"
echo $temp

python3 /opt/controller/programmer.py -t $number > ${temp}; command_status=$?
grep -q "PASS\|IGNORE" ${temp}; pass_status=$?
grep -q FAIL\|UNKNOWN ${temp}; fail_status=$?

! (( command_status || pass_status || !fail_status))