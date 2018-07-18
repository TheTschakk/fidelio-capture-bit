#!/bin/bash

source ./config.sh
tmux send -t fidelio "source $METHOME/bash/config.sh" Enter

DATE=$(date) 
echo "start-up at: $DATE" >> $HOME/log

DATEDIR=$(date -I)
mkdir -p $METVID/$DATEDIR
cd $METVID/$DATEDIR

tmux send -t fidelio $METHOME/python/fidelio.py Enter
