#!/usr/bin/env bash

# This script is meant to be run from within the bin directory
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH

# Testing the error part of the library
if [[ -e ./test_error.elf ]]
then
  ./test_error.elf
fi

# Testing the test part of the library
if [[ -e ./test_test.elf ]]
then
  ./test_test.elf
fi
