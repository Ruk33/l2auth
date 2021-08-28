#!/bin/bash

# Exit if anything fails
set -e

# $0 = run.sh
# $1 = Database path

RUN_PATH="`dirname \"$0\"`"
RUN_PATH="`( cd \"$RUN_PATH\" && pwd )`"

for seed in $RUN_PATH/*.sql; do
    echo "Running migration $seed."

    if [[ $seed == *_.sql ]]
    then
        sqlite3 $1 < $seed >/dev/null 2>&1 || true
    else
        sqlite3 $1 < $seed
    fi
done

echo "Done."
