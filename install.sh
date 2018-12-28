#!/bin/bash

## compile C code ###

gcc -o test src/test.c -O3 -g -lm -Wall
gcc -o main src/main.c -O3 -g -lm -Wall


### set up crontab ###

echo "@reboot $METHOME/bash/screen.sh" > "$METHOME/misc/cronfile"
echo "00 12 * * * $METHOME/bash/startup.sh" >> "$METHOME/misc/cronfile"
crontab -u $USER "$METHOME/misc/cronfile"


### set up environment

CONF="source $METHOME/misc/metconf"
grep -qF -- "$CONF" "$HOME/.bashrc" || echo "$CONF" >> "$HOME/.bashrc"
