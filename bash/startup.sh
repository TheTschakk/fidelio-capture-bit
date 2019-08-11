#!/bin/bash

source $HOME/metconf

DATE=$(date) 
echo "start-up at: $DATE" >> $HOME/log

DATEDIR=$(date -I)
mkdir -p $METVID/$DATEDIR
mkdir -p $METVID/$DATEDIR/meteors
mkdir -p $METVID/$DATEDIR/others

tmux send -t fidelio "source $HOME/metconf" Enter
tmux send -t fidelio "cd $METVID/$DATEDIR" Enter
tmux send -t fidelio "$METHOME/python/fidelio.py" Enter
tmux send -t fidelio "$METHOME/bash/summary.sh *.bwv" Enter
tmux send -t fidelio "$METHOME/python/classify.py $METVID/$DATEDIR/" Enter

