#!/bin/bash

echo "start up ${date}" >> /home/jakob/log

tmux send -t fidelio /home/jakob/fidelio-capture-bit/fidelio.py Enter
