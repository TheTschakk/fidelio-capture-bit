#!/bin/bash

source ~/metconf

DATE=$(date) 
echo "start-up at: $DATE" >> $HOME/log

DATEDIR=$(date -I)
mkdir -p $METVID/$DATEDIR

tmux send -t fidelio "cd $METVID/$DATEDIR" Enter
tmux send -t fidelio "$METHOME/python/fidelio.py" Enter
