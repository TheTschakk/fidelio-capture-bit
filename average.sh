#!/bin/bash

for VID in $@; do

    filename=$VID
    EXT="${filename##*.}"
    FILE="${filename%.*}"

    echo $FILE

    ffmpeg -f rawvideo -pix_fmt gray -s 720x480 -i ${FILE}.bwv -c:v libx264 -f image2pipe pipe:1 | convert - -evaluate-sequence mean ${FILE}.png
done
