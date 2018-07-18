#!/bin/bash

source ./config.sh
tmux send -t fidelio "source $METHOME/bash/config.sh" Enter

DATE=$(date) 
echo "start-up at: $DATE" >> $HOME/log

tmux send -t fidelio $METHOME/python/fidelio.py Enter
