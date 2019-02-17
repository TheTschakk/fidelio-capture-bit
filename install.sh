#!/bin/bash

### first set up "metconf" then run this script (follow instructions given in "config/metconf"!

source "$HOME/metconf"

### compile C code ###

#gcc -o test src/test.c -O3 -g -lm -Wall
gcc -o main src/main.c -O3 -g -lm -Wall


### set up crontab ###

cat <(echo "@reboot $METHOME/bash/screen.sh") \
	<(echo "00 12 * * * $HOME/metconf; $METHOME/bash/startup.sh") \
	> "$METHOME/config/cronfile"

crontab -u $USER "$METHOME/config/cronfile"


### set up environment ###

CONF="source $HOME/metconf"
grep -qF -- "$CONF" "$HOME/.bashrc" || echo "$CONF" >> "$HOME/.bashrc"

touch "$HOME/.tmux.conf"
SHELL="set -g default-shell /bin/bash"
grep -qF -- "$SHELL" "$HOME/.tmux.conf" || echo "$SHELL" >> "$HOME/.tmux.conf"

