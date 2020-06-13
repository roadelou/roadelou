#!/usr/bin/env bash

# Source : https://stackoverflow.com/questions/59895/how-to-get-the-source-directory-of-a-bash-script-from-within-the-script-itself
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

export LD_LIBRARY_PATH=$DIR/:$LD_LIBRARY_PATH
exec $DIR/test_libRoadelou.elf
