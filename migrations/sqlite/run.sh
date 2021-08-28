#!/bin/bash

# Exit if anything fails
set -e

# $0 = run.sh
# $1 = Database path

RUN_PATH="`dirname \"$0\"`"
RUN_PATH="`( cd \"$RUN_PATH\" && pwd )`"

for migration in $RUN_PATH/*.sql; do
    echo "Running migration $migration."

    if [[ $migration == *_.sql ]]
    then
        sqlite3 $1 < $migration >/dev/null 2>&1 || true
    else
        sqlite3 $1 < $migration
    fi
done

sqlite3 $1 '.schema' > schema.txt
echo "Done."
