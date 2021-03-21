#!/bin/bash

# Exit if anything fails
set -e

# $0 = run.sh
# $1 = Database path

RUN_PATH="`dirname \"$0\"`"
RUN_PATH="`( cd \"$RUN_PATH\" && pwd )`"

for migration in $RUN_PATH/*.sql; do
    echo "> Running migration $migration"

    if [[ $migration == *_.sql ]]
    then
        echo "Ignoring possible errors/warnings for this migration"
        sqlite3 $1 < $migration || true
    else
        sqlite3 $1 < $migration
    fi

    echo ""
    echo ""
done

echo "All migrations ran well"

echo "Writing $RUN_PATH/schema.txt file"
sqlite3 $1 '.schema' > $RUN_PATH/schema.txt
echo ""

echo "All done!"
