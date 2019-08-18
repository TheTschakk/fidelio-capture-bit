#!/bin/bash

### first set up "metconf" then run this script (follow instructions given in "config/metconf"!

echo 'Step 1: Loading configuration from "~/metconf". Hope you copied it there with appropriate parameters'
source "$HOME/metconf"

### compile C code ###

echo 'Step 2: Compiling C code'
#gcc -o test src/test.c -O3 -g -lm -lpthread -Wall
gcc -o $METHOME/main $METHOME/src/main.c -O3 -g -lm -lpthread -Wall


### set up crontab ###
	#<(echo "00 12 * * * $HOME/metconf; $METHOME/bash/startup.sh") \

echo 'Step 3: Setting up crontab scheduler'
cat <(echo "@reboot $METHOME/bash/screen.sh") \
	<(echo "00 12 * * * $METHOME/bash/startup.sh") \
	> "$METHOME/config/cronfile"

crontab -u $USER "$METHOME/config/cronfile"


### set up environment ###

echo 'Step 4: Setting up user environment'
CONF="source $HOME/metconf"
grep -qF -- "$CONF" "$HOME/.bashrc" || echo "$CONF" >> "$HOME/.bashrc"

touch "$HOME/.tmux.conf"
SHELL="set -g default-shell /bin/bash"
grep -qF -- "$SHELL" "$HOME/.tmux.conf" || echo "$SHELL" >> "$HOME/.tmux.conf"

