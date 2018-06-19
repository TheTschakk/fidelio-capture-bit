#!/bin/bash

DATE=$(date) 
echo "start-up at: $DATE" >> /home/jakob/log

tmux send -t fidelio /home/jakob/fidelio-capture-bit/fidelio.py Enter
